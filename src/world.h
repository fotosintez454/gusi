static void high_draw180(void);static void high_lights180(void);
#include "game.h"
static int render_mirror=0; /* Compatibility for older capture tools: mirror removed. */
static float worldclock=0;
static V3 vp(float x,float y,float z){V3 a={x,y,z};return a;}
static void quadstrip(V3 a,V3 b,V3 c,V3 d,U32 col){quad(a,b,c,d,col,0,1);}
#include "update202_draw.h"
#include "architecture203_draw.h"
static V3 blend3(V3 a,V3 b,float t){return vp(lerp(a.x,b.x,t),lerp(a.y,b.y,t),lerp(a.z,b.z,t));}
/* Articulated limbs and hinged hardware share actual world-space coordinates. */
static void beam(V3 a,V3 b,float ra,float rb,U32 color){V3 d=vp(b.x-a.x,b.y-a.y,b.z-a.z);float len=(float)sqrt(d.x*d.x+d.y*d.y+d.z*d.z);if(len<.001f)return;d.x/=len;d.y/=len;d.z/=len;
 V3 u=ab(d.y)<.95f?vp(-d.z,0,d.x):vp(1,0,0);float ul=(float)sqrt(u.x*u.x+u.y*u.y+u.z*u.z);u.x/=ul;u.y/=ul;u.z/=ul;V3 v=vp(d.y*u.z-d.z*u.y,d.z*u.x-d.x*u.z,d.x*u.y-d.y*u.x);
 for(int n=0;n<6;n++){float x=n*PI/3,y=(n+1)*PI/3;float ca=(float)cos(x),sa=(float)sin(x),cb=(float)cos(y),sb=(float)sin(y);V3 p=vp(a.x+(u.x*ca+v.x*sa)*ra,a.y+(u.y*ca+v.y*sa)*ra,a.z+(u.z*ca+v.z*sa)*ra),q=vp(a.x+(u.x*cb+v.x*sb)*ra,a.y+(u.y*cb+v.y*sb)*ra,a.z+(u.z*cb+v.z*sb)*ra),r=vp(b.x+(u.x*cb+v.x*sb)*rb,b.y+(u.y*cb+v.y*sb)*rb,b.z+(u.z*cb+v.z*sb)*rb),t=vp(b.x+(u.x*ca+v.x*sa)*rb,b.y+(u.y*ca+v.y*sa)*rb,b.z+(u.z*ca+v.z*sa)*rb);quadstrip(p,q,r,t,color);}
}
static void sash_geometry(int id,V3*a,V3*b,V3*c,V3*d){float x,z;windowpos(id,&x,&z);WindowState*w=&WIN(id);float bottom=1.02f,top=2.7f,width=1.65f,angle=w->angle;
 if(w->mode){*a=vp(x-width/2,bottom,z);*b=vp(a->x+width*(float)cos(angle),bottom,z-width*(float)sin(angle));*c=vp(b->x,top,b->z);*d=vp(a->x,top,a->z);}
 else{*a=vp(x-width/2,bottom,z);*b=vp(x+width/2,bottom,z);*c=vp(b->x,bottom+(top-bottom)*(float)cos(angle),z-(top-bottom)*(float)sin(angle));*d=vp(a->x,c->y,c->z);}
}
static V3 handle_position(int id){float x,y,z;window_handle_pos(id,&x,&y,&z);return vp(x,y,z);}
static void window_draw(int id){surfaceTex=0;
 float x,z;windowpos(id,&x,&z);WindowState*w=&WIN(id);int door=0;float width=1.65f,bottom=door?.12f:1.02f,top=2.7f;V3 a,b,c,d;sash_geometry(id,&a,&b,&c,&d);
 U32 trim=id/3==0?0xe1d8bc:(id/3)%3?0xc9c6b6:0x796e59;
 box(x-width/2-.06f,(bottom+top)/2,z,.12f,top-bottom+.2f,.16f,trim);box(x+width/2+.06f,(bottom+top)/2,z,.12f,top-bottom+.2f,.16f,trim);box(x,top+.06f,z,width+.25f,.12f,.16f,trim);box(x,bottom-.05f,z,width+.38f,.12f,.38f,0xa3a59b);
 V3 ai=blend3(blend3(a,d,.06f),blend3(b,c,.06f),.06f),bi=blend3(blend3(a,d,.06f),blend3(b,c,.06f),.94f),ci=blend3(blend3(a,d,.94f),blend3(b,c,.94f),.94f),di=blend3(blend3(a,d,.94f),blend3(b,c,.94f),.06f);
 quadstrip(a,b,bi,ai,trim);quadstrip(b,c,ci,bi,trim);quadstrip(c,d,di,ci,trim);quadstrip(d,a,ai,di,trim);
 quad(ai,bi,ci,di,mix(0x293e48,0x9bb2b0,sunlight*.58f),0,1.1f);V3 e=blend3(ai,di,.58f),f=blend3(bi,ci,.58f),ee=blend3(ai,di,.61f),ff=blend3(bi,ci,.61f);quadstrip(e,f,ff,ee,trim);
 V3 h=handle_position(id);box(h.x,h.y,h.z+.025f,.09f,.20f,.05f,0xa6a18f);float turn=w->handle?1:opening==id?clamp(openProgress/.23f,0,1):0;V3 tip=vp(h.x-.19f*(float)sin(turn*PI*.5f),h.y-.19f*(float)cos(turn*PI*.5f),h.z-.025f);beam(h,tip,.027f,.025f,0xe6d9ba);
 for(int n=0;n<2;n++){V3 hinge=blend3(a,d,n?.87f:.13f);box(hinge.x,hinge.y,hinge.z-.025f,.09f,.16f,.13f,0x827e6d);}
 V3 e1=blend3(ai,bi,.12f),e2=blend3(ai,bi,.14f),e3=blend3(di,ci,.14f),e4=blend3(di,ci,.12f);quad(e1,e2,e3,e4,0x9bb3b3,0,.75f);
}
static float actorOffset190=0,pullY190=1.9f;
static V3 human_point(float x,float z,float a,float xx,float y,float zz){return vp(x+((float)cos(a)*xx+(float)sin(a)*zz)/WORLD_XZ,y+actorOffset190,z+(-(float)sin(a)*xx+(float)cos(a)*zz)/WORLD_XZ);}
/* Cached indexed ellipsoid mesh; original procedural model, no downloaded assets. */
static V3 meshVertices[117];static int meshReady=0;
static void soft_mesh(float x,float y,float z,float rx,float ry,float rz,float angle,U32 color){if(!meshReady){for(int a=0;a<=8;a++)for(int n=0;n<=12;n++){float p=-PI/2+a*PI/8,t=n*PI/6;meshVertices[a*13+n]=vp((float)cos(p)*(float)cos(t),(float)sin(p),(float)cos(p)*(float)sin(t));}meshReady=1;}V3 v[117];float ca=(float)cos(angle),sa=(float)sin(angle);for(int i=0;i<117;i++){V3 p=meshVertices[i];v[i]=vp(x+ca*p.x*rx+sa*p.z*rz,y+p.y*ry,z-sa*p.x*rx+ca*p.z*rz);}for(int a=0;a<8;a++)for(int n=0;n<12;n++){int k=a*13+n;quadstrip(v[k],v[k+1],v[k+14],v[k+13],shade(color,.94f+(n%3)*.025f));}}
static void glow150(V3 a,V3 b,V3 c,V3 d,U32 color);
#include "cigarette201.h"
#include "civilian_rig.h"
#include "room_art.h"
#include "interior_props.h"
static void contact_shadow(float x,float z,float rx,float rz){surfaceTex=0;for(int k=0;k<12;k++){float a=k*PI/6,b=(k+1)*PI/6;quadstrip(vp(x,.018f,z),vp(x+cos(a)*rx,.018f,z+sin(a)*rz),vp(x+cos(b)*rx,.018f,z+sin(b)*rz),vp(x,.018f,z),0x59615e);}}
static void human(float x,float z,int n,float walk,int hero){surfaceTex=0;civilian_draw(x,z,n,walk,hero);}
static V3 view_point(float x,float y,float z){float yy=y*(float)cos(pitch)+z*(float)sin(pitch),zz=-y*(float)sin(pitch)+z*(float)cos(pitch);return vp(camx+((float)cos(yaw)*x+(float)sin(yaw)*zz)/WORLD_XZ,camy+yy,camz+(-(float)sin(yaw)*x+(float)cos(yaw)*zz)/WORLD_XZ);}
static void hands_draw(void){if(screen!=0||help)return;surfaceTex=0;V3 shoulder=view_point(.32f,-.43f,.05f),hand;
 if(opening>=0){hand=handle_position(opening);float reach=clamp(openTimer/.10f,0,1);hand=blend3(view_point(.32f,-.60f,.53f),hand,reach);V3 elbow=blend3(shoulder,hand,.51f);elbow.y-=.16f;elbow.x+=(float)cos(yaw)*.14f;elbow.z-=(float)sin(yaw)*.14f;beam(shoulder,elbow,.12f,.09f,0x71766a);beam(elbow,hand,.09f,.045f,0x888b79);box(hand.x,hand.y,hand.z-.025f,.11f,.10f,.11f,0xc5a68a);}
 else if(itemAnim>0&&(itemKind==4||itemKind==6)){float t=1-clamp(itemAnim/consume_duration(),0,1),rise=clamp(t/.22f,0,1)*clamp((1-t)/.22f,0,1);float px=lerp(.3f,.14f,rise),py=lerp(-.42f,-.16f,rise),pz=lerp(.50f,.38f,rise);hand=view_point(px,py,pz);V3 elbow=view_point(.38f,-.50f,.24f);beam(shoulder,elbow,.10f,.07f,0x53616a);beam(elbow,hand,.07f,.043f,0x65757c);soft_mesh(hand.x,hand.y,hand.z,.042f,.052f,.040f,yaw,0xc2a284);V3 a=view_point(px-.022f,py+.038f,pz+.015f),b=view_point(px-.025f,py+.038f,pz+.099f);cigarette141(a,b,1);for(int k=0;k<2;k++){V3 p=view_point(px-.02f+k*.017f,py+.022f,pz+.018f);beam(p,view_point(px-.02f+k*.017f,py+.043f,pz+.028f),.008f,.007f,0xc4a68a);}}
 else if(itemAnim>0){float t=1-clamp(itemAnim/consume_duration(),0,1),rise=clamp(t/.20f,0,1)*clamp((1-t)/.20f,0,1);rise=rise*rise*(3-2*rise);float tilt=clamp((t-.30f)/.16f,0,1)*clamp((.78f-t)/.14f,0,1)*1.30f;
 float px=lerp(.32f,.07f,rise),py=lerp(-.64f,-.08f,rise),pz=lerp(.63f,.14f,rise);float ax=0,ay=(float)cos(tilt),az=-(float)sin(tilt);V3 cap=view_point(px,py,pz),base=view_point(px-ax*.32f,py-ay*.32f,pz-az*.32f),neck=view_point(px,py-ay*.09f,pz-az*.09f);hand=blend3(base,neck,.35f);V3 elbow=view_point(.36f,-.50f,.32f);beam(shoulder,elbow,.105f,.08f,0x4d5c69);beam(elbow,hand,.08f,.047f,0x667480);soft_mesh(hand.x,hand.y,hand.z,.055f,.075f,.055f,yaw,0xc5a68a);
 if(itemKind==1){box(hand.x,hand.y+.08f,hand.z,.15f,.09f,.16f,0xc8aa74);box(hand.x,hand.y+.055f,hand.z,.16f,.025f,.17f,0x946c60);}else if(itemKind==3){box(hand.x,hand.y+.07f,hand.z,.13f,.12f,.08f,0xd2d8d3);box(hand.x,hand.y+.08f,hand.z+.045f,.08f,.05f,.01f,0x9c665b);}else if(itemKind==4||itemKind==6){V3 end=view_point(px-.03f,py,pz+.105f);cigarette141(cap,end,1);}else{beam(base,neck,.047f,.035f,itemKind==5?0x45684d:0x7b9b9e);beam(neck,cap,.020f,.020f,itemKind==5?0x68815b:0x92b1b2);V3 label=blend3(base,neck,.4f);box(label.x,label.y,label.z+.035f,.078f,.09f,.016f,itemKind==5?0xc3b591:0xd0dcd9);}
 if(t>.12f&&t<.3f){V3 left=view_point(-.13f,-.28f,.39f),le=view_point(-.34f,-.53f,.26f);beam(view_point(-.32f,-.45f,.03f),le,.10f,.07f,0x4d5c69);beam(le,left,.07f,.045f,0x667480);soft_mesh(left.x,left.y,left.z,.055f,.06f,.05f,yaw,0xc5a68a);if(itemKind!=4&&itemKind!=6)box(left.x,left.y+.05f,left.z,.05f,.02f,.05f,0xb9b7aa);}
 }

}
static void domestic_cat(void){if(dst(camx,camz,catX,catZ)>45)return;surfaceTex=0;float a=(float)atan2(catTX-catX,catTZ-catZ),c=(float)cos(a),t=(float)sin(a),b=catState==4?-.07f+ab((float)sin(realTime*7))*.018f:catState==3?(float)sin(realTime*9)*.025f:catState==1?(float)sin(catPhase)*.02f:0;soft_mesh(catX,.27f+b,catZ,.24f,.21f,.36f,a,0xc9948e);float xx=catX+t*.29f,zz=catZ+c*.29f;soft_mesh(xx,.46f+b,zz,.18f,.15f,.16f,a,0xb8b1a2);V3 nose=vp(xx+t*.175f,.435f+b,zz+c*.175f);box(nose.x,nose.y,nose.z,.045f,.03f,.035f,0xc48283);for(int ws=-1;ws<=1;ws+=2){V3 wa=vp(nose.x+c*.05f*ws,nose.y-.02f,nose.z-t*.05f*ws),wb=vp(nose.x+c*.23f*ws,nose.y-.035f,nose.z-t*.23f*ws);beam(wa,wb,.006f,.003f,0xddd5bf);}for(int side=-1;side<=1;side+=2){V3 ear=vp(xx+c*.11f*side,.61f+b,zz-t*.11f*side);quadstrip(vp(ear.x-.065f,.54f+b,ear.z),vp(ear.x+.065f,.54f+b,ear.z),vp(ear.x,.73f+b,ear.z),vp(ear.x,.73f+b,ear.z),0x918d84);V3 eye=vp(xx+c*.07f*side+t*.14f,.49f+b,zz-t*.07f*side+c*.14f);box(eye.x,eye.y,eye.z,.035f,.025f,.025f,0xb6c889);for(int f=-1;f<=1;f+=2){float swing=(catState==1?(float)sin(catPhase+side*f)*.04f:0);beam(vp(catX+c*.11f*side+t*.19f*f,.23f,catZ-t*.11f*side+c*.19f*f),vp(catX+c*.11f*side+t*(.19f*f+swing),.035f,catZ-t*.11f*side+c*(.19f*f+swing)),.04f,.026f,0x8e8c83);}}beam(vp(catX-t*.27f,.31f,catZ-c*.27f),vp(catX-t*.48f,.53f,catZ-c*.48f),.035f,.02f,0x8d8e87);}
static void pet(void){
 float x=g.petx,z=g.petz,bounce=(float)sin(realTime*5)*.025f;
 box(x,.47f+bounce,z,.64f,.55f,.95f,0xc9948e);
 for(int i=0;i<4;i++)box(x+(i%2?.22f:-.22f),.17f,z+(i/2?.3f:-.3f),.14f,.32f,.17f,0x97706a);
 box(x,.68f+bounce,z+.55f,.66f,.55f,.40f,0xb9a68a);
 box(x-.23f,.99f+bounce,z+.56f,.16f,.24f,.15f,0xb7a58b);box(x+.23f,.99f+bounce,z+.56f,.16f,.24f,.15f,0xb7a58b);
 box(x-.16f,.74f+bounce,z+.765f,.12f,.09f,.025f,0x23433c);box(x+.16f,.74f+bounce,z+.765f,.12f,.09f,.025f,0x23433c);
 box(x,.6f+bounce,z+.77f,.09f,.07f,.035f,0xb4747c);
 box(x-.29f,.60f,z+.79f,.24f,.018f,.015f,0xe0d5be);box(x+.29f,.60f,z+.79f,.24f,.018f,.015f,0xe0d5be);
 box(x,.6f,z-.56f,.09f,.13f,.26f,0xc9948e);
}
static void crown(float x,float y,float z,float rx,float ry,float rz,U32 color){
 for(int belt=0;belt<4;belt++){float a=-PI*.5f+belt*PI/4,b=a+PI/4;for(int n=0;n<10;n++){float u=n*PI/5,v=(n+1)*PI/5;V3 p=vp(x+(float)cos(u)*rx*(float)cos(a),y+ry*(float)sin(a),z+(float)sin(u)*rz*(float)cos(a)),q=vp(x+(float)cos(v)*rx*(float)cos(a),p.y,z+(float)sin(v)*rz*(float)cos(a)),r=vp(x+(float)cos(v)*rx*(float)cos(b),y+ry*(float)sin(b),z+(float)sin(v)*rz*(float)cos(b)),s=vp(x+(float)cos(u)*rx*(float)cos(b),r.y,z+(float)sin(u)*rz*(float)cos(b));quadstrip(p,q,r,s,shade(color,.94f+(n%3)*.035f));}}
}
static void tree(float x,float z,float scale,int type){
 box(x,.015f,z,scale*3.4f,.03f,scale*2,0x424e40);
 surfaceTex=&mat_wood;box(x,1.6f*scale,z,.3f*scale,3.2f*scale,.32f*scale,0x635b4c);surfaceTex=0;
 if(winterEnabled&&!type){for(int n=0;n<7;n++){float a=n*2.4f,yy=(1.8f+n*.29f)*scale;V3 root=vp(x,yy,z),tip=vp(x+(float)sin(a)*1.45f*scale,yy+1.2f*scale,z+(float)cos(a)*1.45f*scale);beam(root,tip,.07f*scale,.025f*scale,0x6c706b);beam(tip,vp(tip.x+(float)cos(a)*.45f*scale,tip.y+.65f*scale,tip.z-(float)sin(a)*.45f*scale),.025f*scale,.008f*scale,0x8c918b);}return;}
 if(type){for(int k=0;k<3;k++)crown(x,scale*(2.5f+k*.65f),z,scale*(1.65f-k*.35f),scale*.9f,scale*(1.65f-k*.35f),k%2?0x657d58:0x4e7054);}
 else{crown(x,3.5f*scale,z,1.8f*scale,1.7f*scale,1.5f*scale,g.season==3?0xa28346:g.season==1?0x89a46b:0x839468);crown(x+.65f*scale,4.4f*scale,z-.2f,1.25f*scale,1.1f*scale,1.2f*scale,g.season==3?0xb39a55:0x9caa77);}

}
static const Texture*wallpaper141(int h,int f);static void river_draw141(void);static void courtyards_draw141(void);
#include "update200_draw.h"
#include "update150_draw.h"
#include "neighborhood_draw.h"
#include "panelki_draw.h"
static void apartment_furniture141(int h,float y);static void residence_art140(int h,float y);static void residence_lift_draw140(int h);
#include "residences_draw.h"
#include "quarter_draw.h"
#include "update140_draw.h"
static Texture signMarket;
#include "update141_draw.h"
static void house_draw(int h,int pass){float x=hx[h],z=hz[h];if(dst(camx,camz,x,z)>viewdist+12)return;residence_upper_draw(h);surfaceTex=&mat_concrete;box(x,-.045f,z,10,.10f,9.1f,0x777d80);surfaceTex=0;}
static void store_draw141(void);
static Texture signOffice={sign_office_rgb,1024,192},signKiosk={sign_kiosk_rgb,1024,192},signMarket={sign_market_rgb,1024,192};
#include "recycling_sign.h"
static Texture recycleSign={recycling_sign_rgb,512,128};
static void recycling_draw(void){float x=RECYCLE_X,z=RECYCLE_Z;surfaceTex=&mat_concrete;box(x,.02f,z,9,.08f,5.4f,0xa2aaa2);surfaceTex=&mat_metal;box(x,.56f,z+1.15f,3.3f,1.1f,.75f,0x6b807b);box(x,3.1f,z+1.2f,8.6f,.16f,4,0x637a7b);for(int side=-1;side<=1;side+=2)box(x+side*3.8f,1.5f,z+2.7f,.12f,3,.12f,0x5a6b68);surfaceTex=0;plane(x,2.45f,z+1.4f,3.8f,.95f,0xffffff,&recycleSign);for(int n=0;n<4;n++)box(x-1+n*.18f,1.28f,z+1.1f,.10f,.32f,.10f,0x7b9b7f);for(int k=0;k<2;k++)box(x+2.6f,.1f,z+1.1f+k*.9f,.9f,.16f,.18f,0x827a62);surfaceTex=&mat_rebar;for(int n=0;n<8;n++)beam(vp(x+2.3f+(n%3)*.13f,.21f+(n/3)*.09f,z+.7f),vp(x+2.5f+(n%3)*.13f,.24f+(n/3)*.09f,z+2.6f),.043f,.043f,0x9da398);surfaceTex=0;box(x-2.9f,.38f,z+1.2f,1.35f,.76f,1.4f,0x566f66);for(int n=0;n<5;n++)box(x-3.3f+n*.17f,.85f,z+1.2f,.12f,.35f,.12f,0x8da58b);}
static void commercial_buildings(void){surfaceTex=&mat_brick;
 box(OX-7.95f,1.8f,OZ,.3f,3.6f,11.4f,white);box(OX+7.95f,1.8f,OZ,.3f,3.6f,11.4f,white);box(OX,1.8f,OZ+5.55f,16,3.6f,.3f,white);
 box(OX-4.68f,1.8f,OZ-5.55f,6.6f,3.6f,.3f,white);box(OX+4.68f,1.8f,OZ-5.55f,6.6f,3.6f,.3f,white);box(OX,3.25f,OZ-5.55f,2.8f,.7f,.3f,white);
 surfaceTex=&mat_floor;box(OX,.03f,OZ,16,.06f,11.4f,white);surfaceTex=&mat_roof;box(OX,3.75f,OZ,16.7f,.25f,12,white);surfaceTex=0;
 plane(OX,3.05f,OZ-5.73f,14,1.10f,white,&signOffice);box(OX,.65f,OZ+2.7f,8,1.3f,1.2f,0x776f58);box(OX,1.34f,OZ+2.7f,8.2f,.12f,1.4f,0xb4b29b);box(OX-2,1.6f,OZ+3.1f,1.15f,.55f,.14f,0x343f42);box(OX-2,1.6f,OZ+3.01f,.98f,.39f,.02f,0x92ac9c);human(OX+1,OZ+4,61,0,0);
 for(int i=0;i<3;i++){box(OX-6+i*1.5f,.46f,OZ+.2f,1.15f,.12f,.65f,0x606d62);box(OX-6+i*1.5f,.85f,OZ+.53f,1.15f,.7f,.12f,0x606d62);}
 surfaceTex=&mat_metal;box(KX,1.25f,KZ,4.4f,2.5f,2.5f,0x52695d);surfaceTex=&mat_roof;box(KX,2.6f,KZ,4.8f,.20f,2.9f,0xa7aa93);surfaceTex=0;
 plane(KX,2.20f,KZ-1.27f,4.2f,.63f,white,&signKiosk);box(KX,1.36f,KZ-1.27f,2.3f,.72f,.03f,0x172a24);box(KX,1.03f,KZ-1.45f,2.6f,.10f,.45f,0xa39979);box(KX,1.51f,KZ-1.30f,.21f,.26f,.03f,0xc0ad88);


}
/* Stylized traffic and mixed-use city blocks: procedural geometry, no external assets. */
static void traffic_car(float x,float z,float phase,int color,int heading){
 if(dst(camx,camz,x,z)>92)return;
 float dir=heading?1.f:-1.f,bodyY=.54f;
 U32 paint=(U32)color,glass=0x526b70;
 surfaceTex=&mat_asphalt;box(x,.012f,z,4.25f,.018f,2.18f,0x303536);surfaceTex=0;
 surfaceTex=&mat_metal;
 box(x,bodyY,z,4.45f,.78f,1.98f,paint);
 box(x-dir*.18f,1.12f,z,2.25f,.82f,1.70f,paint);
 surfaceTex=0;
 /* Separate windscreen and side glazing keeps the silhouette readable. */
 box(x+dir*.76f,1.19f,z,0.055f,.52f,1.48f,glass);
 box(x-dir*.78f,1.19f,z,0.055f,.52f,1.48f,glass);
 for(int side=-1;side<=1;side+=2){
  box(x-dir*.12f,1.19f,z+side*.87f,1.22f,.48f,.035f,glass);
  box(x+dir*.92f,.53f,z+side*.96f,.26f,.20f,.13f,0x333b3a);
  for(int axle=-1;axle<=1;axle+=2){float wx=x+axle*1.27f,wz=z+side*.91f;soft_mesh(wx,.34f,wz,.34f,.34f,.16f,0,0x202526);soft_mesh(wx,.34f,wz+side*.09f,.17f,.17f,.055f,0,0x89918d);}
 }
 box(x+dir*2.25f,.50f,z,.08f,.35f,1.45f,0x333b3a);
 for(int side=-1;side<=1;side+=2){box(x+dir*2.27f,.66f,z+side*.59f,.045f,.17f,.34f,heading?0xe5d9a6:0x9e4c43);box(x-dir*2.27f,.62f,z+side*.59f,.045f,.17f,.34f,heading?0x9e4c43:0xe5d9a6);}
 surfaceTex=&mat_metal;box(x,.18f,z,3.25f,.12f,1.72f,0x4d5554);surfaceTex=0;
}
static void vehicle190(float x,float z,int n);
static void traffic_draw(void){static const U32 colors[7]={0x647b72,0x8a5146,0x657487,0xb0a17c,0x454e52,0x9a9b91,0x6f6254};traffic_init140();for(int n=0;n<7;n++)vehicle190(carX140[n],carZ140[n],n);}

