#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/dg.save";eventpath="build/dg.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 g.x=52;g.z=-32.0f;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);
 int r=physics_prompt200();printf("phys=%d [%.20s]\n",r,prompt);prompt[0]=0;
 r=extra_use200(0);printf("extra=%d [%.20s]\n",r,prompt);prompt[0]=0;
 r=npc_target200();printf("npc=%d [%.20s]\n",r,prompt);
 return 0;}
