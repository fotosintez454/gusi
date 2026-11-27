#include <sys/stat.h>
#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int n205,b205;
#define T205(c,m) do{n205++;if(!(c)){b205++;printf("FAIL205 %s line%d\n",m,__LINE__);fflush(stdout);}}while(0)
static void aim205(float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;screen=0;aim_at(tx,ty,tz);interaction();}
static int use205(const char*msg){int r=street_use205(1);T205(r,msg);s205_selected=-1;return r;}
int main(void){
 savepath="build/qa205.save";eventpath="build/qa205.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 g.winter=0;winterEnabled=0;g.weather=0;g.hours=13;noticeTime=0;g.day=4;g.stress=20;g.thirst=40;
 for(int n=0;n<NN;n++){PERSON(n).x=185;PERSON(n).z=135;}
 /* размещение: объект реально стоит на земле и до него можно дойти с подсказкой */
 for(int k=0;k<POI205_COUNT;k++){Poi205*p=(Poi205*)&poi205[k];
  float sx=p->type==S205_GOAL?p->x-1.5f:(p->type==S205_BABA?p->x-1.1f:p->x),sz=p->z;
  T205(solid(sx,sz),"poi has collision");
  float standx=p->x,standz=p->z+2.4f;
  T205(!solid(standx,standz),"poi approach walkable");
  g.x=standx;g.z=standz;g.playerY=0;screen=0;
  float eye=p->type==S205_STAS?.4f:p->type==S205_PUMP?1.0f:1.2f;
  aim_at(p->x,eye,p->z);int pick=s205_pick();
  T205(pick==k,"poi reachable and selected");
  for(int j=0;j<k;j++)T205(dst(p->x,p->z,poi205[j].x,poi205[j].z)>2.4f,"poi spacing");}
 /* ларёк: коллизия корпуса и покупка пива */
 T205(solid(58,-37.4f),"kiosk collides");
 T205(!solid(58,-31.5f),"kiosk front walkable");
 aim205(58,-34.1f,58,1.2f,-36);
 T205(strstr(prompt,"ПИВО")!=0,"kiosk prompt");
 int m=g.money,b0=g.beer;use205("kiosk buys beer");
 T205(g.money==m-25,"beer price");
 T205(g.beer==b0+1||g.beer>0,"beer added");
 /* таксофон: списание двушки */
 aim205(-56,-11.6f,-56,1.5f,-9.2f);
 T205(strstr(prompt,"таксофон")!=0,"phone prompt");
 m=g.money;int pu=st205.phoneUsed;use205("phone call");
 T205(g.money==m-2,"phone fee");T205(st205.phoneUsed==pu+1,"phone counted");
 /* афишная стена: сорвать три афиши */
 aim205(36,-12.6f,36,1.4f,-10.5f);
 T205(strstr(prompt,"афиш")!=0,"wall prompt");
 for(int k=0;k<3;k++){aim205(36,-12.6f,36,1.4f,-10.5f);use205("tear poster");}
 T205(st205.postersTorn==3,"posters torn");
 /* домино: ставка и исход */
 aim205(-14,-32.9f,-14,1.0f,-35);
 T205(strstr(prompt,"домино")!=0,"domino prompt");
 m=g.money;use205("domino round");
 T205((g.money==m+15)||(g.money==m-10),"domino stake paid");
 /* колонка: вода */
 aim205(-38,-37.1f,-38,1.0f,-39);
 int w0=g.water;use205("pump water");
 T205(g.water==w0+1||g.thirst>40,"pump fills water");
 /* клад: раз в день */
 aim205(28,-35.1f,28,.4f,-37);
 use205("stash dig");
 T205(st205.stashDay==g.day,"stash day");
 m=g.money;aim205(28,-35.1f,28,.4f,-37);use205("stash repeat");T205(g.money==m,"stash once a day");
 /* бочка: зимой горит, летом ищем бутылки */
 aim205(20,-36.1f,20,.6f,-38);
 T205(s205_fire_lit()==0,"barrel summer");
 use205("barrel summer");
 g.hours=20;T205(s205_fire_lit()==1,"barrel evening lit");
 aim205(20,-36.1f,20,.6f,-38);float e=g.energy;use205("barrel warm");T205(g.energy>=e,"barrel warms");
 /* ворота, музыкант, бабки, объявления */
 aim205(0,-43.2f,0,.8f,-41);T205(strstr(prompt,"мячу")!=0,"goal prompt");use205("ball kick");
 aim205(44,18.0f,44,1.2f,20);T205(strstr(prompt,"гитар")!=0,"music prompt");m=g.money;use205("listen");T205((g.money==m-5)||(g.money==m),"coin optional");
 aim205(52,-32.0f,52,1.2f,-34.5f);s205_selected=-1;interaction();printf("DBG baba prompt=[%.40s] sel=%d\n",prompt,s205_selected);T205(strstr(prompt,"бабо")!=0,"baba prompt");use205("gossip");
 aim205(hx[0]+4.3f,hz[0]-6.2f,hx[0]+4.3f,1.5f,hz[0]-4.95f);T205(strstr(prompt,"объявлен")!=0,"board prompt");use205("read board");
 /* расписание торговли: ларёк днём торгует, ночью закрыт */
 int h0=g.hours;
 g.hours=13;aim205(58,-38.6f,58,1.0f,-37.6f);T205(strstr(prompt,"пиво")!=0,"kiosk open at day");
 g.hours=23;aim205(58,-38.6f,58,1.0f,-37.6f);T205(strstr(prompt,"закрыт")!=0,"kiosk prompt closed at night");
 int m0=g.money,bn=g.beer;use205("kiosk night");T205(g.money==m0&&g.beer==bn,"kiosk sells nothing at night");
 T205(!shop_open205(-1)&&!shop_open205(0),"shops closed at 23");
 g.hours=12;T205(shop_open205(-1)&&shop_open205(0),"shops open at noon");
 g.hours=23;int s247=shop247_index();T205(shop_is247(s247)&&shop_open205(s247),"one shop works 24/7");
 T205(s247==s205_shop247(),"24/7 shop is farthest kiosk");
 g.hours=h0;
 /* сидящая поза без лавки не включается */
 {int a0=PERSON(35).activity;float s0=sitting141[35];PERSON(35).activity=3;s205_npc(58,-40,0,35,1,12);
  T205(PERSON(35).activity==3&&sitting141[35]==s0,"npc state restored after draw");PERSON(35).activity=a0;}
 T205(solid(-14-.95f,-35-.2f)&&solid(-14+.2f,-35+1.0f),"domino stools solid");
 /* ракушки и мусорные контейнеры: стоят и держат коллизию */
 T205(S205_GAR_N==6,"six garage sheds");
 for(int n=0;n<S205_GAR_N;n++)T205(solid(s205_gar[n][0],s205_gar[n][1]),"shed solid");
 T205(solid(-97.4f,-38.2f)&&solid(94.6f,-38.4f),"dumpsters solid");
 T205(S205_TRASH_N>=24,"trash scatter dense");
 T205(!solid(s205_gar[0][0],s205_gar[0][1]+2.6f),"shed front walkable");
 /* ракушка: подсказка и осмотр */
 aim205(-100,-39.0f,-100,1.2f,-39.85f);T205(strstr(prompt,"ракуш")!=0,"shed prompt");
 int sl0=st205.shedLook;use205("shed look");T205(st205.shedLook>sl0,"shed inspected");
 /* пустое поле без подсказки уличного слоя */
 g.x=-90;g.z=-60;interaction();T205(!strstr(prompt,"ларёк"),"street quiet elsewhere");
 /* уличный слой не ломает формат сохранения и ядро */
 save_game();T205(load_game(),"save roundtrip");
 T205(g.revision150>0,"save revision");
 printf("QA205 %d checks, %d failed\n",n205,b205);
 return b205?1:0;
}
