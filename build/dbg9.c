#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d9.save";eventpath="build/d9.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 g.x=58;g.z=-34.1f;g.playerY=0;screen=0;aim_at(58,1.2f,-36);interaction();printf("kiosk=[%s]\n",prompt);
 street_use205(1);printf("beer=%d money=%d\n",g.beer,g.money);
 g.x=52;g.z=-32.2f;aim_at(52,1.2f,-34.5f);interaction();printf("baba=[%s] sel=%d\n",prompt,s205_selected);
 printf("dst=%.2f\n",dst(g.x,g.z,52,-34.5f));
 return 0;}
