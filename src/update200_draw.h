/* Reusable geometry. All portable objects render from their persisted physics bodies. */
static void public_window200(float x,float y,float z,float w,float h,int open){surfaceTex=0;float half=w*.5f;for(int s=-1;s<=1;s+=2){float hinge=x+s*half,freeX=open?hinge:x,freeZ=z+(open?-.5f*w:0);V3 a=vp(hinge,y,z),b=vp(freeX,y,freeZ),c=vp(freeX,y+h,freeZ),d=vp(hinge,y+h,z);if(!open)quadstrip(a,b,c,d,0x6b8481);beam(a,b,.035f,.035f,0xc6c8b6);beam(b,c,.035f,.035f,0xc6c8b6);beam(c,d,.035f,.035f,0xc6c8b6);beam(d,a,.035f,.035f,0xc6c8b6);V3 handle=blend3(b,c,.48f);box(handle.x+s*.04f,handle.y,handle.z+.04f,.05f,.14f,.045f,0xd4cbb2);}}
static void bodies_draw200(void){surfaceTex=0;for(int n=0;n<BODY200;n++){Body200*b=&g.bodies200[n];if(!b->active||dst(camx,camz,b->x,b->z)>32)continue;float x=b->x,y=b->y,z=b->z;
 if(b->kind==12){float by=y-.54f;for(int ex=-1;ex<=1;ex+=2)for(int ez=-1;ez<=1;ez+=2)box(x+ex*.21f,by+.22f,z+ez*.21f,.055f,.44f,.055f,0x565d56);box(x,by+.47f,z,.55f,.10f,.55f,0x827b65);box(x,by+.54f,z,.46f,.055f,.45f,0x9b9684);float dx=sin(b->yaw)*.23f,dz=cos(b->yaw)*.23f;beam(vp(x+cos(b->yaw)*.25f+dx,by+.52f,z-sin(b->yaw)*.25f+dz),vp(x+cos(b->yaw)*.25f+dx,by+1.07f,z-sin(b->yaw)*.25f+dz),.06f,.06f,0x706d5d);beam(vp(x-cos(b->yaw)*.25f+dx,by+.52f,z+sin(b->yaw)*.25f+dz),vp(x-cos(b->yaw)*.25f+dx,by+1.07f,z+sin(b->yaw)*.25f+dz),.06f,.06f,0x706d5d);beam(vp(x-cos(b->yaw)*.25f+dx,by+.9f,z+sin(b->yaw)*.25f+dz),vp(x+cos(b->yaw)*.25f+dx,by+.9f,z-sin(b->yaw)*.25f+dz),.08f,.08f,0x827b65);
 }else if(b->kind==11){box(x,y,z,.44f,.44f,.44f,0x947a52);box(x,y+.222f,z,.08f,.008f,.44f,0xb9aa7d);box(x,y,z+.225f,.16f,.10f,.008f,0xd7ccb0);
 }else if(b->kind==2||b->kind==6||b->kind==7){U32 color=b->kind==2?0x8bafb3:b->kind==6?0x9c8353:0x749282;float rr[]={.065f,.07f,.065f,.032f,.032f},hh[]={0,.025f,.23f,.27f,.34f};art_lathe(x,y-.17f,z,rr,hh,5,8,color);box(x,y+.17f,z,.065f,.025f,.065f,0xc9c5a8);if(b->kind!=7)box(x,y-.015f,z,.142f,.08f,.142f,0xd3c6a5);
 }else if(b->kind==10){float r[]={.065f,.074f,.074f,.06f,.06f},h[]={0,.02f,.18f,.18f,.03f};art_lathe(x,y-.09f,z,r,h,5,10,0xd4d2bd);for(int k=0;k<6;k++){float a=k*PI/3,c=(k+1)*PI/3;beam(vp(x+.085f+.045f*cos(a),y+.01f+.06f*sin(a),z),vp(x+.085f+.045f*cos(c),y+.01f+.06f*sin(c),z),.012f,.012f,0xd4d2bd);}
 }else if(b->kind==8){beam(vp(x-.2f,y-.02f,z-.05f),vp(x+.2f,y+.02f,z+.05f),.045f,.045f,0x9ba29b);beam(vp(x-.08f,y-.04f,z+.08f),vp(x+.1f,y+.04f,z-.09f),.045f,.045f,0x6f7f7e);
 }else{U32 color=b->kind==1?0xb6a26d:b->kind==3?0x7c9c72:b->kind==4?0xc9c7b0:0xc3ad83;box(x,y,z,.22f,.18f,.16f,color);box(x,y+.02f,z-.085f,.16f,.05f,.01f,0x5b6658);if(b->kind==4){box(x,y+.02f,z-.094f,.04f,.13f,.01f,0xa35548);box(x,y+.02f,z-.096f,.13f,.04f,.01f,0xa35548);}}
 }}
