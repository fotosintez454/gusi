#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/cl.save";eventpath="build/cl.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 for(int k=0;k<POI205_COUNT;k++){Poi205*p=(Poi205*)&poi205[k];float sx=p->type==S205_GOAL?p->x-1.5f:p->x;printf("t%d %.1f %.1f solid=%d\n",p->type,sx,p->z,solid(sx,p->z));}
 return 0;}
