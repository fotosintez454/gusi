#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d.save";eventpath="build/d.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 g.x=58;g.z=-33;g.playerY=0;screen=0;aim_at(58,1.2f,-36);
 printf("direct pick=%d sel=%d prompt=[%s]\n",s205_pick(),s205_selected,prompt);
 interaction();printf("after interaction prompt=[%s] sel=%d\n",prompt,s205_selected);
 g.x=44;g.z=17;aim_at(44,1.2f,20);interaction();printf("music prompt=[%s] sel=%d pick=%d\n",prompt,s205_selected,s205_pick());
 g.x=52;g.z=-32;aim_at(52,1.2f,-34.5f);interaction();printf("baba prompt=[%s] sel=%d pick=%d\n",prompt,s205_selected,s205_pick());
 g.x=hx[0]+3.2f;g.z=hz[0]-1.6f;aim_at(hx[0]+3.2f,1.5f,hz[0]-4.62f);interaction();printf("board prompt=[%s] sel=%d pick=%d hx%.1f hz%.1f\n",prompt,s205_selected,s205_pick(),hx[0],hz[0]);
 return 0;}
