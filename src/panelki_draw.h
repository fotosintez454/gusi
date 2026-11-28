/* Original modular concrete reference block; not an imported art asset. */
static void panel_slab(float x,float y,float z,float w,float d){floor203(x,y,z,w,d);}
static void panel_door(float x,float y,float z,float open,int side){surfaceTex=0;float angle=open*1.45f;V3 a=vp(x,y,z-.9f),b=vp(x+side*(float)sin(angle)*1.8f,y,z-.9f+(float)cos(angle)*1.8f),c=vp(b.x,y+2.3f,b.z),d=vp(a.x,y+2.3f,a.z);quadstrip(a,b,c,d,0x756b5a);beam(a,d,.045f,.045f,0x404b48);beam(b,c,.045f,.045f,0x404b48);V3 h=blend3(b,c,.5f);box(h.x-side*.07f,h.y,h.z-.08f,.10f,.07f,.10f,0xd1c6a8);box(x,y+2.4f,z,.22f,.16f,2.05f,0x737e6e);}
static void panel_window_draw(int w,int f,int k){float x=panel_cx(w)+(k<2?-1:1)*(k%2?6:4),y=f*3+1.02f,z=PANEL_Z+7.97f;int id=w*PANEL_FLOORS*4+f*4+k;float a=panelWindowAngle[panelIndex][id];
 /* Наружные створки всегда в плоскости фасада: открытая створка
    торчала из стены чёрным клином и ломала вид дома. */
 (void)a;a=0;
 V3 p=vp(x-.86f,y,z),q=vp(p.x+1.72f*(float)cos(a),y,z-1.72f*(float)sin(a)),rr=vp(q.x,y+1.6f,q.z),ss=vp(p.x,y+1.6f,p.z);surfaceTex=0;glass150(p,q,rr,ss,NH+panelIndex,f,w*8+k);beam(p,q,.035f,.035f,0xbac3bf);beam(q,rr,.04f,.04f,0xbac3bf);beam(rr,ss,.035f,.035f,0xbac3bf);beam(ss,p,.04f,.04f,0xbac3bf);
 box(x,y+1.66f,z-.09f,1.72f,.09f,.05f,0x9a9c92);box(x,y-.04f,z-.09f,1.72f,.09f,.05f,0x9a9c92);
 box(x-.88f,y+.8f,z-.09f,.08f,1.66f,.05f,0x9a9c92);box(x+.88f,y+.8f,z-.09f,.08f,1.66f,.05f,0x9a9c92);V3 h=blend3(q,rr,.48f);box(h.x-.06f,h.y,h.z-.06f,.1f,.16f,.05f,0xe1ddd0);}

