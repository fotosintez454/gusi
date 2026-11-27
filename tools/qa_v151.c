#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int checks151=0,bad=0;
#define T(x,label) do{checks151++;if(!(x)){bad++;printf("FAIL: %s\n",label);}}while(0)
int main(void){savepath="release_v151/test.save";eventpath="release_v151/test_events.csv";audio_init();nextWorldSeed=140;test_reset();
 for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}
 for(int i=0;i<3;i++)T(hx[i+1]-hx[i]<15.4f,"central row spacing compact");
 for(int p=1;p<=7;p++)T(panel_origin_z(p)>-90&&panel_origin_z(p)<-88,"north row moved toward district");
 short path[MAXPATH];for(int h=0;h<NH;h++)T(nav_plan(-20,-8,hx[h]+7.55f,hz[h]-9.5f,path)>0,"all residence entrances connected");
 for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++)T(nav_plan(-20,-8,panel_origin_x(p)+(w?7:-7),panel_origin_z(p)-9.3f,path)>0,"all panel entrances connected");
 props_init150();T(propCount150==59,"six playground props, previous IDs stable");
 for(int n=53;n<59;n++){Prop150*p=&props150[n];g.x=p->x;g.z=p->z-3;g.playerY=0;aim_at(p->x,1,p->z);g.zoneFlags150=0;T(prop_use150(1),"playground action reachable");T(play_on151(n),"E starts correct object");realTime+=.4f;T(play_motion151(n)>.3f,"animation clock progresses");T(prop_use150(1),"repeat action available");T(!play_on151(n),"E stops correct object");T(!nav_clear(p->x,p->z),"playground collider used by NPC navigation");}
 for(int n=41;n<45;n++){Prop150*p=&props150[n];g.x=p->x;g.z=p->z-2;g.playerY=0;aim_at(p->x,.81f,p->z);T(prop_use150(1)&&seated==2000+n,"extra bench sits");float x=g.x,z=g.z;bench_stand();T(seated==-1&&dst(x,z,g.x,g.z)<.01f,"extra bench stand has no teleport or invalid index");}
 g.winter=0;g.x=30;g.z=river_z(30);g.playerY=-1.48f;g.stamina=0;g.health=100;swimAir141=100;keys[17]=keys[32]=0;
 for(int k=0;k<18000;k++)water_tick141(1.f/30);T(swimAir141==100&&g.health==100,"ten minutes on surface at zero stamina");T(!underwater151(),"surface does not qualify for underwater HUD");
 keys[17]=1;for(int k=0;k<180;k++)water_tick141(1.f/30);T(swimAir141<55&&underwater151(),"intentional dive uses oxygen");
 keys[32]=1;for(int k=0;k<120;k++)water_tick141(1.f/30);T(swimAir141==100&&!underwater151(),"Space overrides Ctrl to surface");
 keys[32]=keys[17]=0;g.playerY=-2.28f;swimAir141=35;water_tick141(.1f);T(swimAir141>35,"accidental low entry cannot consume oxygen without dive intent");
 g.winter=1;g.drowning=10;swimAir141=20;water_tick141(.1f);T(g.drowning==0&&!underwater151(),"ice clears drowning");
 g.winter=0;g.playerY=1;g.drowning=10;water_tick141(.1f);T(g.drowning==0,"above water clears stale drowning");
 g.hours=16.7f;int lit=0,unlit=0;for(int h=NH;h<NH+PANEL_COUNT;h++)for(int f=0;f<4;f++)for(int k=0;k<32;k++){lit+=window_lit151(h,f,k);unlit+=!window_lit151(h,f,k);}T(lit>0&&unlit>0,"panel windows mixed illuminated and dark");
 g.zoneFlags150=(1u<<16)|(1u<<21)|1;save_game();g.zoneFlags150=0;T(load_game()&&g.zoneFlags150==((1u<<16)|(1u<<21)|1),"garage and playground toggles persist together");
 printf("V151 TESTS: %d checks, %d failures\n",checks151,bad);return bad?1:0;}