static void city_block(float x,float z,int floors,int variant){
 if(dst(camx,camz,x,z)>125)return;
 float height=floors*3.05f;U32 wall=variant%3==0?0xa9a99d:variant%3==1?0x9b9d96:0xb1a38d;
 surfaceTex=&mat_concrete;box(x,height*.5f,z,15.5f,height,10.2f,wall);surfaceTex=&mat_roof;box(x,height+.12f,z,15.9f,.24f,10.6f,variant%2?0x687779:0x777e76);surfaceTex=0;
 /* Recessed vertical bays, lintels and sill bands create a modular facade. */
 for(int f=0;f<floors;f++){
  float y=f*3.05f+1.72f;
  for(int k=0;k<5;k++){
   float wx=x-6.0f+k*3.0f;
   box(wx,y,z-5.13f,1.72f,1.58f,.055f,(k+f+variant)%4?0x526b70:0x7d8277);
   box(wx,y,z-5.19f,1.84f,1.72f,.075f,0xc4c3b4);
   box(wx,y,z-5.24f,1.58f,1.46f,.04f,(k+f+variant)%4?0x526b70:0x7d8277);
   box(wx,y-.84f,z-5.28f,2.0f,.13f,.18f,0xd0c9b2);
   box(wx,y+.84f,z-5.23f,2.0f,.12f,.18f,0xc4c3b4);
   box(wx,y,z-5.31f,.055f,1.48f,.04f,0xbfc3b9);
  }
  box(x,y-1.48f,z-5.22f,15.45f,.09f,.16f,0x777f79);
  /* Side windows repeat on the return facade. */
  for(int k=0;k<3;k++){float wz=z-3.1f+k*3.0f;for(int side=-1;side<=1;side+=2){box(x+side*7.79f,y,wz,.05f,1.55f,1.65f,0x596f70);box(x+side*7.84f,y,wz,.08f,1.72f,1.82f,0xb9b9aa);box(x+side*7.91f,y-.82f,wz, .20f,.12f,1.95f,0xc2c0b1);}}
 }
 /* Ground-floor storefront and entrance. */
 surfaceTex=&mat_brick;box(x,1.45f,z-5.23f,15.45f,2.88f,.14f,variant%2?0x786d5b:0x68776f);surfaceTex=0;
 for(int k=0;k<4;k++){float wx=x-5.7f+k*3.8f;box(wx,1.42f,z-5.34f,2.85f,1.95f,.045f,0x526a6c);box(wx,1.42f,z-5.38f,.08f,1.95f,.05f,0xc7c6b8);}
 box(x+6.2f,1.25f,z-5.38f,1.35f,2.5f,.10f,0x4a5b58);
 box(x,3.12f,z-5.39f,14.8f,.32f,.12f,variant%2?0xc9b58a:0xb5c0b3);
 /* Simple projecting balconies and painted drainpipes break up long flat walls. */
 for(int f=1;f<floors;f++)for(int k=0;k<3;k++){float wx=x-4.8f+k*4.8f,yy=f*3.05f+.55f;box(wx,yy,z-5.65f,2.25f,.12f,.72f,0x8a9188);for(int rail=0;rail<5;rail++)box(wx-1.05f+rail*.525f,yy+.38f,z-5.95f,.035f,.65f,.035f,0x65736f);box(wx,yy+.70f,z-5.95f,2.2f,.06f,.05f,0x65736f);}
 for(int side=-1;side<=1;side+=2){box(x+side*7.91f,height*.5f,z+4.2f,.12f,height-.2f,.12f,0x777e76);for(int f=1;f<floors;f++)box(x+side*7.96f,f*3.05f,z+4.2f,.18f,.12f,.18f,0xb9b6a7);}
 for(int n=0;n<4;n++){float vx=x-5.4f+n*3.6f;box(vx,height+.42f,z+3.2f,.8f,.65f,.8f,0x707a75);}
}
static void city_blocks_draw(void){
 for(int n=0;n<4;n++)city_block(cityBlockX[n],cityBlockZ[n],n<2?3:2,n);
}
/* Original low-noise concrete with a complete mip chain; no new external asset. */
#include "postpunk_materials.h"
static U32 districtConcrete[87381];static int districtMaterialReady=0;
static void district_material_prepare(void){materials_soften();if(districtMaterialReady)return;for(int y=0;y<256;y++)for(int x=0;x<256;x++){unsigned int h=hash32((x/5)+(y/7)*67+773);int coarse=(int)(hash32((x/45)+(y/53)*29)%5)-2,stain=(hash32((x/33)+(y/47)*31)%31<3)?-5:0,crack=0,seam=0;int v=162+(int)(h%5)-2+coarse+stain+crack+seam;districtConcrete[y*256+x]=rgb(v,v+3,v+1);}int offset=0,next=65536;for(int w=256;w>1;w/=2){int nw=w/2;for(int y=0;y<nw;y++)for(int x=0;x<nw;x++){U32 a=districtConcrete[offset+y*2*w+x*2],b=districtConcrete[offset+y*2*w+x*2+1],c=districtConcrete[offset+(y*2+1)*w+x*2],d=districtConcrete[offset+(y*2+1)*w+x*2+1];districtConcrete[next+y*nw+x]=mix(mix(a,b,.5f),mix(c,d,.5f),.5f);}offset=next;next+=nw*nw;}mat_concrete=(Texture){0,512,512,1,art_concrete170_pixels};districtMaterialReady=1;}
static void prepare_room_lights(void){lighting_reset();for(int n=0;n<3;n++)lighting_room(civicX190[n],2.9f,civicZ190[n],3.8f,3.3f,1.05f,n==0?0xd0e3c5:n==1?0xc5d9d1:0xffd9a8);
 if(dst(camx,camz,hx[0]-9.6f,hz[0])<25){lighting_block(hx[0]-12.3f,.55f,hz[0],.6f,.5f,1.8f);lighting_block(hx[0]-9.6f,.42f,hz[0]-.4f,.9f,.4f,.65f);}

 if(dst(camx,camz,hx[0]-9.6f,hz[0])<25)lighting_room(hx[0]-9.6f,2.45f,hz[0],4.5f,4.2f,1.35f,0xffdba2);
 for(int p=0;p<PANEL_COUNT;p++)if(dst(camx,camz,panel_origin_x(p),panel_origin_z(p))<30){panelIndex=p;int f=panel_floor();for(int w=0;w<2;w++)for(int side=-1;side<=1;side+=2){float bx=panel_cx(w),bz=PANEL_Z,by=f*3;lighting_block(bx+side*3.85f,by+1.5f,bz-4.25f,.09f,1.5f,.65f);lighting_block(bx+side*3.85f,by+1.5f,bz-1.75f,.09f,1.5f,.60f);lighting_block(bx+side*2.82f,by+1.5f,bz-1.15f,1.03f,1.5f,.09f);lighting_block(bx+side*2.40f,by+1.5f,bz+2.3f,.45f,1.5f,.09f);lighting_block(bx+side*5.66f,by+1.5f,bz+2.3f,1.01f,1.5f,.09f);if(panel_bath_closed(w,f,side))lighting_block(bx+side*3.85f,by+1.12f,bz-3,.05f,1.12f,.64f);if(flat_lit180(NH+p,f,w*2+(side>0))){U32 lc=flat_color180(NH+p,f,w*2+(side>0));lighting_room(panel_cx(w)+side*5.0f,f*3+2.45f,PANEL_Z-2.0f,1.8f,2.6f,1.25f,lc);lighting_room(panel_cx(w)+side*4.4f,f*3+2.45f,PANEL_Z+5.0f,2.3f,2.7f,1.35f,lc);lighting_room(panel_cx(w)+side*2.85f,f*3+2.45f,PANEL_Z-3.1f,.84f,1.7f,.90f,lc);}} }panelIndex=0;
}