static void window_lights200(void){
 /* Exterior pools of coloured light on snow/facade, tied to actual room switches. */
 for(int h=0;h<NH;h++){if(dst(camx,camz,hx[h],hz[h])>28)continue;for(int f=0;f<2;f++)for(int side=0;side<2;side++){float sg=side?1:-1;for(int k=0;k<3;k++){int lit=side?flat_lit180(h,f,k==2?1:0):(g.hours<9||g.hours>=16);if(!lit)continue;float x=hx[h]+window_a180(side,k),z=hz[h]+sg*4.74f;lighting_window200(x,f*3+1.75f,z,5.6f,3.7f,sg,side?flat_color180(h,f,k==2?1:0):0xe0c797);}}
  if(ab(camx-hx[h])<5&&ab(camz-hz[h])<4.5f){int f=(int)clamp((float)floor(g.playerY/3),0,4);if(sunlight>.3f)for(int s=-1;s<=1;s+=2)lighting_room(hx[h]+s*2.8f,f*3+2.45f,hz[h]+3.7f,1.4f,2.2f,sunlight*.9f,0xd7e3e0);}
  /* Entrance lamp over each door throws a warm pool on the porch and snow. */
  if(g.hours<9||g.hours>16)lighting_window200(hx[h]+7.50f,2.40f,hz[h]-8.55f,6.5f,2.9f,-1,0xffd9a0);
  /* The barrel fire lights the yard around it (same condition as street205 draw). */
  if(winterEnabled||g.hours>=17||g.hours<7)lighting_room(20,1.30f,-38,4.5f,4.5f,1.25f,0xff9a3c);
 }
 for(int p=0;p<PANEL_COUNT;p++){float x=panel_origin_x(p),z=panel_origin_z(p);if(dst(camx,camz,x,z)>31)continue;for(int w=0;w<2;w++)for(int side=0;side<2;side++)if(flat_lit180(NH+p,0,w*2+side)){float xx=x+(w?7:-7)+(side?1:-1)*5;lighting_window200(xx,1.75f,z+8.20f,5.6f,3.5f,1,flat_color180(NH+p,0,w*2+side));}}
 for(int n=0;n<4;n++){float x=cityBlockX[n],z=cityBlockZ[n];if(dst(camx,camz,x,z)>30)continue;for(int f=0;f<2;f++)for(int side=0;side<2;side++)if(high_lit180(n,f,side)){U32 c=high_color180(n,f,side);lighting_window200(x+(side?1:-1)*4.8f,f*3+1.4f,z+5.52f,5.8f,3.8f,1,c);lighting_window200(x+(side?1:-1)*6.8f,f*3+1.75f,z-5.50f,5.8f,3.8f,-1,c);}}
}

/* Fine ground tessellation under windows. The same material and lighting model as
   the terrain; close spacing prevents the coarse 4x24m terrain grid losing a pool. */
static void window_ground200(void){
 const Texture*old=surfaceTex;int split=lightingSplit;lightingSplit=1;
 for(int n=0;n<windowLightCount200;n++){WindowLight200*l=&windowLights200[n];if(l->y>2.9f||dst(camx,camz,l->x,l->z)>23)continue;
 for(int ix=0;ix<24;ix++)for(int iz=0;iz<16;iz++){float x=l->x-6.0f+ix*.5f,z=l->z+l->direction*(.22f+iz*.45f),zz=z+l->direction*.45f;float y=terrain_height141(x,z);if(y<-.1f)continue;y+=.012f;surfaceTex=&mat_grass;quadstrip(vp(x,y,z),vp(x+.5f,y,z),vp(x+.5f,y,zz),vp(x,y,zz),0xffffff);}
 }lightingSplit=split;surfaceTex=old;
}
