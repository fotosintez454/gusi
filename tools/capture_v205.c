#include <sys/stat.h>
#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
/* Контрольные кадры v0.20.5: живой двор. Сборка: gcc -O1 tools/capture_v205.c -lm -o build/cap205 */
static void cap(const char*name,float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;g.fallVelocity=0;jump180=0;screen=0;aim_at(tx,ty,tz);shot(name);printf("CAP %s\n",name);}
int main(void){savepath="/tmp/okno205_capture.save";eventpath="/tmp/okno205_capture.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.hours=13;g.weather=0;g.season=2;winterEnabled=0;realTime=10;showHints=0;noticeTime=0;
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
 cap("qa/v205/10_boards.ppm",-29.8f,-31.5f,hx[0]+4.3f,1.5f,hz[0]-4.95f);
 cap("qa/v205/11_garages.ppm",-97,-46,-97,1.2f,-41.2f);
 cap("qa/v205/12_dumpster.ppm",-95,-34.5f,-97.4f,1.0f,-38.2f);
 g.hours=23;
 cap("qa/v205/13_kiosk_night.ppm",50,-42,58,1.2f,-37.6f);
 {float qx,qz;quarter_shop(s205_shop247(),&qx,&qz);cap("qa/v205/14_shop247_night.ppm",qx-2.5f,qz-6.5f,qx,1.4f,qz-1.3f);}
 winterEnabled=1;st205.clock=12.5f;
 cap("qa/v205/15_winter_night.ppm",-8,-3.2f,60,4,-3);
 screen=1;shot("qa/v205/16_menu.ppm");screen=0;
 screen=2;uiMouseX=250;uiMouseY=186;shot("qa/v205/17_pause.ppm");uiMouseX=520;uiMouseY=172;shot("qa/v205/18_pause_slider.ppm");screen=0;
 screen=6;optionsTab=2;optionsRow=1;uiMouseX=200;uiMouseY=200;shot("qa/v205/19_options_sound.ppm");optionsTab=0;screen=0;
 return 0;}
