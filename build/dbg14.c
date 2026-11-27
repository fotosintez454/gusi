#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/de.save";eventpath="build/de.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 for(int i=0;i<12;i++){g.x=52;g.z=-32.0f;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);interaction();printf("i%d sel=%d [%.20s]\n",i,s205_selected,prompt);street_use205(1);}
 return 0;}
