#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/p2.save";eventpath="build/p2.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);props_init150();
 for(int n=0;n<54;n+=3){float a,b;bench_position141(n,&a,&b);printf("bench %d %.1f %.1f\n",n,a,b);}
 printf("--- free grid front houses (z -31..-41)\n");
 for(float z=-31;z>=-41;z-=2){printf("z%.0f:",z);for(float x=-40;x<=40;x+=2)printf("%c",solid(x,z)?'#':'.');printf("\n");}
 printf("shop %0.1f %0.1f KX %.1f %.1f garages?\n",shopX,shopZ,KX,KZ);
 return 0;}
