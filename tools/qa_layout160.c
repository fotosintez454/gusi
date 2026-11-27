#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main

static int checks160=0,fail160=0;
#define L160(v,msg) do{checks160++;if(!(v)){fail160++;printf("FAIL seed=%u: %s\n",g.worldSeed,msg);}}while(0)
static int overlap_aabb(float ax0,float az0,float ax1,float az1,float bx0,float bz0,float bx1,float bz1,float pad){return ax0<bx1+pad&&ax1>bx0-pad&&az0<bz1+pad&&az1>bz0-pad;}

int main(void){
 savepath="release_v160/layout.save";eventpath="release_v160/layout_events.csv";audio_init();
 test_reset();
 for(unsigned seed=1;seed<=48;seed++){
  generate_world(seed*7919u+17u);
  for(int h=0;h<NH;h++){
   float ex=(float[]){-30,-10,10,30,-30,-10,10,30,-77,-57,-77,-57,38,58,38,58,-77,-57,-77,-57,38,58,38,58}[h];
   float ez=(float[]){-24,-24,-24,-24,29,29,29,29,-64,-64,-88,-88,-64,-64,-88,-88,61,61,121,121,61,61,121,121}[h];
   L160(ab(hx[h]-ex)<=.601f&&ab(hz[h]-ez)<=.601f,"house variation stays inside its reserved cell");
   for(int j=0;j<h;j++){
    float ax0=hx[h]-5.25f,ax1=hx[h]+8.55f,az0=hz[h]-8.6f,az1=hz[h]+4.8f;
    float bx0=hx[j]-5.25f,bx1=hx[j]+8.55f,bz0=hz[j]-8.6f,bz1=hz[j]+4.8f;
    L160(!overlap_aabb(ax0,az0,ax1,az1,bx0,bz0,bx1,bz1,.05f),"residential building footprints do not overlap");
   }
   for(int p=0;p<PANEL_COUNT;p++){
    float px=panel_origin_x(p),pz=panel_origin_z(p);
    L160(!overlap_aabb(hx[h]-5.25f,hz[h]-8.6f,hx[h]+8.55f,hz[h]+4.8f,px-14.3f,pz-9.2f,px+14.3f,pz+9.2f,.05f),"small house does not intersect a long panel block");
   }
  }
  for(int p=0;p<PANEL_COUNT;p++)for(int q=0;q<p;q++){
   float px=panel_origin_x(p),pz=panel_origin_z(p),qx=panel_origin_x(q),qz=panel_origin_z(q);
   L160(!overlap_aabb(px-14.3f,pz-9.2f,px+14.3f,pz+9.2f,qx-14.3f,qz-9.2f,qx+14.3f,qz+9.2f,.05f),"long panel blocks have no overlapping footprints");
  }
  for(int c=0;c<4;c++){
   float cx=cityBlockX[c],cz=cityBlockZ[c];
   for(int h=0;h<NH;h++)L160(!overlap_aabb(cx-7.9f,cz-5.2f,cx+7.9f,cz+5.2f,hx[h]-5.25f,hz[h]-8.6f,hx[h]+8.55f,hz[h]+4.8f,.05f),"skyline block does not intersect a residence");
   for(int p=0;p<PANEL_COUNT;p++){float px=panel_origin_x(p),pz=panel_origin_z(p);L160(!overlap_aabb(cx-7.9f,cz-5.2f,cx+7.9f,cz+5.2f,px-14.3f,pz-9.2f,px+14.3f,pz+9.2f,.05f),"skyline block does not intersect a panel building");}
  }
  /* All main POIs remain on walkable land and outside the river channel. */
  L160(ab(shopZ-river_z(shopX))>6.8f,"shop remains on dry land");
 }
 printf("LAYOUT160: %d checks, %d failures across 48 generated worlds\n",checks160,fail160);
 return fail160?1:0;
}