static void snowbank(float x,float z,float rx,float rz,float height){
 if(dst(camx,camz,x,z)>viewdist)return;surfaceTex=0;unsigned int seed=hash32((unsigned int)((x+200)*71+(z+150)*311)+g.worldSeed);float skew=(seed%100)*.01f;V3 top=vp(x+(.1f-skew*.25f)*rx,height,z+(.12f-skew*.17f)*rz);for(int k=0;k<8;k++){float a=k*PI/4+skew,b=(k+1)*PI/4+skew,ra=.75f+(hash32(seed+k)%100)*.004f,rb=.75f+(hash32(seed+(k+1)%8)%100)*.004f;V3 p=vp(x+rx*ra*(float)cos(a),-.045f,z+rz*ra*(float)sin(a)),q=vp(x+rx*rb*(float)cos(b),-.045f,z+rz*rb*(float)sin(b)),u=vp(x+.50f*rx*(float)cos(a),height*.72f,z+.50f*rz*(float)sin(a)),v=vp(x+.50f*rx*(float)cos(b),height*.72f,z+.50f*rz*(float)sin(b));quadstrip(p,q,v,u,k%2?0xe3e8ed:0xc9d5df);quad(top,u,v,top,0xe9edf0,0,1);}}
static void snowbanks_draw(void){if(!winterEnabled)return;for(int n=0;n<18;n++){float x=-82+n*9.5f;for(int side=-1;side<=1;side+=2)snowbank(x+(world_value(2000+n*2+side)-.5f)*3,side*(9.1f+world_value(2050+n)*1.8f),1.6f+world_value(2100+n)*3,.65f+world_value(2200+n)*.9f,.18f+world_value(2300+n)*.6f);}for(int n=0;n<24;n++){int h=n;float x=hx[h]-6.1f,z=hz[h]+2.8f;if(h==0)continue;snowbank(x,z,.85f,2.3f,.5f);}}
#include "district190_draw.h"
#include "update150_props_draw.h"
#include "update160_draw.h"
#include "architecture170.h"
#include "district180_draw.h"
#include "street205.h"
static void bodies_draw200(void);static void window_lights200(void);
static void world(int pass){district_material_prepare();roof_material141();
 surfaceTex=0;worldclock=g.hours;float day=clamp((float)sin((g.hours-6)/24*2*PI)*.8f+.35f,.12f,1);
 camera();clear_scene(day);prepare_room_lights();rooms_light160();high_lights180();window_lights200();
 /* Ground subdivided so fog and depth stay stable near the horizon. */
 for(int h=0;h<NH;h++)house_draw(h,pass);
 surfaceTex=&mat_asphalt;box(0,-.025f,-106,384,.05f,8,white);box(0,-.025f,106,384,.05f,8,white);box(-150,-.025f,0,8,.05f,288,white);box(150,-.025f,0,8,.05f,288,white);surfaceTex=0;
 snowbanks_draw();
 /* Street furniture. */
 for(int n=0;n<envCount;n++)if(env[n].kind==0)tree(env[n].x,env[n].z,.75f+(env[n].id%3)*.12f,env[n].id%2);

 for(int n=0;n<8;n++){float x=-50+n*14.5f,z=n%2?-6.4f:6.4f;box(x,2.6f,z,.12f,5.2f,.12f,0x58635f);box(x+.5f,5.2f,z,1.2f,.12f,.2f,0x58635f);box(x+1,5.12f,z,.4f,.12f,.35f,day<.5f?0xefc97e:0xc1c4b3);}
 for(int n=0;n<4;n++){float x=-18+n*12,z=10;box(x,.37f,z,2.8f,.16f,.65f,0x8a7051);box(x,.82f,z+.37f,2.8f,.75f,.12f,0x8a7051);box(x-1,.185f,z,.12f,.37f,.7f,0x485951);box(x+1,.185f,z,.12f,.37f,.7f,0x485951);}
 /* Fixed, art-directed POIs: a Soviet bus stop and a small courtyard playground. */
 surfaceTex=&mat_metal;box(-69,.08f,11,6.8f,.16f,2.1f,0x555e54);box(-69,2.35f,12.0f,6.8f,.14f,.12f,0x626b60);box(-72.2f,1.23f,12,.10f,2.3f,.12f,0x606c65);box(-65.8f,1.23f,12,.10f,2.3f,.12f,0x606c65);box(-69,1.16f,11.96f,3.8f,.08f,.10f,0x7a7a64);for(int side=-1;side<=1;side+=2)box(-69+side*1.45f,.70f,11.9f,.08f,.9f,.08f,0x555e54);surfaceTex=0;
 box(-69,1.82f,12.04f,1.7f,.42f,.035f,0x435754);box(-69,2.12f,12.07f,1.74f,.08f,.035f,0xb18b56);
 /* Controlled debris clusters and curb drains; positions are stable between sessions. */
 for(int n=0;n<8;n++){float dx=-46+n*13.2f,dz=n%2?8.8f:-8.8f;box(dx,.035f,dz,.44f,.045f,.25f,n%2?0x77796b:0x8a806c);box(dx+.34f,.12f,dz+.14f,.16f,.22f,.16f,0x6d785c);box(dx-.24f,.08f,dz-.12f,.18f,.14f,.12f,0x8c846d);}
 store_draw141();commercial_buildings();recycling_draw();
 /* Parked vehicle. */
 box(-44,.55f,-10,2.1f,.8f,4.2f,0x737f72);box(-44,1.25f,-10.3f,1.9f,.7f,2.2f,0x737f72);box(-44,1.33f,-11.42f,1.6f,.48f,.025f,0x486168);
 for(int n=0;n<4;n++)box(-44+(n%2?1.1f:-1.1f),.35f,-10+(n/2?1.35f:-1.35f),.22f,.6f,.6f,0x303b3b);
 for(int i=0;i<NN;i++)if(dst(camx,camz,PERSON(i).x,PERSON(i).z)<viewdist)human(lerp(npcPrevX[i],PERSON(i).x,visualAlpha),lerp(npcPrevZ[i],PERSON(i).z,visualAlpha),i,walkPhase[i],0);

 district_draw160();district_plan180();high_draw180();civic_draw190();winter_details190();locals_draw190();props_draw150();street_draw205();snow_fall205();panel_draw();home_radio_draw();domestic_cat();neighborhood_details(day);quarter_details();update140_draw();
 for(int n=0;n<LOOT_COUNT;n++){float x,z;loot_position(n,&x,&z);if(dst(camx,camz,x,z)<45){surfaceTex=&mat_concrete;box(x+.55f,.025f,z,2.0f,.05f,1.8f,0x8a8c82);surfaceTex=&mat_metal;box(x+.55f,.57f,z,1.25f,1.02f,1.05f,n%3?0x426456:0x75664c);surfaceTex=0;box(x+.55f,1.11f,z,1.34f,.11f,1.15f,0x727b67);box(x+.55f,1.19f,z-.15f,.32f,.065f,.10f,0x383f36);for(int s=-1;s<=1;s+=2){box(x+.55f+s*.62f,.59f,z,.08f,.12f,.45f,0x9a9982);for(int t=-1;t<=1;t+=2)box(x+.55f+s*.48f,.11f,z+t*.37f,.16f,.20f,.16f,0x323c37);}if(winterEnabled)box(x+.55f,1.18f,z+.1f,1.25f,.035f,.77f,0xd5dcd5);soft_mesh(x-.35f,.20f,z+.35f,.23f,.25f,.22f,0,0x343e37);}}

 surfaceTex=&mat_asphalt;box(0,-.025f,0,264,.05f,12,0x565e5d);box(0,-.025f,-43,254,.05f,8,0x565e5d);box(0,-.025f,43,254,.05f,8,0x565e5d);paved_box202(-94,-.025f,0,8,.05f,174,0x565e5d);paved_box202(94,-.025f,0,8,.05f,174,0x565e5d);
 surfaceTex=&mat_pavement;box(0,0,-7.6f,264,.06f,2.8f,0x9b9c89);box(0,0,7.6f,264,.06f,2.8f,0x9b9c89);surfaceTex=0;
 for(int x=-126;x<128;x+=7)box(x,.012f,0,2.8f,.025f,.12f,0xb8b198);
 ground_draw141();window_ground200();

 surfaceTex=0;
 traffic_draw();
 /* World-space navigation beacon: no forced teleport, walk to the call. */
 if(0&&!pass&&screen==0&&hudMode==0&&showHints){Job*q=&g.jobs[g.activeJob];float tx=navOffice?OX:hx[q->house],tz=navOffice?OZ:hz[q->house];PV p=transform(tx,4.8f,tz,0,0);if(p.z>1){int xx=(int)(W*.5f+p.x*focal/p.z),yy=(int)(H*.5f-p.y*focal/p.z);if(xx>W*.12f&&xx<W*.73f&&yy>H*.16f&&yy<H*.82f){rect(xx-4,yy-4,8,8,accent);char b[64];SN(b,sizeof(b),"%s · %d м",navOffice?"Офис":names[q->house],(int)dst(g.x,g.z,tx,tz));text(xx-textwidth(b,.42f*UI)/2,yy-21,b,.42f*UI,white);}}}
}
static void update_mirror(void){} /* No reflection pass or mirror geometry. */
static void weather_overlay(void){
 int indoor170=high_at180(g.x,g.z)>=0||(home_annex(g.x,g.z)&&g.playerY<2.8f)||residence_at(g.x,g.z)>=0||panel_wing(g.x,g.z)>=0;
 for(int n=0;n<3;n++)if(ab(g.x-civicX190[n])<4&&ab(g.z-civicZ190[n])<3.5f)indoor170=1;
 if((g.weather||winterEnabled)&&!indoor170){int rain=!winterEnabled;int count=g.focusMode?90:260;for(int n=0;n<count;n++){
 unsigned int h=hash32(n*371u+g.worldSeed);float wx=camx-24+(h%4800)*.01f,wz=camz-24+((h>>12)%4800)*.01f;
 float wy=20-(float)fmod(realTime*(rain?15:1.5f+(h%12)*.1f)+(h%200)*.1f,20.f);
 if(winterEnabled)wx+=(float)sin(realTime*.6f+n)*.35f;
 float roof;int sheltered=0;float py=g.playerY;g.playerY=0;
 if(roof_at(wx,wz,&roof)&&wy<roof)sheltered=1;
 if(ab(wx-shopX)<5.5f&&ab(wz-shopZ)<6&&wy<3.5f)sheltered=1;
 if(ab(wx-OX)<8&&ab(wz-OZ)<5.7f&&wy<4)sheltered=1;
 g.playerY=py;if(sheltered)continue;
 PV p=transform(wx,wy,wz,0,0);if(p.z<.3f)continue;int xx=W/2+p.x*focal/p.z,yy=H/2-p.y*focal/p.z;
 if(xx<1||xx>=W-2||yy<1||yy>=H-12)continue;
 /* Software z-buffer and GPU-compatible world projection: outside flakes stay visible through glass. */
 if(!weather_visible140(wx,wy,wz))continue;
 if(!gpuActive&&1/p.z<depth[yy*W+xx]-.002f)continue;
 if(rain)panel(xx,yy,1,6,0x9bb8c4,.55f);else panel(xx,yy,2,2,0xe2e9e3,.72f);
 }}

 if(g.alcohol>5&&screen==0)panel(0,0,W,H,0x967853,g.alcohol*.001f);
 /* Lightweight stress grading for the legacy renderer: tint + stronger vignette, no expensive blur. */
 float stress=screen==0?clamp(g.stress/100.f,0,1):0;float pulse=(g.focusMode||g.uiMotion==0)?1.f:.88f+.12f*(float)sin(realTime*2.1f);if(stress>.02f){float a=stress*.075f*pulse;panel(0,0,W,H,0x6b6047,a);for(int i=0;i<24;i++){float edge=(24-i)/24.f;float v=stress*.25f*edge*pulse;panel(i,0,1,H,0x171c17,v);panel(W-1-i,0,1,H,0x171c17,v);}panel(0,0,W,2,0x8a6845,stress*.10f);}
 /* Restrained ambient vignette. */
 for(int i=0;i<14;i++){panel(i,0,1,H,0x171c17,(14-i)*(.018f+stress*.004f));panel(W-1-i,0,1,H,0x171c17,(14-i)*(.018f+stress*.004f));}
}
