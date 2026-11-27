/* Shared visual pass for v140: coherent room materials, seed variation and river. */
static void residence_art140(int h,float y){(void)h;(void)y;}
static void residence_lift_draw140(int h){float x=hx[h]+6.1f,z=hz[h]-6.f;ResidenceSave*s=&g.residence[h];surfaceTex=&mat_metal;
 box(x-1.05f,7.5f,z,.18f,15,2.25f,0x7d8f83);box(x,7.5f,z-1.12f,2.25f,15,.18f,0x7d8f83);box(x,7.5f,z+1.12f,2.25f,15,.18f,0x7d8f83);
 box(x,s->y+.04f,z,1.85f,.08f,1.95f,0xb0b6a5);surfaceTex=0;box(x-.91f,s->y+1.3f,z,.035f,2.48f,1.85f,0xb7bcae);box(x,s->y+1.3f,z-.94f,1.8f,2.48f,.035f,0x9ea99c);box(x,s->y+1.3f,z+.94f,1.8f,2.48f,.035f,0x9ea99c);surfaceTex=&mat_metal;box(x,s->y+2.6f,z,1.85f,.08f,1.95f,0xa7b2a4);surfaceTex=0;box(x,s->y+2.54f,z,.55f,.04f,.5f,0xffe6b5);surfaceTex=&mat_metal;
 for(int f=0;f<5;f++){float y=f*3;box(x+1.05f,y+1.2f,z-1.04f,.16f,2.4f,.24f,0x5f796d);box(x+1.05f,y+1.2f,z+1.04f,.16f,2.4f,.24f,0x5f796d);box(x+1.05f,y+2.7f,z,.16f,.6f,2.25f,0x5f796d);
 if(ab(s->y-y)>.18f||s->wait>0)box(x+1.05f,y+1.2f,z,.10f,2.4f,1.78f,0x929e96);box(x+1.16f,y+1.28f,z+.94f,.12f,.25f,.16f,0xd7ba77);box(x+1.1f,y-.04f,z,.60f,.08f,1.70f,0x99a697);}
 surfaceTex=0;lift_display202(x,s->y+2.05f,z-.915f,s->y,s->target);
}
static int weather_visible140(float x,float y,float z){
 /* Indoors, intersect the view ray with the facade. Only real window rectangles pass. */
 float dx=x-camx,dz=z-camz,dy=y-camy;
 int h=residence_at(camx,camz);if(h>=0&&camy<15){if(camx-hx[h]>4.65f)return 0;float boundary=hz[h]+(dz>0?4.5f:-4.5f);if(ab(dz)<.01f)return 0;float t=(boundary-camz)/dz;if(t<=0||t>=1)return 0;float xx=camx+dx*t-hx[h],yy=camy+dy*t;int f=(int)(yy/3);float local=yy-f*3;if(f<0||f>4||local<1.12f||local>2.50f)return 0;for(int k=0;k<3;k++)if(ab(xx-(-2.8f+k*2.8f))<.70f)return 1;return 0;}
 int wing=panel_wing(camx,camz);if(wing>=0&&wing<30&&camy<12){int p=wing/2,w=wing%2;float ox=panel_origin_x(p)+(w?7:-7),oz=panel_origin_z(p),boundary=oz+(dz>0?7.92f:-7.92f);if(ab(dz)<.01f)return 0;float t=(boundary-camz)/dz;if(t<=0||t>=1)return 0;float xx=camx+dx*t-ox,yy=camy+dy*t,local=yy-(int)(yy/3)*3;if(local<1.04f||local>2.5f||ab(xx)<2.3f)return 0;return 1;}
 return 1;
}
static void update140_draw(void){
 surfaceTex=&mat_metal;box(ATM_X,.95f,ATM_Z,1.0f,1.9f,.78f,0x42766d);surfaceTex=0;box(ATM_X,1.3f,ATM_Z-.401f,.65f,.49f,.025f,0x152d35);box(ATM_X,1.3f,ATM_Z-.42f,.5f,.32f,.02f,0x96bcaa);box(ATM_X,.86f,ATM_Z-.43f,.53f,.045f,.04f,0x303c3b);box(ATM_X,1.86f,ATM_Z-.42f,.8f,.12f,.03f,0xe0dbc1);
 river_draw141();courtyards_draw141();
 for(int n=0;n<envCount;n++)if(env[n].kind==4){EnvCollider*c=&env[n];if(dst(camx,camz,c->x,c->z)<viewdist){float sc=.75f+world_value(3000+c->id)*.55f;tree(c->x,c->z,sc,c->id%3==0);if(g.season==3){for(int k=0;k<4;k++)box(c->x+(world_value(5000+n*4+k)-.5f)*3,.015f,c->z+(world_value(5200+n*4+k)-.5f)*3,.45f,.025f,.3f,k%2?0xb8924e:0x957750);}else if(g.season==1)soft_mesh(c->x,.15f,c->z,.55f,.10f,.50f,0,0x798b5d);}}
 for(int n=0;n<envCount;n++)if(env[n].kind==6&&dst(camx,camz,env[n].x,env[n].z)<viewdist){float x=env[n].x,z=env[n].z;surfaceTex=&mat_wood;for(int k=-1;k<=1;k++)box(x,.48f,z+k*.23f,2.6f,.10f,.20f,env[n].id%2?0x947e58:0x788363);for(int k=0;k<3;k++)box(x,.82f+k*.15f,z+.35f,2.6f,.11f,.07f,0x958263);surfaceTex=0;for(int a=-1;a<=1;a+=2){box(x+a*.95f,.24f,z,.10f,.48f,.65f,0x4d645b);box(x+a*.95f,.72f,z+.34f,.07f,.82f,.08f,0x4d645b);}}
 /* Seeded roof equipment and hatch. Most of the perimeter is unguarded: watch your step. */
 for(int h=0;h<NH;h++){float x=hx[h],z=hz[h];if(dst(camx,camz,x,z)>viewdist)continue;surfaceTex=&mat_roof;box(x+2,15.55f,z-2,1.2f,.8f,1.1f,0x707f7c);surfaceTex=0;beam(vp(x-2,15.15f,z+1),vp(x-2,17.4f,z+1),.045f,.02f,0x5c716f);beam(vp(x-3,17.1f,z+1),vp(x-1,17.1f,z+1),.02f,.02f,0x6d807a);}
 surfaceTex=0;
}
