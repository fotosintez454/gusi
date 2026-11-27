#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d8.save";eventpath="build/d8.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 aim205x: ;
 g.x=52;g.z=-32.2f;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);interaction();printf("early baba prompt=[%s]\n",prompt);
 return 0;}