static void panel_one_draw(void){if(dst(camx,camz,PANEL_X,PANEL_Z)>viewdist+25||!building_visible203(PANEL_X,PANEL_Z,28.4f,15.1f,19.5f))return;
 for(int w=0;w<2;w++){float x=panel_cx(w),z=PANEL_Z;U32 facade151=(U32[]){0xb4b8ac,0x969e97,0x9e9a88,0x87928f}[hash32(g.worldSeed+panelIndex*23+w)%4];
 for(int f=0;f<PANEL_FLOORS;f++){float y=f*3;if(f==0)floor203(x,0,z,13.8f,15.8f);int interior=dst(camx,camz,x,z)<24&&ab(f-(int)(g.playerY/3+.1f))<=1;
 /* Slabs have real holes for both stair flights and the utility riser. */
 {floor203(x+4.35f,y,z,5.1f,15.8f);floor_hole203(x-4.35f,y,z,5.1f,15.8f,x-3.5f,z-6,2.2f,2.1f);panel_slab(x,y,z-3,3.6f,10);
 panel_slab(x-5.85f,y,z-6,1.7f,4);panel_slab(x-2.02f,y,z-6,.25f,4);panel_slab(x-3.5f,y,z-7.7f,3,0.5f);
 }surfaceTex=&mat_concrete;
 box(x-6.9f,y+1.5f,z,.2f,3,16,0xb5b6a7);box(x+6.9f,y+1.5f,z,.2f,3,16,0xb5b6a7);
 /* Rear panels have separate glass window recesses. */
 box(x,y+.49f,z+7.9f,13.8f,.98f,.20f,facade151);box(x,y+2.81f,z+7.9f,13.8f,.38f,.20f,facade151);
 {float edge=-7;float win[4]={-6,-4,4,6};for(int k=0;k<4;k++){float lo=win[k]-.825f;box(x+(edge+lo)*.5f,y+1.8f,z+7.9f,lo-edge,1.64f,.20f,facade151);edge=win[k]+.825f;}box(x+(edge+7)*.5f,y+1.8f,z+7.9f,7-edge,1.64f,.20f,facade151);}

 for(int side=-1;side<=1;side+=2){
  box(x+side*1.8f,y+1.5f,z-2.9f,.16f,3,3.9f,0x7b8b7c);box(x+side*1.8f,y+1.5f,z+4.3f,.16f,3,6.6f,0x7b8b7c);box(x+side*1.8f,y+2.72f,z,.16f,.56f,2,0xd0cdb7);
  box(x+side*4.4f,y+1.5f,z-5.0f,5.1f,3,.18f,0x8d9987);
  box(x+side*4.6f,y+.45f,z-7.9f,4.3f,.9f,.2f,0xa5aa9d);box(x+side*4.6f,y+2.8f,z-7.9f,4.3f,.4f,.2f,0xa5aa9d);
  for(int n=0;n<3;n++)box(x+side*(2.6f+n*2),y+1.75f,z-7.9f,.18f,1.70f,.22f,0xb6b9a7);box(x+side*2.245f,y+1.75f,z-7.9f,.89f,1.70f,.22f,0xb6b9a7);box(x+side*6.73f,y+1.75f,z-7.9f,.18f,1.70f,.22f,0xb6b9a7);
  surfaceTex=0;for(int n=0;n<2;n++){float wx=x+side*(3.6f+n*2);glass150(vp(wx-.91f,y+.90f,z-7.94f),vp(wx+.91f,y+.90f,z-7.94f),vp(wx+.91f,y+2.60f,z-7.94f),vp(wx-.91f,y+2.60f,z-7.94f),NH+panelIndex,f,20+w*4+(side>0)*2+n);box(wx,y+1.72f,z-8.0f,.07f,1.58f,.055f,0xc8c4ad);box(wx,y+.92f,z-8.05f,1.9f,.1f,.22f,0xb8bcaa);}surfaceTex=&mat_concrete;
 }
 if(f>0){box(x,y+.45f,z-7.9f,4.6f,.9f,.2f,0xb1b5a7);box(x,y+2.8f,z-7.9f,4.6f,.4f,.2f,0xb1b5a7);box(x-1.8f,y+1.72f,z-7.9f,1.0f,1.65f,.2f,0xb1b5a7);box(x+1.8f,y+1.72f,z-7.9f,1.0f,1.65f,.2f,0xb1b5a7);surfaceTex=0;public_window200(x,y+.92f,z-7.93f,2.5f,1.6f,!!(g.stairWindows200[NH+panelIndex]&(1u<<(w*4+f))));surfaceTex=&mat_concrete;}else{box(x-1.42f,1.35f,z-8,.18f,2.7f,.24f,0x53645c);box(x+1.42f,1.35f,z-8,.18f,2.7f,.24f,0x53645c);}
 surfaceTex=0;for(int k=0;k<4;k++)panel_window_draw(w,f,k);
 if(interior){
 for(int side=-1;side<=1;side+=2){int bit=w*(PANEL_FLOORS*2)+f*2+(side>0);panel_door(x+side*1.8f,y,z,flatAngles[bit],side);
  /* A real mirrored apartment plan: corridor -> kitchen/wet room -> bedroom. */
  art_flat(x,z,y,side,w*8+f*2+(side>0));
  surfaceTex=&art_wallpaper;U32 wall=(f+w)%2?0x9d9d85:0x9da697;
  box(x+side*2.40f,y+1.5f,z+2.3f,.90f,3.0f,.18f,wall);box(x+side*5.66f,y+1.5f,z+2.3f,2.02f,3.0f,.18f,wall);box(x+side*3.75f,y+2.7f,z+2.3f,1.8f,.6f,.18f,wall);
  box(x+side*3.85f,y+1.5f,z-4.25f,.18f,3.0f,1.3f,wall);box(x+side*3.85f,y+1.5f,z-1.75f,.18f,3.0f,1.2f,wall);box(x+side*3.85f,y+2.7f,z-3,.18f,.6f,1.3f,wall);box(x+side*2.82f,y+1.5f,z-1.15f,2.06f,3.0f,.18f,wall);surfaceTex=0;
  surfaceTex=&mat_wood;box(x+side*5.1f,y+.37f,z+5.2f,1.45f,.65f,2.15f,0x756d56);surfaceTex=0;box(x+side*5.1f,y+.77f,z+5.2f,1.47f,.18f,2.15f,0x7a8270);box(x+side*5.1f,y+.9f,z+5.9f,1.10f,.14f,.50f,0xaaa98f);
  if(panel_bath_closed(w,f,side)){box(x+side*3.85f,y+1.12f,z-3,.065f,2.24f,1.28f,0xb3a68b);box(x+side*3.92f,y+1.02f,z-2.58f,.10f,.07f,.10f,0x766953);}else{box(x+side*3.94f,y+1.12f,z-4.30f,.065f,2.24f,1.26f,0xb3a68b);box(x+side*3.99f,y+1.02f,z-4.75f,.10f,.07f,.10f,0x766953);}
  float tx=x+side*2.75f,tz=z-4.05f;box(tx,y+.24f,tz,.3f,.48f,.4f,0xb5b8a4);soft_mesh(tx,y+.54f,tz,.28f,.17f,.36f,0,0xc9cbb9);soft_mesh(tx,y+.65f,tz,.23f,.04f,.29f,0,0x666f61);box(tx,y+.76f,tz-.28f,.46f,.65f,.17f,0xb8beac);soft_mesh(x+side*2.48f,y+.82f,z-1.52f,.34f,.10f,.24f,0,0xc2c9b9);
  for(int n=0;n<5;n++)box(x+side*4.6f+n*.12f,y+.52f,z+7.5f,.08f,.65f,.18f,0xc0b9a1);
 }
 if(f<PANEL_FLOORS){for(int n=0;n<16;n++){float yy=y+(n+1)*1.5f/16,zz=z+2+(n+.5f)*4/16;surfaceTex=&mat_concrete;floor_record203(x-.82f,yy,zz,1.35f,.25f);box(x-.82f,yy-.09f,zz,1.35f,.18f,4.f/16,0xabb0a2);floor_record203(x+.82f,y+3-n*1.5f/16,zz,1.35f,.25f);box(x+.82f,y+3-(n)*1.5f/16-.09f,zz,1.35f,.18f,4.f/16,0xabb0a2);surfaceTex=0;}panel_slab(x,y+1.5f,z+6.85f,3.1f,1.7f);
 stair_rail203(x-.18f,z+2.125f,z+5.875f,y+.09375f,y+1.5f);stair_rail203(x+.18f,z+2.125f,z+5.875f,y+3,y+1.59375f);}
 }
 /* Landing lights, warm Soviet tile band, floor numeral bars. */
 box(x,y+2.7f,z-2.5f,.5f,.12f,.4f,0xe2ca82);for(int n=0;n<=f;n++)box(x+1.67f,y+1.6f+n*.13f,z-2.2f,.035f,.08f,.45f,0xe2d7a5);
 }
 surfaceTex=&mat_roof;box(x-4.4f,12.02f,z,5.2f,.20f,16.2f,0x7f8580);box(x+4.4f,12.02f,z,5.2f,.20f,16.2f,0x7f8580);box(x,12.02f,z-3.05f,3.6f,.20f,10.1f,0x7f8580);surfaceTex=0;
 /* Stair headhouse: door from top landing to roof, actual opening above flights. */
 for(int side=-1;side<=1;side+=2)box(x+side*1.7f,13.4f,z+4.25f,.18f,2.8f,7.4f,0x9baba2);
 box(x,13.4f,z+7.9f,3.6f,2.8f,.18f,0x9baba2);box(x,14.85f,z+4.25f,3.6f,.18f,7.5f,0x707e78);
 box(x-1.25f,13.4f,z+.6f,.9f,2.8f,.18f,0x9baba2);box(x+1.25f,13.4f,z+.6f,.9f,2.8f,.18f,0x9baba2);box(x,14.65f,z+.6f,1.6f,.3f,.18f,0x9baba2);
 /* Weathered top cornice, rain pipes and varied balcony enclosures. */
 box(x,PANEL_FLOORS*3-.22f,z-8.08f,14.0f,.30f,.20f,0xb4afa0);
 for(int s=-1;s<=1;s+=2){box(x+s*6.75f,PANEL_FLOORS*1.5f,z-8.13f,.12f,PANEL_FLOORS*3,.12f,0x777d76);
 for(int f=1;f<PANEL_FLOORS;f++){float yy=f*3;box(x+s*5.0f,yy-.02f,z+8.43f,2.55f,.14f,1.15f,0x9a9b8d);box(x+s*5.0f,yy+.53f,z+8.96f,2.55f,1.0f,.09f,(f+w+panelIndex)%3?0x8b958e:0x9f9180);for(int e=-1;e<=1;e+=2)box(x+s*5.0f+e*1.23f,yy+.53f,z+8.44f,.08f,1.0f,1.05f,0x858d85);box(x+s*5.0f,yy+1.07f,z+8.96f,2.6f,.06f,.08f,0xb3b5a7);}}
 /* End walls carry storey joints, a plinth and a patched top band. */
 for(int s=-1;s<=1;s+=2){box(x+s*7.015f,.35f,z,.035f,.7f,15.9f,0x778178);for(int f=1;f<PANEL_FLOORS;f++)box(x+s*7.015f,f*3,z,.035f,.045f,15.9f,0x8c9187);for(int k=-1;k<=1;k++)box(x+s*7.015f,PANEL_FLOORS*1.5f,z+k*5.2f,.035f,PANEL_FLOORS*3,.045f,0x8d9389);}
 /* Horizontal/vertical panel seams. */
 for(int f=0;f<=PANEL_FLOORS;f++)box(x,f*3,z-8.03f,14,.06f,.04f,0x596660);
 for(int n=0;n<7;n++)box(x-6+n*2,PANEL_FLOORS*1.5f,z-8.04f,.035f,PANEL_FLOORS*3,.04f,0x737c70);
 if(!(entryOpen&(1u<<w)))box(x,1.25f,z-8,2.5f,2.5f,.15f,0x476656);else box(x-1.26f,1.25f,z-7.0f,.12f,2.5f,2,0x476656);
 box(x,2.85f,z-8.5f,3.2f,.15f,1.5f,0x747c70);box(x,2.3f,z-8.09f,.32f,.37f,.04f,0xe3ca8a);if(w)box(x+.23f,2.3f,z-8.09f,.08f,.37f,.04f,0xe3ca8a);
 /* Moving cabin opens east into the public foyer, matching collision. */
 surfaceTex=&mat_metal;
 box(x-4.76f,6,z-6,.18f,12,2.5f,0x68736b);box(x-3.5f,6,z-7.22f,2.7f,12,.18f,0x68736b);box(x-3.5f,6,z-4.8f,2.7f,12,.18f,0x68736b);
 lift_display202(x-3.5f,liftY[w]+2.08f,z-7.10f,liftY[w],liftTarget[w]);box(x-3.5f,liftY[w]+.015f,z-6,2.28f,.10f,2.2f,0x929c91);box(x-3.5f,liftY[w]+2.55f,z-6,2.28f,.10f,2.2f,0x929c91);
 for(int f=0;f<PANEL_FLOORS;f++){float yy=f*3;box(x-2.24f,yy+1.2f,z-7.02f,.18f,2.4f,.35f,0x626f68);box(x-2.24f,yy+1.2f,z-4.98f,.18f,2.4f,.35f,0x626f68);box(x-2.24f,yy+2.7f,z-6,.18f,.6f,2.5f,0x626f68);
 if(!liftDoor[w]||ab(liftY[w]-yy)>.15f)box(x-2.24f,yy+1.2f,z-6,.10f,2.4f,1.7f,0x8c958d);
 box(x-2.10f,yy+1.25f,z-5.0f,.12f,.3f,.18f,0xdeb66a);}
 surfaceTex=0;

 }
 surfaceTex=0;
}
static void panel_draw(void){for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;float dx=PANEL_X-camx,dz=PANEL_Z-camz,forward=dx*(float)sin(yaw)+dz*(float)cos(yaw),side=dx*(float)cos(yaw)-dz*(float)sin(yaw);if(forward< -25||ab(side)>mx(0,forward)*1.4f+25)continue;panel_one_draw();}panelIndex=0;}
static void home_radio_draw(void){
 float x=HOME_RADIO_X,z=HOME_RADIO_Z,y=.98f;
 /* Small tabletop Soviet radio: compact enough to leave the table usable. */
 surfaceTex=&mat_wood;box(x,y,z,.78f,.30f,.30f,0x7c654d);surfaceTex=0;
 box(x,y,z-.158f,.72f,.25f,.018f,0x272e27);
 /* Speaker grille and tuning scale. */
 for(int n=0;n<7;n++)box(x-.25f+n*.035f,y,z-.172f,.009f,.19f,.008f,0x89907b);
 box(x+.18f,y+.045f,z-.172f,.23f,.045f,.009f,0xd4bf80);
 for(int n=0;n<5;n++)box(x+.095f+n*.042f,y+.045f,z-.182f,.008f,.028f,.006f,0x555b3c);
 for(int n=0;n<2;n++)soft_mesh(x+.13f+n*.14f,y-.075f,z-.174f,.027f,.027f,.018f,0,0xb4b4a0);
 /* Short telescopic aerial, not an oversized mast. */
 beam(vp(x+.29f,y+.13f,z+.04f),vp(x+.40f,y+.34f,z+.04f),.012f,.008f,0xbabda9);
 box(x+.40f,y+.34f,z+.04f,.025f,.025f,.025f,0xc7c7b9);
 box(x+.32f,y+.11f,z-.174f,.025f,.025f,.012f,g.radioOn?0xb4cc7b:0x65654c);
 if(!g.homeFeedTaken){box(hx[0]+1.4f,.24f,hz[0]-2.1f,.30f,.45f,.18f,0xc7b47e);box(hx[0]+1.4f,.28f,hz[0]-2.2f,.23f,.16f,.018f,0x628461);}
}
