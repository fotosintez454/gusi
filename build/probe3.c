#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/p3.save";eventpath="build/p3.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);props_init150();
 float xs[]={58,-56,36,-14,-38,20,0,28,44,52,-26.6,-6.6,13.4,33.4,40};
 float zs[]={-36,-9.2f,-10.5f,-35,-39,-38,-41,-37,20,-34.5f,-26.6f,-26.6f,-26.6f,-26.6f,-33};
 for(int i=0;i<15;i++){printf("%2d %7.1f %7.1f solid=%d out=%d\n",i,xs[i],zs[i],solid(xs[i],zs[i]),outside141(xs[i],zs[i],2.5f));}
 // list shop/houses fronts
 printf("shop %.1f %.1f office %.1f %.1f\n",shopX,shopZ,OX,OZ);
 for(int h=0;h<8;h++)printf("house %d %.1f %.1f\n",h,hx[h],hz[h]);
 return 0;}
