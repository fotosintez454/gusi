#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int n205,b205;
#define T205(c,m) do{n205++;if(!(c)){b205++;printf("FAIL %s line%d\n",m,__LINE__);fflush(stdout);}}while(0)
static void aim205(float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;screen=0;aim_at(tx,ty,tz);interaction();}
static int use205(const char*msg){int r=street_use205(1);T205(r,msg);s205_selected=-1;return r;}
int main(void){
 savepath="build/qb.save";eventpath="build/qb.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 aim205(58,-34.1f,58,1.2f,-36);use205("kiosk");
 aim205(-56,-11.6f,-56,1.5f,-9.2f);use205("phone");
 aim205(36,-12.6f,36,1.4f,-10.5f);use205("tear1");aim205(36,-12.6f,36,1.4f,-10.5f);use205("tear2");aim205(36,-12.6f,36,1.4f,-10.5f);use205("tear3");
 aim205(-14,-32.9f,-14,1.0f,-35);use205("domino");
 aim205(-38,-37.1f,-38,1.0f,-39);use205("pump");
 aim205(28,-35.1f,28,.4f,-37);use205("stash");
 aim205(20,-36.1f,20,.6f,-38);use205("barrel summer");g.hours=20;
 aim205(20,-36.1f,20,.6f,-38);use205("barrel warm");
 aim205(0,-43.2f,0,.8f,-41);use205("kick");
 aim205(44,18.0f,44,1.2f,20);use205("listen");
 aim205(52,-32.0f,52,1.2f,-34.5f);printf("baba=[%.30s] sel=%d\n",prompt,s205_selected);
 return 0;}
