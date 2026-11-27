#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d6.save";eventpath="build/d6.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 float zs[]={-31.5f,-32.0f,-32.4f,-33.0f};
 for(int i=0;i<4;i++){g.x=52;g.z=zs[i];g.playerY=0;aim_at(52,1.2f,-34.5f);interaction();printf("z=%.1f prompt=[%s] pick=%d\n",zs[i],prompt,s205_pick());}
 return 0;}
