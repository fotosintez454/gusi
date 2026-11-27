/* Emission is explicit, independent of world sunlight and light direction. */
static void glow150(V3 a,V3 b,V3 c,V3 d,U32 color){PV aa=transform(a.x,a.y,a.z,0,0),bb=transform(b.x,b.y,b.z,1,0),cc=transform(c.x,c.y,c.z,1,1),dd=transform(d.x,d.y,d.z,0,1);if(gpuActive)gpu_quad(aa,bb,cc,dd,color,0,1);else{tri(aa,bb,cc,color,0,1);tri(aa,cc,dd,color,0,1);}}
static int window_household180(int h,int k){
 if(h<NH)return k<3?-1:(k==5?1:0);
 if(k>=40){int w=(k-40)/7,n=(k-40)%7;return n==3?-1:w*2+(n>3);}
 if(k>=20)return ((k-20)/4)*2+((k-20)%4>=2);
 return (k/8)*2+(k%8>=2);
}
static int window_lit151(int h,int f,int k){int side=window_household180(h,k);return side<0?(g.hours<9||g.hours>=16):flat_lit180(h,f,side);}
static void pane_part201(V3 p,V3 q,V3 v,V3 u,float a,float b,U32 col,int emissive){
 V3 aa=blend3(p,q,a),bb=blend3(p,q,b),cc=blend3(u,v,b),dd=blend3(u,v,a);
 if(emissive)glow150(aa,bb,cc,dd,col);else quadstrip(aa,bb,cc,dd,col);
}
static void glass150(V3 p,V3 q,V3 v,V3 u,int h,int f,int k){
 unsigned seed=hash32(g.worldSeed+h*389+f*173+k*31);
 int inside160=h<NH?(ab(camx-hx[h])<4.8f&&camz>hz[h]-4.4f&&camz<hz[h]+4.4f):(h<NH+PANEL_COUNT&&ab(camx-panel_origin_x(h-NH))<14.2f&&ab(camz-panel_origin_z(h-NH))<8.3f);
 int lit=window_lit151(h,f,k),side=window_household180(h,k);float left=seed%4!=0?(seed%3==0?.24f:.12f):0,right=seed%3==1?.84f:1;
 U32 curtain=(U32[]){0x666458,0x77715e,0x626e6b}[seed%3];
 surfaceTex=0;
 if(left>0)pane_part201(p,q,v,u,0,left,curtain,0);
 if(!inside160)pane_part201(p,q,v,u,left,right,lit?(side<0?0xb8a77d:flat_color180(h,f,side)):(U32[]){0x414c53,0x536068,0x67716b,0x7b8076}[seed%4],lit);
 if(right<1)pane_part201(p,q,v,u,right,1,curtain,0);
}
/* v170: aperture, frame, lintel and mirrored hinge use the same measured span. */
static void flat_door170(float x,float y,float z,int open,float w,int hingeSide,int swing){
 surfaceTex=0;float hinge=x+hingeSide*w*.5f,freeX=open?hinge: x-hingeSide*w*.5f,freeZ=z+(open?swing*(w-.04f):0);
 V3 a=vp(hinge,y+.03f,z),b=vp(freeX,y+.03f,freeZ),c=vp(freeX,y+2.43f,freeZ),d=vp(hinge,y+2.43f,z);
 float nx=open?.025f:0,nz=open?0:.025f;
 surfaceTex=&art_wood;quadstrip(vp(a.x-nx,a.y,a.z-nz),vp(b.x-nx,b.y,b.z-nz),vp(c.x-nx,c.y,c.z-nz),vp(d.x-nx,d.y,d.z-nz),0xb4afa1);
 quadstrip(vp(b.x+nx,b.y,b.z+nz),vp(a.x+nx,a.y,a.z+nz),vp(d.x+nx,d.y,d.z+nz),vp(c.x+nx,c.y,c.z+nz),0xb4afa1);surfaceTex=0;
 beam(a,d,.025f,.025f,0x655f54);beam(b,c,.025f,.025f,0x655f54);beam(d,c,.025f,.025f,0x655f54);
 V3 h=blend3(b,c,.46f);box(h.x+(open?.055f:0),h.y,h.z+(open?0:-.055f),.10f,.055f,.10f,0xbab5a3);
 for(int e=-1;e<=1;e+=2)box(x+e*(w*.5f+.025f),y+1.225f,z,.065f,2.45f,.24f,0xa3a396);
 box(x,y+2.475f,z,w+.12f,.075f,.24f,0xa3a396);
}
static void flat_leafw150(float x,float y,float z,int open,float w){float hw=w*.5f;V3 p=vp(x-hw,y,z),q=vp(x-hw+(open?.09f:w),y,z+(open?w-.02f:0));quadstrip(p,q,vp(q.x,y+2.45f,q.z),vp(p.x,y+2.45f,p.z),0x626258);beam(q,vp(q.x,y+2.45f,q.z),.025f,.025f,0x887e69);box(q.x-.1f,y+1.1f,q.z-.04f,.12f,.06f,.06f,0xb0aa91);}
static void flat_leaf150(float x,float y,float z,int open){flat_leafw150(x,y,z,open,FLAT_DOOR_HALF*2);}
static void balcony_leaf150(int h,int f,int s){float x=hx[h]+(s?2.8f:-2.8f),y=f*3,z=hz[h]+4.48f;int open=balcony_open150(h,f,s);V3 p=vp(x-.65f,y+.05f,z),q=vp(p.x+(open?.10f:1.3f),y+.05f,z-(open?1.30f:0)),v=vp(q.x,y+2.6f,q.z),u=vp(p.x,y+2.6f,p.z);glass150(p,q,v,u,h,f,s+13);beam(p,q,.045f,.045f,0xb0b3a7);beam(q,v,.045f,.045f,0xb0b3a7);beam(v,u,.045f,.045f,0xb0b3a7);beam(u,p,.045f,.045f,0xb0b3a7);beam(blend3(p,u,.30f),blend3(q,v,.30f),.035f,.035f,0xb0b3a7);V3 handle=blend3(q,v,.43f);box(handle.x-.05f,handle.y,handle.z-.05f,.08f,.14f,.07f,0xccc4a6);
 for(int edge=-1;edge<=1;edge+=2)box(x+edge*.75f,y+1.3f,z,.18f,2.65f,.22f,0x9da39f);
 surfaceTex=&mat_concrete;box(x,y-.03f,z+1.0f,2.55f,.16f,2.0f,0x777d80);surfaceTex=0;for(int edge=-1;edge<=1;edge+=2){box(x+edge*1.14f,y+.56f,z+1.0f,.05f,1.12f,1.95f,0x414c53);box(x+edge*1.14f,y+1.1f,z+1,.07f,.07f,2,0x555f62);}box(x,y+1.1f,z+1.61f,2.3f,.07f,.06f,0x414c53);for(int n=0;n<9;n++)box(x-1.1f+n*.275f,y+.55f,z+1.61f,.035f,1.1f,.035f,0x555f62);if(winterEnabled)box(x,y+.066f,z+1,2.2f,.025f,1.45f,0xa6a9a5);
}
static void lived_flat170(float x,float z,float y,int sign);
static void studio150_draw(int h,float y){float x=hx[h],z=hz[h];int f=(int)(y/3);U32 wall=(U32[]){0xc2c9c0,0xcac4b6,0xb5c2bb,0xc5c6b9}[hash32(g.worldSeed+h*17+f*91)%4];surfaceTex=wallpaper141(h,f);
 float seg[3][2]={{-4.82f,-3.05f},{-1.75f,1.75f},{3.05f,4.82f}};for(int k=0;k<3;k++)box(x+(seg[k][0]+seg[k][1])*.5f,y+1.5f,z-1.9f,seg[k][1]-seg[k][0],3,.20f,wall);box(x,y+2.75f,z-1.9f,9.64f,.5f,.20f,wall);box(x,y+1.5f,z+1.25f,.18f,3,6.3f,wall);surfaceTex=0;
 for(int s=0;s<2;s++){float sign=s?1:-1,cx=x+sign*2.4f;int id=unit150(h,f,s);unsigned flags=g.things150[id];surfaceTex=&art_parquet;box(cx,y+.02f,z+1.23f,4.56f,.04f,6.2f,0xb1ac95);surfaceTex=0;
 flat_door170(cx,y,z+FLAT_ENTRY_B,!!(g.flatDoors150[h]&(1u<<(f*2+s))),FLAT_DOOR_HALF*2,-(int)sign,-1);box(cx,y+2.1f,z-2.02f,.25f,.14f,.035f,0x827b65);for(int n=0;n<(f*2+s+1)%5+1;n++)box(cx-.07f+n*.025f,y+2.10f,z-2.045f,.008f,.07f,.01f,0xc1bda7);
 /* Enclosed 1.5x2.4m wet room: unobstructed 1.1m door opening. */
 box(x+sign*BATH_A,y+1.5f,z+(FLAT_ENTRY_B+BATH_DOOR_B0)*.5f,WALL_T,3,BATH_DOOR_B0-FLAT_ENTRY_B,wall);box(x+sign*BATH_A,y+1.5f,z+(BATH_DOOR_B1+BATH_B)*.5f,WALL_T,3,BATH_B-BATH_DOOR_B1,wall);box(x+sign*BATH_A,y+2.725f,z+(BATH_DOOR_B0+BATH_DOOR_B1)*.5f,WALL_T,.55f,BATH_DOOR_B1-BATH_DOOR_B0,wall);box(x+sign*4.0f,y+1.5f,z+.5f,1.6f,3,.18f,wall);surfaceTex=&art_tiles;box(x+sign*4,y+.04f,z-.65f,1.4f,.035f,2.18f,0xa6a9a1);surfaceTex=0;
 /* Kitchen / living room split. A clear 1.4m doorway, not a studio. */
 surfaceTex=wallpaper141(h,f);
 box(x+sign*(WALL_HIT+ROOM_DOOR_A0)*.5f,y+1.5f,z+ROOM_WALL_B,ROOM_DOOR_A0-WALL_HIT,3,.18f,wall);
 box(x+sign*(ROOM_DOOR_A1+HOUSE_HALF_W)*.5f,y+1.5f,z+ROOM_WALL_B,HOUSE_HALF_W-ROOM_DOOR_A1,3,.18f,wall);
 box(x+sign*ROOM_DOOR_A,y+2.75f,z+ROOM_WALL_B,ROOM_DOOR_A1-ROOM_DOOR_A0,.5f,.18f,wall);surfaceTex=0;
 flat_door170(x+sign*ROOM_DOOR_A,y,z+ROOM_WALL_B,!!(flags&(1u<<17)),ROOM_DOOR_A1-ROOM_DOOR_A0,-(int)sign,1);
 /* Bathroom door is a narrow leaf against the shared wet-room partition. */
 if(flags&(1u<<16))box(x+sign*(BATH_A+.54f),y+1.2f,z+BATH_DOOR_B1,1.08f,2.4f,.055f,0x92988c);
 else box(x+sign*BATH_A,y+1.2f,z+(BATH_DOOR_B0+BATH_DOOR_B1)*.5f,.055f,2.4f,BATH_DOOR_B1-BATH_DOOR_B0,0x92988c);
 surfaceTex=&art_tiles;box(cx,y+.044f,z-.43f,4.56f,.03f,2.7f,0x888f84);surfaceTex=0;
 /* Low rug, skirting, picture and books stay out of walking space. */
 box(x+sign*2.5f,y+.047f,z+2.55f,1.25f,.018f,2.1f,(U32[]){0x685753,0x66675b,0x586b6b}[hash32(g.worldSeed+id)%3]);
 for(int e=-1;e<=1;e+=2)box(cx+e*2.24f,y+.12f,z+1.22f,.04f,.17f,6.15f,0x625d50);
 box(x+sign*.14f,y+1.8f,z+2.8f,.035f,.65f,.85f,0x6f6658);box(x+sign*.16f,y+1.8f,z+2.8f,.025f,.49f,.68f,0x7e8b85);
 lived_flat170(x,z,y,(int)sign);
 /* Every substantial furniture piece uses object150 for rendering and collision. */
 for(int t=0;t<22;t++){if(t==10||t==20||(t==6&&g.chairMoved200[id]))continue;float a,b,c,rx,rz;object150(h,f,s,t,&a,&b,&c,&rx,&rz);if(t==0){box(a,y+.98f,c,rx*2,1.96f,rz*2,0xa2a89e);if(flags&2){box(a-sign*.46f,y+1.03f,c-.35f,.05f,1.8f,.70f,0xaeb5aa);for(int k=0;k<3;k++)box(a,y+.5f+k*.45f,c-.4f,.70f,.035f,.06f,0x656e68);}else{box(a,y+1.02f,c+.41f,.75f,1.85f,.035f,0xb6b9ad);box(a+sign*.25f,y+1.1f,c+.45f,.035f,.25f,.035f,0x6c7771);}}
 else if(t==1){surfaceTex=&mat_wood;box(a,y+.38f,c,.46f,.72f,.48f,0x887b65);box(a,y+.76f,c,.50f,.10f,.52f,0xbdb6a1);surfaceTex=0;float kr[]={.075f,.125f,.12f,.09f,.06f,0},kh[]={0,.03f,.18f,.23f,.245f,.25f};art_lathe(a,y+.82f,c,kr,kh,6,12,0xc7c9bd);beam(vp(a,y+.95f,c),vp(a-sign*.22f,y+1.04f,c),.04f,.025f,0xc7c9bd);for(int k=0;k<6;k++){float v=k*PI/6,w=(k+1)*PI/6;beam(vp(a+.12f*cos(v),y+1.03f+.12f*sin(v),c),vp(a+.12f*cos(w),y+1.03f+.12f*sin(w),c),.018f,.018f,0x353d3b);}box(a+.17f,y+.88f,c,.04f,.035f,.04f,g.kettleOn200[id]?0xe4a45c:0x56615b);if(g.kettle200[id]>.85f&&g.uiMotion)for(int k=0;k<3;k++){float t=(float)fmod(realTime*.5f+k*.3f,1);beam(vp(a-sign*.22f,y+1.08f+t*.23f,c),vp(a-sign*.22f+.02f*sin(realTime+k),y+1.12f+t*.23f,c),.008f,.01f,0xc2ccc5);}}
 else if(t==2){
 surfaceTex=&mat_wood;
 /* Side-facing wardrobe: back, top, plinth and two real shelves. No solid infill. */
 box(a-sign*.265f,y+1.01f,c,.035f,1.96f,.9f,0x665742);
 for(int e=-1;e<=1;e+=2)box(a,y+1.01f,c+e*.435f,.56f,1.96f,.035f,0x8c7758);
 box(a,y+1.98f,c,.56f,.05f,.90f,0x9c8665);box(a,y+.07f,c,.56f,.10f,.90f,0x685942);
 box(a,y+.43f,c,.52f,.035f,.84f,0x9c8665);box(a,y+1.51f,c,.52f,.035f,.84f,0x9c8665);
 surfaceTex=0;
 for(int e=-1;e<=1;e+=2){float hingeZ=c+e*.42f,freeZ=flags&4?hingeZ:c,freeX=a+sign*(.29f+(flags&4?.40f:0));
 quadstrip(vp(a+sign*.29f,y+.13f,hingeZ),vp(freeX,y+.13f,freeZ),vp(freeX,y+1.92f,freeZ),vp(a+sign*.29f,y+1.92f,hingeZ),0x8b7353);
 beam(vp(freeX+sign*.018f,y+.88f,freeZ+e*.04f),vp(freeX+sign*.018f,y+1.08f,freeZ+e*.04f),.025f,.025f,0xc0b18b);}
 }

 else if(t==3){box(a,b,c,.18f,.22f,.045f,0xa0a28f);box(a,b,c-.027f,.07f,.12f,.02f,flags&8?0x5e665f:0xb8a77d);
 float lx=cx,lz=z+2.7f;beam(vp(lx,y+2.95f,lz),vp(lx,y+2.67f,lz),.012f,.012f,0x555c55);box(lx,y+2.65f,lz,.44f,.09f,.44f,flat_lit180(h,f,s)?flat_color180(h,f,s):0x626b62);
 if(flat_lit180(h,f,s))glow150(vp(lx-.18f,y+2.60f,lz-.18f),vp(lx+.18f,y+2.60f,lz-.18f),vp(lx+.18f,y+2.60f,lz+.18f),vp(lx-.18f,y+2.60f,lz+.18f),0xb8a77d);}

 else if(t==5){
 box(a,y+.28f,c,rx*2,.16f,rz*2,0x625b4d);
 for(int ex=-1;ex<=1;ex+=2)for(int ez=-1;ez<=1;ez+=2)box(a+ex*(rx-.08f),y+.13f,c+ez*(rz-.08f),.09f,.26f,.09f,0x514d43);
 box(a,y+.47f,c,rx*2-.08f,.22f,rz*2-.08f,0xb8b7a9);
 box(a,y+.61f,c-.30f,rx*1.82f,.08f,rz*1.12f,(U32[]){0x66736c,0x77715e,0x625f60,0x65767a}[hash32(g.worldSeed+id)%4]);
 box(a,y+.64f,c+.55f,rx*1.70f,.10f,.58f,0xd5d0c1);
 box(a,y+.83f,c+1.03f,rx*2,.62f,.12f,0x595e52);
 }
 else if(t==6){
 for(int e=-1;e<=1;e+=2)for(int j=-1;j<=1;j+=2)box(a+e*.21f,y+.22f,c+j*.21f,.055f,.44f,.055f,0x565d56);
 box(a,y+.47f,c,.55f,.10f,.55f,0x827b65);box(a,y+.54f,c,.46f,.055f,.45f,0x9b9684);
 box(a,y+.82f,c-.23f,.55f,.65f,.09f,0x706d5d);
 }
 else if(t==7){box(a,y+.45f,c,rx*2,.90f,rz*2,0x9a9e92);box(a,y+.92f,c,.72f,.04f,.72f,0x444c4d);for(int k=0;k<4;k++)soft_mesh(a+(k%2?-.18f:.18f),y+.95f,c+(k/2?-.18f:.18f),.12f,.02f,.12f,0,flags&128?0xb87c4e:0x252e31);}
 else if(t==8){box(a,y+.45f,c,rx*2,.9f,rz*2,0x77715e);box(a,y+.95f,c,rx*2,.07f,rz*2,0xa6aaa0);soft_mesh(a,y+.98f,c,.23f,.04f,.16f,0,0x56625f);beam(vp(a,y+1,c+.1f),vp(a,y+1.25f,c+.1f),.025f,.025f,0xa9b0a3);if(flags&256)beam(vp(a,y+1.2f,c),vp(a,y+.96f,c),.008f,.012f,0x8eaba5);}
 else if(t==9&&(flags&512)){box(a-sign*.7f,y+1.57f,c,.40f,1.8f,.035f,0x8f8e7b);box(a+sign*.7f,y+1.57f,c,.40f,1.8f,.035f,0x8f8e7b);}
 else if(t==10){box(a,b,c,.28f,.045f,.19f,0x8b735b);}
 else if(t==13){box(a,b,c,rx*2,.09f,rz*2,0x8a826b);for(int e=-1;e<=1;e+=2)for(int j=-1;j<=1;j+=2)box(a+e*.43f,y+.39f,c+j*.27f,.055f,.78f,.055f,0x62685a);soft_mesh(a+.2f,y+.94f,c,.065f,.1f,.065f,0,0xb5b3a3);}
 else if(t==14){
 box(a,y+.18f,c,.42f,.34f,.48f,0xa6aaa0);
 soft_mesh(a,y+.43f,c+.06f,.27f,.20f,.34f,0,0xd4d4c8);
 box(a,y+.56f,c+.06f,.36f,.055f,.43f,0x777d75);
 box(a,y+.77f,c-.28f,.42f,.58f,.18f,0xb5b9ad);
 box(a,y+1.08f,c-.28f,.34f,.045f,.15f,0x8d948a);
 }
 else if(t==15)box(a,b,c,.035f,.72f,.64f,0x798c89);
 else if(t==18){box(a,y+.08f,c,.94f,.16f,.78f,0xa6a99d);beam(vp(a+sign*.39f,y+.2f,c+.34f),vp(a+sign*.39f,y+2.2f,c+.34f),.025f,.025f,0xa1a89f);beam(vp(a+sign*.39f,y+2.2f,c+.34f),vp(a,y+2.2f,c+.34f),.035f,.035f,0xa1a89f);if(flags&(1u<<18))for(int j=0;j<4;j++)beam(vp(a-.15f+j*.1f,y+2.18f,c+.3f),vp(a-.15f+j*.1f,y+.17f,c+.3f),.005f,.005f,0x7c9997);}

 else if(t==20){box(a,b,c,.29f,.1f,.26f,0x333b40);box(a,b+.08f+(flags&(1u<<20)?.2f:0),c,.33f,.065f,.10f,0x414c53);}
 else if(t==21){
 surfaceTex=&mat_wood;
 for(int e=-1;e<=1;e+=2){box(a+e*.255f,y+.39f,c,.045f,.68f,.60f,0x8a7453);for(int j=-1;j<=1;j+=2)box(a+e*.22f,y+.06f,c+j*.24f,.06f,.12f,.06f,0x514739);}
 box(a,y+.76f,c,.59f,.055f,.65f,0xa58b64);box(a,y+.13f,c,.50f,.04f,.56f,0x7b674d);box(a,y+.39f,c+.28f,.50f,.68f,.035f,0x6a5843);
 float dz=flags&(1u<<21)?-.30f:0;
 box(a,y+.44f,c+dz,.47f,.035f,.51f,0x6d5a40);
 for(int e=-1;e<=1;e+=2)box(a+e*.22f,y+.54f,c+dz,.025f,.18f,.51f,0x958160);
 box(a,y+.54f,c-.30f+dz,.50f,.21f,.04f,0x9e8966);surfaceTex=0;
 box(a,y+.55f,c-.334f+dz,.15f,.028f,.035f,0xbbb49a);
 }


 }
 }
 surfaceTex=wallpaper141(h,f);box(x,y+2.96f,z,9.65f,.08f,8.7f,0x989c90);surfaceTex=0;
}
