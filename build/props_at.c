#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/df.save";eventpath="build/df.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);props_init150();
 for(int n=0;n<propCount150;n++){Prop150*p=&props150[n];if(dst(p->x,p->z,52,-34.5f)<3.4f)printf("prop t%d %.1f %.1f r%.1f/%.1f\n",p->type,p->x,p->z,p->rx,p->rz);}
 for(int n=0;n<54;n++){float bx,bz;bench_position141(n,&bx,&bz);if(dst(bx,bz,52,-32)<6)printf("bench %d %.1f %.1f\n",n,bx,bz);}
 return 0;}
