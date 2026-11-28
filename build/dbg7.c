#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d7.save";eventpath="build/d7.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=20;g.day=4;
 g.x=20;g.z=-36.1f;g.playerY=0;screen=0;aim_at(20,.6f,-38);street_use205(1);
 printf("after barrel: hours=%.1f energy=%.1f\n",g.hours,g.energy);
 g.x=44;g.z=18;aim_at(44,1.2f,20);interaction();printf("music prompt=[%s]\n",prompt);
 g.x=52;g.z=-32.2f;aim_at(52,1.2f,-34.5f);interaction();printf("baba prompt=[%s] sel=%d\n",prompt,s205_selected);
 return 0;}
