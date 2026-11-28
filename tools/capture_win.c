#include <sys/stat.h>
#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
/* Контрольные кадры фасадов: окна, створки, занавески. */
static void cap(const char*name,float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;g.fallVelocity=0;jump180=0;screen=0;aim_at(tx,ty,tz);shot(name);printf("CAP %s\n",name);}
int main(void){savepath="/tmp/okno_win_capture.save";eventpath="/tmp/okno_win_capture.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.hours=13;g.weather=0;g.season=2;winterEnabled=0;realTime=10;showHints=0;noticeTime=0;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}local_init190();for(int i=0;i<LOCAL190;i++){g.locals190[i].x=185;g.locals190[i].z=135;g.locals190[i].y=39;}
 mkdir("qa/win",0755);
 /* часть окон открыта, чтобы проверить створки на фасаде */
 for(int h=0;h<NH;h++){g.residence[h].windows=0xffffff;for(int k=0;k<24;k++)residenceWindowAngle[h][k]=1.2f;}
 for(int p=0;p<PANEL_COUNT;p++){g.district[p].windows=0xffffffffffull;for(int k=0;k<40;k++)panelWindowAngle[p][k]=1.25f;}
 cap("qa/win/01_residence_side.ppm",hx[0]-13.f,hz[0]-10.f,hx[0],3.2f,hz[0]-4.48f);
 cap("qa/win/02_residence_close.ppm",hx[0]+2.f,hz[0]-9.f,hx[0]+2.f,3.0f,hz[0]-4.48f);
 cap("qa/win/03_panel_front.ppm",PANEL_X,PANEL_Z+22.f,PANEL_X,4.5f,PANEL_Z+7.97f);
 g.hours=21;cap("qa/win/04_residence_night.ppm",hx[0]+2.f,hz[0]-9.f,hx[0]+2.f,3.0f,hz[0]-4.48f);
 return 0;}
