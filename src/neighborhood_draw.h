/* Original editable geometry: no imported models or copied blueprints. */
static U32 softened[4][87381],rebarPixels[87381];static int softReady=0;
static Texture mat_rebar={0,256,256,1,rebarPixels};
static void mip_build(U32*p){int at=0,next=65536;for(int w=256;w>1;w/=2){int nw=w/2;for(int y=0;y<nw;y++)for(int x=0;x<nw;x++){int a=at+y*2*w+x*2;p[next+y*nw+x]=mix(mix(p[a],p[a+1],.5f),mix(p[a+w],p[a+w+1],.5f),.5f);}at=next;next+=nw*nw;}}
static void materials_soften(void){if(softReady)return;Texture*t[]={&mat_wood,&mat_floor,&mat_plaster,&mat_brick};for(int n=0;n<4;n++){for(int y=0;y<256;y++)for(int x=0;x<256;x++){unsigned int h=hash32((x/5)+(y/7)*67+n*917);int grain=(int)(h%13)-6,coarse=(int)(hash32((x/24)+(y/19)*31+n*127)%15)-7;int seam=0;U32 c;if(n==0){seam=(x%64<2)?-23:0;int scratch=(hash32((x/3)+(y/29)*83)%97<4)?-24:0;c=rgb(111+grain+coarse+scratch+seam,99+grain+coarse+scratch+seam,78+grain+coarse+scratch+seam);}else if(n==1){seam=(x%64<2||y%64<2)?-24:0;int wear=(hash32((x/6)+(y/6)*59)%37<3)?-20:0;c=rgb(108+grain+coarse+wear+seam,105+grain+coarse+wear+seam,91+grain+coarse+wear+seam);}else if(n==2){int stain=coarse+(hash32((x/18)+(y/13)*29)%9<2?-26:0),crack=(ab((x*3+y*5+(int)(h%23))%61-30)<1&&h%5==0)?-38:0;int drip=(x%47<2&&y%73<38)?-9:0;c=rgb(151+grain+stain+crack+drip,153+grain+stain+crack+drip,140+grain+stain+crack+drip);}else{int joint=(y%32<3||(x+(y/32%2)*40)%80<3);int dirt=(hash32((x/10)+(y/9)*43)%19<3)?-23:0;c=joint?rgb(102+grain,101+grain,89+grain):rgb(126+grain+coarse+dirt,119+grain+coarse+dirt,99+grain+coarse+dirt);}softened[n][y*256+x]=c;}mip_build(softened[n]);t[n]->packed=softened[n];}for(int y=0;y<256;y++)for(int x=0;x<256;x++){int ridge=(x+y/2)%18;U32 c=ridge<3?0x8d8b7b:ridge<6?0x424b4b:0x666b65;if(hash32(x+y*256)%23<6)c=mix(c,0x875b42,.40f);rebarPixels[y*256+x]=c;}mip_build(rebarPixels);mat_floor.packed=art_parquet_pixels;mat_wood.packed=art_wood_pixels;softReady=1;}
static void room_walls_draw(int h,float y){studio150_draw(h,y);}
static void home_annex_draw(void){float x=hx[0]-9.6f,z=hz[0];if(dst(camx,camz,x,z)>45)return;
 surfaceTex=&mat_concrete;box(x-4.6f,1.5f,z,.2f,3,8.6f,0xb6b6a6);box(x,1.5f,z-4.3f,9.2f,3,.2f,0xb6b6a6);box(x,1.5f,z+4.3f,9.2f,3,.2f,0xb6b6a6);
 surfaceTex=&art_parquet;box(x,.09f,z,9.2f,.16f,8.4f,0xffffff);surfaceTex=0;box(x,2.96f,z,9.2f,.14f,8.4f,0xd3cfb8);
 surfaceTex=&art_wallpaper;box(x-4.48f,1.5f,z,.025f,3.0f,8.2f,0xffffff);box(x,1.5f,z-4.18f,8.9f,3.0f,.025f,0xffffff);box(x,1.5f,z+4.18f,8.9f,3.0f,.025f,0xffffff);surfaceTex=0;
 /* Sofa: base, three cushions, padded back and arms. The seat stays outside the entrance lane. */
 box(x-2.7f,.30f,z+.4f,.96f,.38f,2.55f,0x536057);
 box(x-3.11f,.78f,z+.4f,.18f,.84f,2.55f,0x48564f);
 for(int k=-1;k<=1;k++){
  box(x-2.68f,.55f,z+.4f+k*.78f,.82f,.15f,.72f,(U32[]){0x758078,0x6e796f,0x7b8175}[k+1]);
  box(x-3.00f,.89f,z+.4f+k*.78f,.25f,.58f,.72f,(U32[]){0x68766d,0x707a70,0x657269}[k+1]);
 }
 for(int k=-1;k<=1;k+=2)box(x-2.7f,.60f,z+.4f+k*1.22f,.96f,.48f,.16f,0x5d685f);
 for(int e=-1;e<=1;e+=2)for(int q=-1;q<=1;q+=2)box(x-2.7f+e*.37f,.10f,z+.4f+q*1.12f,.07f,.20f,.07f,0x49483f);
 art_kitchen_table(x,.0f,z+.4f);box(x,.18f,z+.5f,2.2f,.015f,2.5f,0x78674f);
 /* A modest framed family photo/calendar breaks the empty rear wall. */
 box(x+2.65f,1.82f,z+4.14f,.92f,.72f,.045f,0x675945);
 box(x+2.65f,1.82f,z+4.105f,.80f,.60f,.025f,0xb7ad91);
 box(x+2.65f,1.98f,z+4.086f,.62f,.025f,.012f,0x69776d);
 for(int n=0;n<5;n++)box(x+2.42f+n*.11f,1.70f,z+4.086f,.035f,.035f,.012f,0x777b68);
 /* v180: no television/sideboard obstacle. */
 if(g.homeLevel>=3){float px=hx[0]-11.7f,pz=hz[0]-3.5f;box(px,.35f,pz,.7f,.7f,.7f,0x88715b);for(int n=0;n<4;n++){beam(vp(px,.6f,pz),vp(px+(n-1.5f)*.10f,1.2f,pz),.018f,.008f,0x617d5c);if(g.herbDay!=g.day)soft_mesh(px+(n-1.5f)*.10f,1.13f,pz,.18f,.05f,.07f,n,0x698761);}}
 art_bulb(x,0,z);surfaceTex=0;}

