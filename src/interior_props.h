static void art_kitchen_table(float x,float y,float z){
 surfaceTex=&art_wood;box(x,y+.74f,z,1.35f,.09f,.82f,0xffffff);
 for(int a=-1;a<=1;a+=2)for(int b=-1;b<=1;b+=2)box(x+a*.56f,y+.36f,z+b*.32f,.055f,.68f,.055f,0xffffff);
 surfaceTex=0;
}
/* Original editable low-poly prop geometry. Texture sources documented in art/reference_pass. */
static void art_lathe(float x,float y,float z,const float*radius,const float*height,int rows,int sides,U32 col){for(int j=0;j<rows-1;j++)for(int k=0;k<sides;k++){float a=k*PI*2/sides,b=(k+1)*PI*2/sides;quadstrip(vp(x+cos(a)*radius[j],y+height[j],z+sin(a)*radius[j]),vp(x+cos(b)*radius[j],y+height[j],z+sin(b)*radius[j]),vp(x+cos(b)*radius[j+1],y+height[j+1],z+sin(b)*radius[j+1]),vp(x+cos(a)*radius[j+1],y+height[j+1],z+sin(a)*radius[j+1]),col);}}
static void art_bevel(float x,float y,float z,float w,float h,float d,U32 color){float c=.07f;float xx[8]={-w/2+c,w/2-c,w/2,w/2,w/2-c,-w/2+c,-w/2,-w/2},zz[8]={-d/2,-d/2,-d/2+c,d/2-c,d/2,d/2,d/2-c,-d/2+c};for(int k=0;k<8;k++){int n=(k+1)%8;quadstrip(vp(x+xx[k],y,z+zz[k]),vp(x+xx[n],y,z+zz[n]),vp(x+xx[n],y+h,z+zz[n]),vp(x+xx[k],y+h,z+zz[k]),color);quadstrip(vp(x,y+h,z),vp(x+xx[k],y+h,z+zz[k]),vp(x+xx[n],y+h,z+zz[n]),vp(x,y+h,z),color);}}
static void art_bulb(float x,float y,float z){surfaceTex=0;beam(vp(x,y+2.78f,z),vp(x,y+2.47f,z),.013f,.013f,0x50483c);box(x,y+2.76f,z,.16f,.035f,.16f,0xd4c8a7);box(x,y+2.45f,z,.07f,.08f,.07f,0x82765c);float old=sunlight;sunlight=1;float r[]={.025f,.047f,.050f,.025f,0},h[]={0,-.025f,-.08f,-.12f,-.125f};art_lathe(x,y+2.41f,z,r,h,5,8,0xffe2a0);sunlight=old;}
static void art_flat(float x,float z,float y,int side,int id){float old=sunlight;sunlight=mx(sunlight,.72f);
 surfaceTex=&art_parquet;box(x+side*4.35f,y+.01f,z+1.4f,4.74f,.012f,12.6f,0xffffff);
 surfaceTex=&art_tiles;box(x+side*2.82f,y+.023f,z-3.04f,1.75f,.012f,3.65f,0xffffff);
 surfaceTex=&art_wallpaper;box(x+side*6.78f,y+1.4f,z+1.4f,.016f,2.8f,12.6f,0xffffff);box(x+side*4.4f,y+.49f,z+7.78f,4.8f,.9f,.016f,0xffffff);box(x+side*4.4f,y+2.73f,z+7.78f,4.8f,.16f,.016f,0xffffff);box(x+side*4.4f,y+1.4f,z-4.895f,4.8f,2.8f,.016f,0xffffff);
 box(x+side*1.895f,y+1.4f,z-2.98f,.016f,2.8f,3.82f,0xffffff);box(x+side*1.895f,y+1.4f,z+4.3f,.016f,2.8f,6.6f,0xffffff);
 surfaceTex=&art_tiles;box(x+side*6.765f,y+1.35f,z-3.4f,.02f,.65f,2.9f,0xffffff);box(x+side*2.82f,y+.8f,z-4.88f,1.75f,1.6f,.025f,0xffffff);surfaceTex=0;
 box(x+side*4.35f,y+2.795f,z+1.4f,4.74f,.014f,12.6f,0xc6b89a);
 /* Wood skirting and coherent room fixtures; no intersecting toilet cabinet. */
 surfaceTex=&art_wood;box(x+side*6.75f,y+.11f,z+1.4f,.035f,.16f,12.6f,0xffffff);box(x+side*4.4f,y+.11f,z+7.74f,4.8f,.16f,.04f,0xffffff);surfaceTex=0;
 float fx=x+side*6.18f,fz=z-.4f;art_bevel(fx,y+.04f,fz,.98f,1.72f,1.10f,0xbdbba4);
 box(x+side*5.675f,y+.63f,fz,.035f,1.10f,.94f,0xd2cbb0);box(x+side*5.675f,y+1.46f,fz,.035f,.49f,.94f,0xd2cbb0);box(x+side*5.644f,y+.99f,fz+.30f,.035f,.28f,.055f,0x706e60);box(x+side*5.644f,y+1.44f,fz+.30f,.035f,.16f,.055f,0x706e60);box(x+side*5.65f,y+1.64f,fz-.28f,.012f,.045f,.17f,0x858773);
 /* Oven and controls face the kitchen rather than the outer wall. */
 float sx=x+side*6.35f,sz=z-4;box(x+side*6.055f,y+.48f,sz,.025f,.52f,.88f,0xc9c6ae);box(x+side*6.035f,y+.47f,sz,.025f,.33f,.67f,0x343b36);box(x+side*6.015f,y+.72f,sz,.035f,.045f,.73f,0x777b6d);for(int k=0;k<3;k++)box(x+side*6.035f,y+.87f,sz-.24f+k*.24f,.04f,.075f,.075f,0x4e5249);
 if(panelStoveOn[panelIndex][id]){float r[]={.115f,.115f,.09f},hh[]={0,.012f,.016f};art_lathe(sx,y+1.027f,sz-.3f,r,hh,3,12,0xbd693c);}
 float kx=sx,kz=sz+.30f;
 /* A narrow counter supports the kettle; it no longer floats against the wall. */
 surfaceTex=&art_wood;box(kx,y+.91f,kz,.82f,.10f,.68f,0xffffff);box(kx,y+.44f,kz,.70f,.82f,.58f,0x887b65);surfaceTex=0;
 float rr[]={.085f,.12f,.115f,.085f,.065f,0},hh[]={0,.025f,.13f,.18f,.20f,.205f};art_lathe(kx,y+.97f,kz,rr,hh,6,12,0xb6b7a5);box(kx,y+1.23f,kz,.06f,.05f,.06f,0x424b41);beam(vp(kx-side*.08f,y+1.09f,kz),vp(kx-side*.21f,y+1.18f,kz),.048f,.022f,0xa2a894);for(int k=0;k<7;k++){float a=k*PI/7,b=(k+1)*PI/7;beam(vp(kx+.115f*cos(a),y+1.18f+.105f*sin(a),kz),vp(kx+.115f*cos(b),y+1.18f+.105f*sin(b),kz),.014f,.014f,0x42473e);}
 if(panelKettleHeat[panelIndex][id]>.8f&&g.uiMotion)for(int k=0;k<4;k++){float t=(float)fmod(realTime*.25f+k*.22f,1);beam(vp(kx-side*.23f+sin(realTime+k)*.02f,y+1.34f+t*.30f,kz),vp(kx-side*.23f+sin(realTime+k+.6f)*.02f,y+1.38f+t*.30f,kz+.01f),.009f,.012f,0xb1b3a3);}
 /* Cup, spoon, bread and a slatted stool around the existing kitchen table. */
 float tx=x+side*5.35f,tz=z-2.7f;float cr[]={.06f,.07f,.07f,.058f,.058f},ch[]={0,.025f,.17f,.17f,.035f};art_lathe(tx,y+.785f,tz,cr,ch,5,10,0xd0c1a0);beam(vp(tx,y+.925f,tz+.07f),vp(tx,y+.875f,tz+.12f),.012f,.012f,0xb7ad90);beam(vp(tx,y+.875f,tz+.12f),vp(tx,y+.815f,tz+.07f),.012f,.012f,0xb7ad90);beam(vp(tx+side*.2f,y+.79f,tz-.20f),vp(tx+side*.2f,y+.79f,tz+.04f),.009f,.009f,0xa8a58d);art_bevel(tx-side*.25f,y+.785f,tz-.1f,.24f,.10f,.16f,0xad8d55);
 /* The cup, spoon and bread belong on this table, not in mid-air. */
 art_kitchen_table(tx,y,tz);
 art_bulb(x+side*5.0f,y,z-1.5f);art_bulb(x+side*4.7f,y,z+4.6f);sunlight=old;surfaceTex=0;
}
