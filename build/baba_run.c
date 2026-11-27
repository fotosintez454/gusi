#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void aimx(float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;screen=0;aim_at(tx,ty,tz);interaction();}
int main(void){savepath="build/di.save";eventpath="build/di.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 int m;int b0;
 aimx(58,-34.1f,58,1.2f,-36);m=g.money;b0=g.beer;street_use205(1);printf("kiosk beer %d->%d\n",b0,g.beer);
 aimx(-56,-11.6f,-56,1.5f,-9.2f);street_use205(1);
 for(int k=0;k<3;k++){aimx(36,-12.6f,36,1.4f,-10.5f);street_use205(1);}
 printf("posters=%d\n",st205.postersTorn);
 aimx(-14,-32.9f,-14,1.0f,-35);m=g.money;street_use205(1);printf("domino money %d->%d\n",m,g.money);
 aimx(-38,-37.1f,-38,1.0f,-39);street_use205(1);
 aimx(28,-35.1f,28,.4f,-37);street_use205(1);m=g.money;aimx(28,-35.1f,28,.4f,-37);street_use205(1);
 aimx(20,-36.1f,20,.6f,-38);street_use205(1);g.hours=20;
 aimx(20,-36.1f,20,.6f,-38);street_use205(1);
 aimx(0,-43.2f,0,.8f,-41);street_use205(1);
 aimx(44,18.0f,44,1.2f,20);m=g.money;street_use205(1);printf("music money %d->%d\n",m,g.money);
 aimx(52,-32.0f,52,1.2f,-34.5f);printf("baba prompt=[%.30s] sel=%d\n",prompt,s205_selected);
 return 0;}