static void tire_bed(float x,float z,int n){float y=.12f,outer=.58f,inner=.37f;surfaceTex=0;U32 color=n%2?0x647f78:0x9d9061;for(int k=0;k<20;k++){float a=k*PI/10,b=(k+1)*PI/10;V3 p=vp(x+cos(a)*outer,y,z+sin(a)*outer),q=vp(x+cos(b)*outer,y,z+sin(b)*outer),r=vp(q.x,y+.25f,q.z),s=vp(p.x,y+.25f,p.z);quadstrip(p,q,r,s,color);quadstrip(r,s,vp(x+cos(a)*inner,y+.25f,z+sin(a)*inner),vp(x+cos(b)*inner,y+.25f,z+sin(b)*inner),winterEnabled?0xd9e0d9:0x4f5f45);}for(int k=0;k<5;k++){float xx=x+sin(k*2.3f)*.24f,zz=z+cos(k*2.3f)*.24f;beam(vp(xx,.25f,zz),vp(xx,.62f,zz),.01f,.005f,0x7e8164);if(!winterEnabled)soft_mesh(xx,.64f,zz,.10f,.045f,.10f,0,k%2?0xc89c87:0xcfbf78);}}
static void snowman(float x,float z){if(!winterEnabled)return;soft_mesh(x,.53f,z,.51f,.53f,.48f,0,0xdde5e1);soft_mesh(x,1.2f,z,.36f,.36f,.35f,0,0xe4ebe5);soft_mesh(x,1.74f,z,.25f,.26f,.25f,0,0xe4e9df);for(int side=-1;side<=1;side+=2){box(x+side*.095f,1.81f,z-.23f,.035f,.035f,.035f,0x313c39);beam(vp(x+side*.29f,1.3f,z),vp(x+side*.91f,1.6f,z),.025f,.013f,0x80725a);}beam(vp(x,1.7f,z-.23f),vp(x,1.68f,z-.52f),.045f,.004f,0xc88b50);box(x,2.04f,z,.44f,.27f,.4f,0x657772);box(x,1.48f,z,.56f,.12f,.52f,0x957a65);for(int n=0;n<3;n++)box(x,.85f+n*.22f,z-.4f,.045f,.045f,.045f,0x45504b);}
static void neighborhood_details(float day){home_annex_draw();}
