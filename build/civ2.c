#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/dh.save";eventpath="build/dh.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 g.x=52;g.z=-32.0f;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);
 int r=civic_use190(0);printf("civic=%d [%.20s]\n",r,prompt);prompt[0]=0;
 r=update140_use(0);printf("upd140=%d [%.20s]\n",r,prompt);prompt[0]=0;
 r=high_use180(0);printf("high=%d\n",r);prompt[0]=0;
 r=panel_prompt();printf("panel=%d [%.20s]\n",r,prompt);prompt[0]=0;
 r=street_prompt205();printf("street=%d [%.20s] sel=%d\n",r,prompt,s205_selected);
 return 0;}
