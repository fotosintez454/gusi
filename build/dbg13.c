#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/dd.save";eventpath="build/dd.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 g.x=58;g.z=-34.1f;aim_at(58,1.2f,-36);interaction();street_use205(1);
 g.x=-56;g.z=-11.6f;aim_at(-56,1.5f,-9.2f);interaction();street_use205(1);
 g.x=36;g.z=-12.6f;aim_at(36,1.4f,-10.5f);interaction();for(int k=0;k<3;k++){aim_at(36,1.4f,-10.5f);street_use205(1);}
 g.x=-14;g.z=-32.9f;aim_at(-14,1.0f,-35);interaction();street_use205(1);
 g.x=-38;g.z=-37.1f;aim_at(-38,1.0f,-39);interaction();street_use205(1);
 g.x=28;g.z=-35.1f;aim_at(28,.4f,-37);interaction();street_use205(1);street_use205(1);
 g.x=20;g.z=-36.1f;aim_at(20,.6f,-38);interaction();street_use205(1);g.hours=20;
 g.x=20;g.z=-36.1f;aim_at(20,.6f,-38);interaction();street_use205(1);
 g.x=0;g.z=-43.2f;aim_at(0,.8f,-41);interaction();street_use205(1);
 g.x=44;g.z=18;aim_at(44,1.2f,20);interaction();street_use205(1);
 g.x=52;g.z=-32.0f;g.playerY=0;screen=0;aim_at(52,1.2f,-34.5f);
 printf("npc=%d civic=%d upd140=%d high=%d panel=%d prop=%d use150=%d\n",npc_target200(),civic_use190(0),update140_use(0),high_use180(0),panel_prompt(),prop_use150(0),use150(0));
 aim_at(52,1.2f,-34.5f);interaction();printf("baba=[%.30s] sel=%d\n",prompt,s205_selected);
 return 0;}
