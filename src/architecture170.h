/* OKNO v170: original modular, inhabited late-Soviet architecture.
   Detail is attached to existing solid walls, never floating across passageways. */
static void radiator170(float x,float y,float z){
 surfaceTex=0;
 for(int k=0;k<7;k++){float xx=x+(k-3)*.085f;box(xx,y+.50f,z,.065f,.57f,.135f,0xa3aaa0);box(xx,y+.78f,z,.08f,.055f,.14f,0xaeb2a5);}
 beam(vp(x-.38f,y+.73f,z+.05f),vp(x+.40f,y+.73f,z+.05f),.02f,.02f,0x8d978d);
 beam(vp(x-.38f,y+.23f,z+.05f),vp(x+.40f,y+.23f,z+.05f),.02f,.02f,0x8d978d);
}
static void lived_flat170(float x,float z,float y,int sign){
 /* Keep the centre of the room completely clear; detail lives in the envelope. */
 surfaceTex=0;
 box(x+sign*2.4f,y+2.84f,z+1.25f,4.55f,.065f,6.25f,0xb9beb3);
 /* Separate sill and radiator in the rear corner, below glass and away from the balcony door. */
 radiator170(x+sign*4.12f,y,z+4.02f);
 /* Small kitchen ventilation grille, with actual slats. */
 box(x+sign*.13f,y+2.46f,z-.70f,.04f,.32f,.40f,0x747d75);
 for(int k=0;k<5;k++)box(x+sign*.16f,y+2.36f+k*.045f,z-.70f,.025f,.016f,.32f,0x3e4845);
 /* Faded framed print; doesn't obstruct the corridor or interaction point. */
 box(x+sign*4.64f,y+1.80f,z+2.85f,.035f,.65f,.80f,0x665f53);
 box(x+sign*4.61f,y+1.80f,z+2.85f,.025f,.55f,.70f,0x9da89b);
 box(x+sign*4.59f,y+1.68f,z+2.85f,.02f,.15f,.65f,0x697e78);
 surfaceTex=0;
}
static void entry_lamp170(float x,float y,float z,int lit){
 surfaceTex=0;box(x,y,z,.32f,.21f,.09f,0x4d5854);box(x,y,z-.062f,.25f,.14f,.045f,lit?0xc5b084:0x9eaa9d);
 for(int k=-1;k<=1;k++)box(x+k*.082f,y,z-.09f,.014f,.19f,.014f,0x59645d);
 if(lit)glow150(vp(x-.10f,y-.05f,z-.091f),vp(x+.10f,y-.05f,z-.091f),vp(x+.10f,y+.05f,z-.091f),vp(x-.10f,y+.05f,z-.091f),0xc5b084);
}
static void residence_skin170(int h){float x=hx[h],z=hz[h];if(dst(camx,camz,x,z)>viewdist)return;
 surfaceTex=&mat_concrete;
 /* Recessed panel bays, narrow joints, a darker service plinth. */
 for(int s=-1;s<=1;s+=2){
  box(x, .30f,z+s*4.625f,9.85f,.58f,.045f,0x707a75);
  for(int f=1;f<5;f++)box(x,f*3,z+s*4.635f,9.85f,.038f,.03f,0x606c68);
  for(int n=-1;n<=1;n++)box(x+n*1.4f,7.5f,z+s*4.64f,.025f,15,.025f,0x7e8983);
  /* Repairs only on solid parapets: no new fake holes or visual obstruction. */
  for(int f=0;f<5;f++){int k=(h+f)%3;box(x-3.8f+k*3.3f,f*3+.48f,z+s*4.65f,.55f,.30f,.015f,0x8b938b);}
 }
 surfaceTex=0;
 /* Entrance canopy is deep enough to read as a real volume. */
 box(x+7.5f,2.73f,z-8.65f,1.9f,.14f,.95f,0x697570);
 entry_lamp170(x+7.50f,2.48f,z-8.55f,g.hours<9||g.hours>16);
 /* Small intercom and mailboxes, fixed against the service core wall. */
 box(x+8.13f,1.30f,z-8.53f,.13f,.30f,.055f,0x4f5957);
 for(int a=0;a<2;a++)for(int b=0;b<3;b++){float yy=1.05f+b*.23f;box(x+8.17f,yy,z-3.9f+a*.35f,.035f,.20f,.31f,0x737e75);box(x+8.14f,yy+.035f,z-3.9f+a*.35f,.018f,.018f,.19f,0x333e3a);}
 surfaceTex=0;
}
static void district_plan170(void){
 /* Continuous paved pedestrian connections. Existing seed layout and building IDs survive. */
 surfaceTex=&mat_pavement;
 box(-6,.004f,-40,91,.008f,2.4f,0x9b9f96);box(-6,.004f,8.5f,91,.008f,2.4f,0x9b9f96);
 box(-51,.004f,-15.7f,2.4f,.008f,51,0x9b9f96);box(39,.004f,-15.7f,2.4f,.008f,51,0x9b9f96);
 /* Sidewalks accompany the access roads, not random coloured patches. */
 box(-5,.007f,-72.4f,162,.014f,1.8f,0x9a9d94);box(-5,.007f,68.4f,162,.014f,1.8f,0x9a9d94);
 surfaceTex=0;
 for(int h=0;h<NH;h++)residence_skin170(h);
}
