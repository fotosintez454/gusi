#include <sys/stat.h>
#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void cap(const char*name,float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;g.fallVelocity=0;jump180=0;screen=0;aim_at(tx,ty,tz);shot(name);printf("CAP %s\n",name);}
int main(void){savepath="build/c205.save";eventpath="build/c205.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=13;g.weather=0;g.season=2;winterEnabled=0;realTime=10;showHints=0;noticeTime=0;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}local_init190();for(int i=0;i<LOCAL190;i++){g.locals190[i].x=185;g.locals190[i].z=135;g.locals190[i].y=39;}
 mkdir("qa/v205",0755);
 cap("qa/v205/01_kiosk.ppm",50,-42,58,1.6f,-36);
 cap("qa/v205/02_domino.ppm",-14,-30.5f,-14,1.0f,-35);
 cap("qa/v205/03_poster_wall.ppm",36,-17,36,1.4f,-10.5f);
 cap("qa/v205/04_phone.ppm",-56,-16.5f,-56,1.5f,-9.2f);
 cap("qa/v205/05_barrel_fire.ppm",15,-33,20,.6f,-38);
 cap("qa/v205/06_wires_street.ppm",-8,-3.2f,60,4,-3);
 cap("qa/v205/07_goal.ppm",-3,-47,0,.8f,-41);
 cap("qa/v205/08_musician.ppm",44,12.5f,44,1.2f,20);
 cap("qa/v205/09_pump_stash.ppm",-40,-42,-38,1.0f,-39);
 cap("qa/v205/10_boards.ppm",-29.8f,-31.5f,-26.6f,1.5f,-28.3f);
 return 0;}
