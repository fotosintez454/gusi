/* Shared authored floors, shaft cut-outs and supported handrails. */
static void beam(V3 a,V3 b,float ra,float rb,U32 color);
#ifdef HEADLESS
typedef struct{float x,y,z,w,d;} FloorRect203;
static FloorRect203 floorRects203[16384];static int floorAudit203=0,floorCount203=0;
typedef struct{float x,z,base,top;} RailPost203;static RailPost203 railPosts203[2048];static int railCount203=0;
#endif
static void floor_record203(float x,float y,float z,float w,float d){
#ifdef HEADLESS
 if(floorAudit203&&floorCount203<16384)floorRects203[floorCount203++]=(FloorRect203){x,y,z,w,d};
#endif
}
static void floor203(float x,float y,float z,float w,float d){floor_record203(x,y,z,w,d);surfaceTex=&mat_concrete;box(x,y-.09f,z,w,.18f,d,0xb1b0a0);surfaceTex=0;}
static void floor_hole203(float x,float y,float z,float w,float d,float hx,float hz,float hw,float hd){float l=x-w*.5f,r=x+w*.5f,f=z-d*.5f,b=z+d*.5f,hl=hx-hw*.5f,hr=hx+hw*.5f,hf=hz-hd*.5f,hb=hz+hd*.5f;
 floor203((l+hl)*.5f,y,z,hl-l,d);floor203((hr+r)*.5f,y,z,r-hr,d);floor203(hx,y,(f+hf)*.5f,hw,hf-f);floor203(hx,y,(hb+b)*.5f,hw,b-hb);
}
static void stair_rail203(float x,float z0,float z1,float y0,float y1){surfaceTex=0;for(int k=0;k<4;k++){float t=k/3.f,z=lerp(z0,z1,t),base=lerp(y0,y1,t);box(x,base+.43f,z,.05f,.86f,.05f,0x536365);
#ifdef HEADLESS
 if(floorAudit203&&railCount203<2048)railPosts203[railCount203++]=(RailPost203){x,z,base,base+.86f};
#endif
 }beam(vp(x,y0+.86f,z0),vp(x,y1+.86f,z1),.035f,.035f,0x536365);
}
static int building_visible203(float x,float z,float w,float h,float d){float radius=(float)sqrt((w*w+d*d)*WORLD_XZ*WORLD_XZ*.25f+h*h*.25f);PV p=transform(x,h*.5f,z,0,0);float sx=W/(2*focal),sy=H/(2*focal);return p.z+radius>=.06f&&ab(p.x)<=mx(0,p.z)*sx+radius*(1+sx)&&ab(p.y)<=mx(0,p.z)*sy+radius*(1+sy);}
static void garage203(Prop150*p,int n){float x=p->x,z=p->z;int brick=p->type==11||n%3==0,open=p->type==4&&!!(g.zoneFlags150&(1u<<(n-17)));U32 wall=brick?0x897563:0x767f70,gate=(U32[]){0x506b62,0x67584e,0x65767c,0x7c6a55}[n%4];const Texture*old=surfaceTex;surfaceTex=brick?&mat_concrete:&mat_metal;
 box(x-3.42f,1.34f,z,.16f,2.68f,6,wall);box(x+3.42f,1.34f,z,.16f,2.68f,6,wall);box(x,1.34f,z+2.92f,7,2.68f,.16f,wall);box(x,2.68f,z,7,.08f,6,0x4d5754);surfaceTex=0;
 quadstrip(vp(x-3.6f,2.75f,z-3.1f),vp(x+3.6f,2.75f,z-3.1f),vp(x+3.6f,2.85f,z+3.1f),vp(x-3.6f,2.85f,z+3.1f),0x59635d);
 box(x,2.72f,z-3.07f,7.2f,.12f,.10f,0x3e504c);box(x,2.82f,z+3.07f,7.2f,.12f,.10f,0x3e504c);
 if(brick)for(int side=-1;side<=1;side+=2)for(int k=1;k<9;k++)box(x+side*3.505f,k*.28f,z,.012f,.015f,5.9f,0x6c645b);
 if(!open){surfaceTex=&mat_metal;box(x,1.30f,z-2.94f,6.8f,2.6f,.12f,gate);surfaceTex=0;for(int j=0;j<15;j++)box(x-3.15f+j*.45f,1.30f,z-3.015f,.025f,2.45f,.025f,0x435750);
 box(x,1.30f,z-3.04f,.055f,2.55f,.04f,0x354945);for(int e=-1;e<=1;e+=2){box(x+e*.12f,1.1f,z-3.085f,.10f,.16f,.08f,0x9b9a80);for(int k=0;k<2;k++)box(x+e*3.24f,.55f+k*1.5f,z-3.06f,.13f,.20f,.06f,0x3f5149);}box(x,1.01f,z-3.14f,.12f,.15f,.07f,0x9d987e);
 }else for(int e=-1;e<=1;e+=2){surfaceTex=&mat_metal;box(x+e*3.59f,1.30f,z-1.3f,.12f,2.6f,3.2f,gate);surfaceTex=0;}
 box(x-2.35f,2.33f,z-3.04f,.58f,.28f,.04f,0xd0c5a4);for(int k=0;k<(n%5)+1;k++)box(x-2.56f+k*.10f,2.33f,z-3.07f,.035f,.16f,.015f,0x354c44);
 box(x,.025f,z,6.8f,.05f,5.8f,0x555b52);box(x+3.54f,1.3f,z+2.85f,.07f,2.6f,.07f,0x56635a);
 if(winterEnabled)quadstrip(vp(x-3.55f,2.785f,z-3.05f),vp(x+3.55f,2.785f,z-3.05f),vp(x+3.55f,2.885f,z+3.05f),vp(x-3.55f,2.885f,z+3.05f),0xb5b9ae);
 surfaceTex=old;
}
