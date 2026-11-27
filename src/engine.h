#include "platform.h"
#include "../assets.h"
#include "../materials.h"
typedef unsigned int U32;
typedef struct {float x,y,z;} V3;
typedef struct {float x,y,z,u,v,lr,lg,lb;} PV;
typedef struct {const unsigned char*rgb;int w,h;int repeat;const U32*packed;} Texture;
#define WORLD_XZ 1.35f /* One legacy horizontal coordinate = 1.35 metres. */
static const Texture *surfaceTex=0;
static Texture mat_brick={0,256,256,1,px_brick};
static Texture mat_redbrick={0,256,256,1,px_redbrick};
static Texture mat_plaster={0,256,256,1,px_plaster};
static Texture mat_paint={0,256,256,1,px_paint};
static Texture mat_roof={0,256,256,1,px_roof};
static Texture mat_wood={0,256,256,1,px_wood};
static Texture mat_floor={0,256,256,1,px_floor};
static Texture mat_asphalt={0,256,256,1,px_asphalt};
static Texture mat_grass={0,256,256,1,px_grass};
static Texture mat_pavement={0,256,256,1,px_pavement};
static Texture mat_concrete={0,256,256,1,px_concrete};
static Texture mat_metal={0,256,256,1,px_metal};

static int seasonVisual140=0;static int winterEnabled=0,seasonReady=0;static float overlayAlpha=1;
static U32 snowPixels[4][87381];static Texture snowTextures[4];
static void season_prepare(void);
#define PI 3.14159265358979323846f
#define MAXW 1920
#define MAXH 1080
static U32 frame[MAXW*MAXH];static float depth[MAXW*MAXH];
static int W=1920,H=1080,draws=0,quality=4;
static int postAA=0;
static int antialias=1,detail=1,frameLimit=60,showHints=1,skillAssist=1,autoChecks=0;
static float hudScale=.84f;
static const int renderWidths[]={800,960,1280,1600,1920};
static void set_resolution(int q){quality=q<0?0:q>4?4:q;W=renderWidths[quality];H=W*9/16;}
static float camx=0,camy=1.7f,camz=0,yaw=0,pitch=0,cy=1,sy=0,cp=1,sp=0,focal=540,field=80;
static float sunlight=1,viewdist=90;
static U32 fogcolor=0x858b88;
static unsigned int seed=57291;
static float rnd(void){seed=seed*1664525u+1013904223u;return (seed>>8)*(1.f/16777216.f);}
static float ab(float a){return a<0?-a:a;}
static float mn(float a,float b){return a<b?a:b;}
static float mx(float a,float b){return a>b?a:b;}
static float clamp(float a,float l,float h){return a<l?l:a>h?h:a;}
static float lerp(float a,float b,float t){return a+(b-a)*t;}
static float dst(float x,float z,float a,float b){float dx=x-a,dz=z-b;return (float)sqrt(dx*dx+dz*dz);}
static U32 rgb(int r,int g,int b){return ((U32)r<<16)|((U32)g<<8)|(U32)b;}
static U32 mix(U32 a,U32 b,float t){t=clamp(t,0,1);return rgb((int)lerp((a>>16)&255,(b>>16)&255,t),(int)lerp((a>>8)&255,(b>>8)&255,t),(int)lerp(a&255,b&255,t));}
static U32 shade(U32 a,float t){return rgb((int)clamp(((a>>16)&255)*t,0,255),(int)clamp(((a>>8)&255)*t,0,255),(int)clamp((a&255)*t,0,255));}
static void season_prepare(void){if(seasonReady)return;const Texture*base[]={&mat_grass,&mat_asphalt,&mat_pavement,&mat_roof};for(int n=0;n<4;n++){for(int i=0;i<87381;i++)snowPixels[n][i]=mix(base[n]->packed[i],n==1?0xc1cbd0:0xe0e6e3,n==1?.58f:.87f);snowTextures[n]=(Texture){0,256,256,1,snowPixels[n]};}seasonReady=1;}
static int utf8(const char**str);
#include "gpu.h"
static void rect(int x,int y,int w,int h,U32 c){if(gpuActive){gpu_rect(x,y,w,h,c,1);return;}int x0=(int)mx(x,0),y0=(int)mx(y,0),x1=(int)mn(x+w,W),y1=(int)mn(y+h,H);for(int j=y0;j<y1;j++)for(int i=x0;i<x1;i++)frame[j*W+i]=overlayAlpha>.999f?c:mix(frame[j*W+i],c,overlayAlpha);}
static void panel(int x,int y,int w,int h,U32 c,float alpha){if(gpuActive){gpu_rect(x,y,w,h,c,alpha);return;}int x0=(int)mx(x,0),y0=(int)mx(y,0),x1=(int)mn(x+w,W),y1=(int)mn(y+h,H);for(int j=y0;j<y1;j++)for(int i=x0;i<x1;i++)frame[j*W+i]=mix(frame[j*W+i],c,alpha*overlayAlpha);}
static int utf8(const char**str){const unsigned char*s=(const unsigned char*)*str;int c=*s++;if(c>=240){c=((c&7)<<18)|((s[0]&63)<<12)|((s[1]&63)<<6)|(s[2]&63);s+=3;}else if(c>=224){c=((c&15)<<12)|((s[0]&63)<<6)|(s[1]&63);s+=2;}else if(c>=192){c=((c&31)<<6)|(s[0]&63);s++;}*str=(const char*)s;return c;}
static int textwidth(const char*s,float sc){int w=0;while(*s){int c=utf8(&s);for(int n=0;n<(int)(sizeof(glyphs)/sizeof(glyphs[0]));n++)if(glyphs[n][0]==c){w+=(int)(glyphs[n][3]*sc);break;}}return w;}
static int font_sample(float x,float y){float fx=x*2,fy=y*2;int ix=(int)fx,iy=(int)fy;float tx=fx-ix,ty=fy-iy;ix=(int)clamp(ix,0,2046);iy=(int)clamp(iy,0,510);const unsigned char*p=font_pixels+iy*2048+ix;return (int)lerp(lerp(p[0],p[1],tx),lerp(p[2048],p[2049],tx),ty);}
static void text(int x,int y,const char*s,float sc,U32 color){if(gpuActive){gpu_text(x,y,s,sc,color);return;}float pen=x;int start=x;while(*s){int c=utf8(&s);if(c==10){pen=start;y+=(int)(34*sc);continue;}for(int n=0;n<(int)(sizeof(glyphs)/sizeof(glyphs[0]));n++)if(glyphs[n][0]==c){int gx=glyphs[n][1],gy=glyphs[n][2],adv=glyphs[n][3],hh=(int)(34*sc),ww=(int)((adv+1)*sc);for(int yy=0;yy<hh;yy++){int py=y+yy;if(py<0||py>=H)continue;for(int xx=0;xx<ww;xx++){int px=(int)pen+xx;if(px<0||px>=W)continue;int a=font_sample(gx+(xx+.5f)/sc,gy+(yy+.5f)/sc);if(a)frame[py*W+px]=mix(frame[py*W+px],color,a/255.f*overlayAlpha);}}pen+=adv*sc;break;}}}
static void center(int y,const char*s,float sc,U32 col){text((W-textwidth(s,sc))/2,y,s,sc,col);}
static void line2(int x0,int y0,int x1,int y1,U32 c){if(gpuActive){gpu_line(x0,y0,x1,y1,c);return;}int dx=x1-x0,dy=y1-y0,n=(int)mx(ab(dx),ab(dy));if(!n)return;for(int i=0;i<=n;i++){int x=x0+dx*i/n,y=y0+dy*i/n;if(x>=0&&y>=0&&x<W&&y<H)frame[y*W+x]=c;}}
static PV transform(float x,float y,float z,float u,float v){float a=(x-camx)*WORLD_XZ,b=y-camy,c=(z-camz)*WORLD_XZ;float xx=a*cy-c*sy,zz=a*sy+c*cy;PV p={xx,b*cp-zz*sp,b*sp+zz*cp,u,v};return p;}
/* Scanline rasterizer: walk covered spans, not the full triangle bounding box. */
static U32 blend_fast(U32 a,U32 b,int t){return ((((a&0xff00ff)*(256-t)+(b&0xff00ff)*t)>>8)&0xff00ff)|((((a&0x00ff00)*(256-t)+(b&0x00ff00)*t)>>8)&0x00ff00);}
static void raster(PV a,PV b,PV c,U32 col,const Texture*tex,float light){
 if(a.z<.09f||b.z<.09f||c.z<.09f)return;
 float iz[3]={1/a.z,1/b.z,1/c.z};
 float x[3]={W*.5f+a.x*focal*iz[0],W*.5f+b.x*focal*iz[1],W*.5f+c.x*focal*iz[2]};
 float y[3]={H*.5f-a.y*focal*iz[0],H*.5f-b.y*focal*iz[1],H*.5f-c.y*focal*iz[2]};
 float den=(y[1]-y[2])*(x[0]-x[2])+(x[2]-x[1])*(y[0]-y[2]);if(ab(den)<.12f)return;
 int y0=(int)mx(0,-floor(-(mn(y[0],mn(y[1],y[2]))-.5f))),y1=(int)mn(H-1,-floor(-(mx(y[0],mx(y[1],y[2]))-.5f))-1);if(y1<y0)return;
 float inv=1/den,dx0=(y[1]-y[2])*inv,dx1=(y[2]-y[0])*inv,dy0=(x[2]-x[1])*inv,dy1=(x[0]-x[2])*inv;
 float dzdx=dx0*(iz[0]-iz[2])+dx1*(iz[1]-iz[2]),dzdy=dy0*(iz[0]-iz[2])+dy1*(iz[1]-iz[2]);
 float origin=iz[2]-dzdx*x[2]-dzdy*y[2];
 int smooth=(a.lr+a.lg+a.lb)>0&&(b.lr+b.lg+b.lb)>0&&(c.lr+c.lg+c.lb)>0;
 float rdx=dx0*(a.lr-c.lr)+dx1*(b.lr-c.lr),rdy=dy0*(a.lr-c.lr)+dy1*(b.lr-c.lr),rd0=c.lr-rdx*x[2]-rdy*y[2];
 float gdx=dx0*(a.lg-c.lg)+dx1*(b.lg-c.lg),gdy=dy0*(a.lg-c.lg)+dy1*(b.lg-c.lg),gd0=c.lg-gdx*x[2]-gdy*y[2];
 float bdx=dx0*(a.lb-c.lb)+dx1*(b.lb-c.lb),bdy=dy0*(a.lb-c.lb)+dy1*(b.lb-c.lb),bd0=c.lb-bdx*x[2]-bdy*y[2];
 float fog=clamp(((a.z+b.z+c.z)/3-22)/(viewdist*1.4f-22),0,.90f);U32 solid=mix(shade(col,light),fogcolor,fog);
 float u[3]={a.u*iz[0],b.u*iz[1],c.u*iz[2]},v[3]={a.v*iz[0],b.v*iz[1],c.v*iz[2]};
 float dudx=dx0*(u[0]-u[2])+dx1*(u[1]-u[2]),dudy=dy0*(u[0]-u[2])+dy1*(u[1]-u[2]),dvdx=dx0*(v[0]-v[2])+dx1*(v[1]-v[2]),dvdy=dy0*(v[0]-v[2])+dy1*(v[1]-v[2]);
 float uorigin=u[2]-dudx*x[2]-dudy*y[2],vorigin=v[2]-dvdx*x[2]-dvdy*y[2];
 int tw=tex?tex->w:0,th=tex?tex->h:0;const U32*packed=tex?tex->packed:0;
 if(packed){float meanz=(a.z+b.z+c.z)/3;float fp=mx(mx(ab(dudx),ab(dudy)),mx(ab(dvdx),ab(dvdy)))*meanz*tw;
 while(fp>1.65f&&tw>2){packed+=tw*th;tw/=2;th/=2;fp*=.5f;}}
 int lit=(int)clamp(light*256,0,320),fogbyte=(int)(fog*256);
 for(int yy=y0;yy<=y1;yy++){
  float yc=yy+.5f,left=1e20f,right=-1e20f;
  for(int e=0;e<3;e++){int f=(e+1)%3;if((y[e]<=yc&&y[f]>yc)||(y[f]<=yc&&y[e]>yc)){int lo=y[e]<y[f]?e:f,hi=y[e]<y[f]?f:e;float xx=x[lo]+(yc-y[lo])*(x[hi]-x[lo])/(y[hi]-y[lo]);left=mn(left,xx);right=mx(right,xx);}}
  if(left>right)continue;int l=(int)mx(0,-floor(-(left-.5f))),r=(int)mn(W-1,-floor(-(right-.5f))-1);if(l>r)continue;
  float z=origin+dzdx*(l+.5f)+dzdy*yc;int at=yy*W+l;float rr=rd0+rdx*(l+.5f)+rdy*yc,gg=gd0+gdx*(l+.5f)+gdy*yc,bb=bd0+bdx*(l+.5f)+bdy*yc;
  if(!tex){for(int xx=l;xx<=r;xx++,at++,z+=dzdx,rr+=rdx,gg+=gdx,bb+=bdx){if(z>depth[at]){frame[at]=smooth?blend_fast(rgb((int)clamp(rr,0,255),(int)clamp(gg,0,255),(int)clamp(bb,0,255)),fogcolor,fogbyte):solid;depth[at]=z;}}}
  else if(packed){
   float uz=uorigin+dudx*(l+.5f)+dudy*yc,vz=vorigin+dvdx*(l+.5f)+dvdy*yc;int mask=tw-1;float mul=tw*65536.f;
   for(int leftx=l;leftx<=r;){int count=(int)mn(8,r-leftx+1);float iz0=1/z,iz1=1/(z+dzdx*count);int iu=(int)(uz*iz0*mul),iv=(int)(vz*iz0*mul),eu=(int)((uz+dudx*count)*iz1*mul),ev=(int)((vz+dvdx*count)*iz1*mul);int du=(eu-iu)/count,dv=(ev-iv)/count;
    for(int j=0;j<count;j++,at++,z+=dzdx,iu+=du,iv+=dv,rr+=rdx,gg+=gdx,bb+=bdx){if(z<=depth[at])continue;int ix=(iu>>16)&mask,iy=(iv>>16)&mask;U32 color=packed[iy*tw+ix];
     if(antialias){int tx=(iu>>8)&255,ty=(iv>>8)&255,xx1=(ix+1)&mask,yy1=(iy+1)&mask;color=blend_fast(blend_fast(color,packed[iy*tw+xx1],tx),blend_fast(packed[yy1*tw+ix],packed[yy1*tw+xx1],tx),ty);}
     int cr=(((color>>16)&255)*lit>>8)*((col>>16)&255)/255,cg=(((color>>8)&255)*lit>>8)*((col>>8)&255)/255,cb=((color&255)*lit>>8)*(col&255)/255;if(smooth){cr=(int)(((color>>16)&255)*clamp(rr,0,255)/255);cg=(int)(((color>>8)&255)*clamp(gg,0,255)/255);cb=(int)((color&255)*clamp(bb,0,255)/255);}color=rgb(cr>255?255:cr,cg>255?255:cg,cb>255?255:cb);frame[at]=blend_fast(color,fogcolor,fogbyte);depth[at]=z;
    }uz+=dudx*count;vz+=dvdx*count;leftx+=count;
   }
  }else{
   float uz=uorigin+dudx*(l+.5f)+dudy*yc,vz=vorigin+dvdx*(l+.5f)+dvdy*yc;
   for(int xx=l;xx<=r;xx++,at++,z+=dzdx,uz+=dudx,vz+=dvdx){if(z<=depth[at])continue;float zz=1/z;float uf=clamp(uz*zz*(tw-1),0,tw-1.001f),vf=clamp(vz*zz*(th-1),0,th-1.001f);int ui=(int)uf,vi=(int)vf;const unsigned char*p=tex->rgb+(vi*tw+ui)*3;U32 color=rgb(p[0],p[1],p[2]);
    if(antialias){float tx=uf-ui,ty=vf-vi;const unsigned char*pp=p+tw*3;color=mix(mix(color,rgb(p[3],p[4],p[5]),tx),mix(rgb(pp[0],pp[1],pp[2]),rgb(pp[3],pp[4],pp[5]),tx),ty);}color=rgb(((color>>16)&255)*((col>>16)&255)/255,((color>>8)&255)*((col>>8)&255)/255,(color&255)*(col&255)/255);frame[at]=mix(shade(color,light),fogcolor,fog);depth[at]=z;
   }
  }
 }draws++;
}
static PV plerp(PV a,PV b,float t){PV c={lerp(a.x,b.x,t),lerp(a.y,b.y,t),lerp(a.z,b.z,t),lerp(a.u,b.u,t),lerp(a.v,b.v,t),lerp(a.lr,b.lr,t),lerp(a.lg,b.lg,t),lerp(a.lb,b.lb,t)};return c;}
static void tri(PV a,PV b,PV c,U32 col,const Texture*t,float light){PV in[5]={a,b,c},out[5];int n=0;for(int i=0;i<3;i++){PV p=in[i],q=in[(i+1)%3];int ip=p.z>=.1f,iq=q.z>=.1f;if(ip)out[n++]=p;if(ip!=iq)out[n++]=plerp(p,q,(.1f-p.z)/(q.z-p.z));}for(int i=1;i<n-1;i++)raster(out[0],out[i],out[i+1],col,t,light);}
/* World-space lighting shared by OpenGL and software paths. Not a shadow-map renderer. */
typedef struct{float x,y,z,rx,rz,floor,power;U32 color;} RoomLight;
typedef struct{float minx,miny,minz,maxx,maxy,maxz;} LightBlock;
static LightBlock lightBlocks[192];static int lightBlockCount=0;
static void lighting_block(float x,float y,float z,float rx,float ry,float rz){if(lightBlockCount<192)lightBlocks[lightBlockCount++]=(LightBlock){x-rx,y-ry,z-rz,x+rx,y+ry,z+rz};}
static int light_blocked(V3 from,V3 to){for(int i=0;i<lightBlockCount;i++){LightBlock*b=&lightBlocks[i];float lo=.025f,hi=.975f;float f[3]={from.x,from.y,from.z},t[3]={to.x,to.y,to.z},mnv[3]={b->minx,b->miny,b->minz},mxv[3]={b->maxx,b->maxy,b->maxz};int hit=1;for(int k=0;k<3;k++){float d=t[k]-f[k];if(ab(d)<.00001f){if(f[k]<mnv[k]||f[k]>mxv[k]){hit=0;break;}}else{float a=(mnv[k]-f[k])/d,c=(mxv[k]-f[k])/d;lo=mx(lo,mn(a,c));hi=mn(hi,mx(a,c));if(hi<lo){hit=0;break;}}}if(hit)return 1;}return 0;}
static RoomLight sceneLights[128];static int sceneLightCount=0;
typedef struct{float x,y,z,radius,power,direction;U32 color;} WindowLight200;
static WindowLight200 windowLights200[128];static int windowLightCount200=0;
static void lighting_window200(float x,float y,float z,float radius,float power,float direction,U32 color){if(windowLightCount200<128)windowLights200[windowLightCount200++]=(WindowLight200){x,y,z,radius,power,direction,color};}
static void lighting_reset(void){sceneLightCount=0;lightBlockCount=0;windowLightCount200=0;}
static void lighting_room(float x,float y,float z,float rx,float rz,float power,U32 color){if(sceneLightCount>=128)return;sceneLights[sceneLightCount++]=(RoomLight){x,y,z,rx,rz,y-2.45f,power,color};}
static U32 color_multiply(U32 a,U32 b){return rgb(((a>>16)&255)*((b>>16)&255)/255,((a>>8)&255)*((b>>8)&255)/255,(a&255)*(b&255)/255);}
static float lighting_at(V3 pos,V3 normal,U32*tint){float len=(float)sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);if(len<.0001f)len=1;normal.x/=len;normal.y/=len;normal.z/=len;
 /* Geometry is two-sided. Orient the geometric normal toward the visible side. */
 if(normal.x*(camx-pos.x)+normal.y*(camy-pos.y)+normal.z*(camz-pos.z)<0){normal.x=-normal.x;normal.y=-normal.y;normal.z=-normal.z;}
 float direct=mx(0,normal.x*-.42f+normal.y*.78f+normal.z*-.45f);float light=.19f+sunlight*(.29f+.48f*direct);*tint=mix(0xb9c8e2,0xf0f0e5,sunlight);
 for(int i=0;i<sceneLightCount;i++){RoomLight*l=&sceneLights[i];if(pos.y<l->floor+.02f||pos.y>l->floor+2.99f||ab(pos.x-l->x)>l->rx||ab(pos.z-l->z)>l->rz)continue;
 float dx=l->x-pos.x,dy=l->y-pos.y,dz=l->z-pos.z,d2=dx*dx+dy*dy+dz*dz,inv=1/(float)sqrt(mx(.03f,d2));float incidence=.25f+.75f*mx(0,(normal.x*dx+normal.y*dy+normal.z*dz)*inv);float point=l->power*incidence/(1+d2*.20f);if(point>.04f&&light_blocked((V3){l->x,l->y,l->z},pos))point*=.10f;light=mx(light,.30f+sunlight*.13f+point);*tint=mix(*tint,l->color,clamp(point*.85f,0,.72f));}
 for(int i=0;i<windowLightCount200;i++){WindowLight200*l=&windowLights200[i];float dx=l->x-pos.x,dy=l->y-pos.y,dz=l->z-pos.z;if(-dz*l->direction<-.25f||ab(dx)>l->radius||ab(dz)>l->radius||ab(dy)>l->radius)continue;float d2=dx*dx+dy*dy+dz*dz;if(d2>l->radius*l->radius)continue;float inv=1/(float)sqrt(mx(.04f,d2));float incidence=.18f+.82f*mx(0,(normal.x*dx+normal.y*dy+normal.z*dz)*inv);float edge=1-d2/(l->radius*l->radius);float point=l->power*incidence*edge*edge/(1+.18f*d2);light=mx(light,.20f+sunlight*.1f+point);*tint=mix(*tint,l->color,clamp(point*.6f,0,.8f));}
 return clamp(light,.14f,1.25f);}

