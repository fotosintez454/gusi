#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/db.save";eventpath="build/db.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 for(float z=-31.8f;z>=-33.2f;z-=.2f){g.x=52;g.z=z;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);interaction();printf("z=%.1f sel=%d prompt=[%.30s]\n",z,s205_selected,prompt);}
 return 0;}
