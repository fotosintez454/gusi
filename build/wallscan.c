#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/w.save";eventpath="build/w.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 float x=hx[0]+3.2f;for(float z=-19;z>=-34;z-=.5f)printf("z%.1f %d\n",z,building_solid(x,z));
 printf("hx %.1f hz %.1f\n",hx[0],hz[0]);return 0;}