static int lightingSplit=0;
static V3 light_lerp3(V3 a,V3 b,float t){V3 r={lerp(a.x,b.x,t),lerp(a.y,b.y,t),lerp(a.z,b.z,t)};return r;}
static void quad(V3 a,V3 b,V3 c,V3 d,U32 col,const Texture*t,float light){
 {float lx=mn(mn(a.x,b.x),mn(c.x,d.x)),hx=mx(mx(a.x,b.x),mx(c.x,d.x)),ly=mn(mn(a.y,b.y),mn(c.y,d.y)),hy=mx(mx(a.y,b.y),mx(c.y,d.y)),lz=mn(mn(a.z,b.z),mn(c.z,d.z)),hz=mx(mx(a.z,b.z),mx(c.z,d.z));
 float radius=((hx-lx+hz-lz)*WORLD_XZ+hy-ly)*.5f;PV mid=transform((lx+hx)*.5f,(ly+hy)*.5f,(lz+hz)*.5f,0,0);float slope=W/(2*focal);
 if(mid.z+radius<.06f||ab(mid.x)>mx(0,mid.z)*slope+radius*(1+slope)||ab(mid.y)>mx(0,mid.z)*H/(2*focal)+radius*(1+H/(2*focal)))return;}

 if(!t)t=surfaceTex;
 if(!lightingSplit&&(sceneLightCount||windowLightCount200)&&t&&t->repeat){float u=(float)sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)+(b.z-a.z)*(b.z-a.z)),v=(float)sqrt((d.x-a.x)*(d.x-a.x)+(d.y-a.y)*(d.y-a.y)+(d.z-a.z)*(d.z-a.z));if(mx(u,v)>2.4f&&mx(u,v)<40&&dst(camx,camz,(a.x+c.x)*.5f,(a.z+c.z)*.5f)<22){int nx=(int)clamp((float)floor(u/1.8f)+1,1,6),ny=(int)clamp((float)floor(v/1.8f)+1,1,6);lightingSplit=1;for(int y=0;y<ny;y++)for(int x=0;x<nx;x++){float x0=(float)x/nx,x1=(float)(x+1)/nx,y0=(float)y/ny,y1=(float)(y+1)/ny;V3 aa=light_lerp3(a,d,y0),bb=light_lerp3(b,c,y0),cc=light_lerp3(a,d,y1),dd=light_lerp3(b,c,y1);quad(light_lerp3(aa,bb,x0),light_lerp3(aa,bb,x1),light_lerp3(cc,dd,x1),light_lerp3(cc,dd,x0),col,t,light);}lightingSplit=0;return;}}

 if(!winterEnabled&&t==&mat_grass){col=mix(col,seasonVisual140==3?0xb29b61:seasonVisual140==1?0x9dbb83:0x8da46e,.44f);}
 if(winterEnabled&&t){season_prepare();if(t==&mat_grass){t=&snowTextures[0];col=0xffffff;}else if(t==&mat_asphalt){t=&snowTextures[1];col=0xe1e5e6;}else if(t==&mat_pavement){t=&snowTextures[2];col=0xf1f2f0;}else if(t==&mat_roof){t=&snowTextures[3];col=0xffffff;}}
 float dx=b.x-a.x,dy=b.y-a.y,dz=b.z-a.z,ex=c.x-a.x,ey=c.y-a.y,ez=c.z-a.z;
 float nx=dy*ez-dz*ey,ny=dz*ex-dx*ez,nz=dx*ey-dy*ex;
 U32 material=col;float baseLight=light;V3 center={(a.x+b.x+c.x+d.x)*.25f,(a.y+b.y+c.y+d.y)*.25f,(a.z+b.z+c.z+d.z)*.25f},normal={nx,ny,nz};U32 tint;light*=lighting_at(center,normal,&tint);col=color_multiply(col,tint);
 PV p=transform(a.x,a.y,a.z,0,1),q=transform(b.x,b.y,b.z,1,1),r=transform(c.x,c.y,c.z,1,0),s=transform(d.x,d.y,d.z,0,0);
 if((sceneLightCount||windowLightCount200)&&dst(camx,camz,center.x,center.z)<28){V3 vv[4]={a,b,c,d};PV*pp[4]={&p,&q,&r,&s};for(int k=0;k<4;k++){U32 tc;float vl=lighting_at(vv[k],normal,&tc);U32 vc=shade(color_multiply(material,tc),baseLight*vl);pp[k]->lr=(vc>>16)&255;pp[k]->lg=(vc>>8)&255;pp[k]->lb=vc&255;}}

 if(t&&t->repeat){V3 points[4]={a,b,c,d};PV *verts[4]={&p,&q,&r,&s};int horizontal=ab(ny)>ab(nx)&&ab(ny)>ab(nz),side=ab(nx)>ab(nz);for(int k=0;k<4;k++){verts[k]->u=(horizontal?points[k].x:side?points[k].z:points[k].x)*(.4f*t->repeat);verts[k]->v=(horizontal?points[k].z:-points[k].y)*(.4f*t->repeat);}}

 if(gpuActive){gpu_quad(p,q,r,s,col,t,light);return;}
 tri(p,q,r,col,t,light);tri(p,r,s,col,t,light);
}
static void box(float x,float y,float z,float sx,float syy,float sz,U32 c){
 if(ab(x-camx)>viewdist+sx||ab(z-camz)>viewdist+sz)return;float radius=(sx+syy+sz)*.55f;float cx=(x-camx)*cy-(z-camz)*sy,cz=(x-camx)*sy+(z-camz)*cy;if(cz+radius<.1f||ab(cx)>mx(cz,0)*W/(2*focal)+radius*2)return;
 float l=x-sx*.5f,r=x+sx*.5f,b=y-syy*.5f,t=y+syy*.5f,f=z-sz*.5f,k=z+sz*.5f;
 V3 p[8]={{l,b,f},{r,b,f},{r,t,f},{l,t,f},{l,b,k},{r,b,k},{r,t,k},{l,t,k}};
 if(camz<f)quad(p[0],p[1],p[2],p[3],c,0,1);
 if(camz>k)quad(p[5],p[4],p[7],p[6],c,0,.86f);
 if(camx<l)quad(p[4],p[0],p[3],p[7],c,0,.9f);
 if(camx>r)quad(p[1],p[5],p[6],p[2],c,0,.9f);
 if(camy>t)quad(p[3],p[2],p[6],p[7],c,0,1.12f);
 if(camy<b)quad(p[4],p[5],p[1],p[0],c,0,.65f);
}
static void plane(float x,float y,float z,float w,float h,U32 c,const Texture*t){V3 a={x-w/2,y-h/2,z},b={x+w/2,y-h/2,z},cc={x+w/2,y+h/2,z},d={x-w/2,y+h/2,z};quad(a,b,cc,d,c,t,1.25f);}
static void camera(void){cy=(float)cos(yaw);sy=(float)sin(yaw);cp=(float)cos(pitch);sp=(float)sin(pitch);focal=W/(2*(float)tan(field*PI/360));}
static void clear_scene(float day){
 sunlight=day;draws=0;fogcolor=mix(0x222d41,winterEnabled?0x9da4a2:0xa9a6a0,day);
 if(gpuActive){gpu_frame_begin(day);return;}
 U32 top=mix(0x111a30,winterEnabled?0x7d898b:0x728d99,day),horizon=mix(0x39444c,winterEnabled?0xa6aaa6:0xe0bf99,day);
 for(int y=0;y<H;y++){float t=clamp((float)y/(H*.68f),0,1);U32 c=mix(top,horizon,t);for(int x=0;x<W;x++){frame[y*W+x]=c;depth[y*W+x]=0;}}
 /* The sun is a world-oriented distant disc, not a screen overlay. */
 PV s=transform(camx-90,camy+40,camz+120,0,0);if(!winterEnabled&&s.z>0){int sx=(int)(W*.5f+s.x*focal/s.z),yy=(int)(H*.5f-s.y*focal/s.z);for(int y=-13;y<=13;y++)for(int x=-13;x<=13;x++)if(x*x+y*y<169&&sx+x>=0&&sx+x<W&&yy+y>=0&&yy+y<H)frame[(yy+y)*W+sx+x]=0xf9d1a0;}
 camera();
}

static U32 aa_copy[MAXW*MAXH];
static int luma(U32 c){return (((c>>16)&255)*3+((c>>8)&255)*6+(c&255))/10;}
static void smooth_edges(void){if(gpuActive||!antialias||!postAA)return;memcpy(aa_copy,frame,W*H*4);for(int y=1;y<H-1;y++){int i=y*W+1;for(int x=1;x<W-1;x++,i++){U32 c=aa_copy[i];int a=luma(aa_copy[i-1]),b=luma(aa_copy[i+1]),u=luma(aa_copy[i-W]),d=luma(aa_copy[i+W]);if(ab(a-b)>42&&ab(u-d)>28){U32 avg=mix(mix(aa_copy[i-1],aa_copy[i+1],.5f),mix(aa_copy[i-W],aa_copy[i+W],.5f),.5f);frame[i]=mix(c,avg,.30f);}}}}
#include "audio.h"
