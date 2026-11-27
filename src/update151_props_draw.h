/* Shared prop dimensions also feed collision; animation stays inside reserved bounds. */
 if(p->type==6){
 for(int e=-1;e<=1;e+=2){box(x+e*1.1f,.25f,z,.06f,.5f,.65f,0x414c53);box(x+e*1.1f,.9f,z+.38f,.06f,.9f,.06f,0x414c53);}
 for(int j=0;j<3;j++){box(x,.52f,z-.25f+j*.23f,2.8f,.07f,.18f,0x77715e);box(x,.85f+j*.17f,z+.39f,2.8f,.12f,.07f,0x77715e);}
 }else if(p->type==7){box(x,2.6f,z,.15f,5.2f,.15f,0x555e61);box(x,3.9f,z,.75f,.9f,.13f,0x515f63);box(x,3.9f,z-.075f,.45f,.6f,.025f,0xa6a9a5);box(x,1.6f,z-.1f,.48f,.62f,.04f,0x77715e);}
 else if(p->type==8){float a=(float)sin(ridePhase202[n-53]+2.0f*renderAhead202)*rideAmp202[n-53];U32 c=n%2?0x6c7772:0x77715e;
 for(int e=-1;e<=1;e+=2){beam(vp(x+e*1.75f,0,z-1.4f),vp(x+e*1.55f,2.4f,z),.065f,.065f,c);beam(vp(x+e*1.75f,0,z+1.4f),vp(x+e*1.55f,2.4f,z),.065f,.065f,c);}
 beam(vp(x-1.6f,2.4f,z),vp(x+1.6f,2.4f,z),.07f,.07f,c);
 float zz=z+(float)sin(a)*1.7f,yy=2.4f-(float)cos(a)*1.7f;
 for(int e=-1;e<=1;e+=2)beam(vp(x+e*.45f,2.4f,z),vp(x+e*.45f,yy,zz),.014f,.014f,0x827b75);
 box(x,yy,zz,1.05f,.09f,.40f,0x77715e);
 }else if(p->type==9){float a=ridePhase202[n-53]+rideOmega202[n-53]*renderAhead202;box(x,.28f,z,.18f,.56f,.18f,0x555e61);
 for(int j=0;j<12;j++){float aa=a+j*PI/6,bb=a+(j+1)*PI/6;V3 p1=vp(x,.35f,z),p2=vp(x+cos(aa)*1.2f,.35f,z+sin(aa)*1.2f),p3=vp(x+cos(bb)*1.2f,.35f,z+sin(bb)*1.2f);quadstrip(p1,p2,p3,p3,0x77715e);}
 for(int j=0;j<4;j++){float seat=a+PI/4+j*PI/2,sx=x+cos(seat)*.62f,sz=z+sin(seat)*.62f;box(sx,.62f,sz,.43f,.09f,.40f,0x9b8668);box(sx,.48f,sz,.06f,.25f,.06f,0x586c65);float aa=a+j*PI/2;V3 p1=vp(x+cos(aa)*.85f,.36f,z+sin(aa)*.85f),p2=vp(p1.x,1.15f,p1.z);beam(p1,p2,.035f,.035f,0x6b7975);beam(p2,vp(x,1.15f,z),.035f,.035f,0x6b7975);}
 }else if(p->type==10){float a=(float)sin(ridePhase202[n-53]+2*renderAhead202)*rideAmp202[n-53]*(.18f/.55f);
 box(x,.32f,z,.45f,.64f,.5f,0x555e61);beam(vp(x-1.9f,.65f-a*4,z),vp(x+1.9f,.65f+a*4,z),.18f,.07f,0x827b65);
 for(int e=-1;e<=1;e+=2){box(x+e*1.65f,.72f+e*a*3.3f,z,.5f,.09f,.4f,0x77715e);beam(vp(x+e*1.35f,.7f+e*a*2.7f,z),vp(x+e*1.35f,1.05f+e*a*2.7f,z),.03f,.03f,0x697773);}
 }
