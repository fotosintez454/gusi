#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void cap(const char*name,float x,float z,float y,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=y;g.fallVelocity=0;jump180=0;screen=0;aim_at(tx,ty,tz);shot(name);printf("CAP %s\n",name);}
int main(void){savepath="build/c205.save";eventpath="build/c205.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=13;g.weather=0;g.season=2;winterEnabled=0;realTime=10;showHints=0;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}local_init190();for(int i=0;i<LOCAL190;i++){g.locals190[i].x=185;g.locals190[i].z=135;g.locals190[i].y=39;}
 cap("build/shots/p_domino.ppm",-14,-30.5f,0,-14,1.0f,-35);
 cap("build/shots/p_kiosk.ppm",50,-42,0,58,1.6f,-36);
 cap("build/shots/p_wall.ppm",36,-17,0,36,1.4f,-10.5f);
 cap("build/shots/p_phone.ppm",-56,-16.5f,0,-56,1.5f,-9.2f);
 cap("build/shots/p_barrel.ppm",15,-33,0,20,.6f,-38);
 cap("build/shots/p_wires.ppm",-8,-3.2f,0,60,4,-3);
 cap("build/shots/p_goal.ppm",-3,-47,0,0,.8f,-41);
 cap("build/shots/p_music.ppm",44,12.5f,0,44,1.2f,20);
 return 0;}
