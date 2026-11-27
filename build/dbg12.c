#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/dc.save";eventpath="build/dc.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 struct{float x,z,tx,ty,tz;}st[]={{58,-34.1f,58,1.2f,-36},{-56,-11.6f,-56,1.5f,-9.2f},{36,-12.6f,36,1.4f,-10.5f},{36,-12.6f,36,1.4f,-10.5f},{36,-12.6f,36,1.4f,-10.5f},{-14,-32.9f,-14,1.0f,-35},{-38,-37.1f,-38,1.0f,-39},{28,-35.1f,28,.4f,-37},{28,-35.1f,28,.4f,-37},{20,-36.1f,20,.6f,-38},{20,-36.1f,20,.6f,-38},{0,-43.2f,0,.8f,-41},{44,18,44,1.2f,20},{52,-32.0f,52,1.2f,-34.5f}};
 for(int i=0;i<14;i++){g.x=st[i].x;g.z=st[i].z;g.playerY=0;screen=0;aim_at(st[i].tx,st[i].ty,st[i].tz);interaction();printf("%2d sel=%2d prompt=[%.24s]\n",i,s205_selected,prompt);street_use205(1);}
 return 0;}
