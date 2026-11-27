#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose141(float x,float z,float y,float tx,float ty,float tz){screen=0;simAccumulator=1.f/30;g.x=x;g.z=z;g.playerY=y;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void cap141(const char*n){char p[200];SN(p,sizeof(p),"qa/v141/%s.ppm",n);shot(p);}
int main(void){savepath="release_v141/capture.save";eventpath="release_v141/capture_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(1);g.hours=13;g.season=2;g.seasonDay=g.day;update140_tick(0);g.weather=0;realTime=12;visualAlpha=1;
 pose141(hx[0]-9.6f,hz[0],0,hx[0]-4,1.2f,hz[0]);cap141("01_home_no_shaft");
 int h=2;pose141(hx[h]+7.8f,hz[h]-6.f,0,hx[h]+5.9f,1.3f,hz[h]-6);cap141("02_internal_lift_lobby");
 pose141(hx[h]+9.9f,hz[h]-12,0,hx[h]+3,6.5f,hz[h]-2);cap141("03_enclosed_entry_exterior");
 for(int f=0;f<3;f++){pose141(hx[h],hz[h]-1.8f,f*3,hx[h]-2.8f,f*3+1.3f,hz[h]+2.4f);char n[80];SN(n,sizeof(n),"04_flat_variant_%d",f);cap141(n);}
 pose141(hx[h]+7.35f,hz[h]-1,12,hx[h]+5.85f,14.3f,hz[h]+2.8f);cap141("05_actual_roof_stair");
 pose141(hx[h]+1.5f,hz[h],15.16f,hx[h]-2,14.8f,hz[h]+8);cap141("06_clean_roof_surface");
 pose141(shopX+1,shopZ-12,0,shopX,1.8f,shopZ-4);cap141("07_store_facade");
 pose141(shopX,shopZ-3.5f,0,shopX+2,1.2f,shopZ+2);cap141("08_store_inside");
 int npc=1;float bx,bz;bench_position141(PERSON(npc).home,&bx,&bz);PERSON(npc).x=bx-.58f;PERSON(npc).z=bz-.07f;PERSON(npc).facing=PI;PERSON(npc).activity=12;npcMoving[npc]=0;npcGait[npc]=0;sitting141[npc]=1;npcPrevX[npc]=PERSON(npc).x;npcPrevZ[npc]=PERSON(npc).z;
 pose141(bx-2.0f,bz-3.3f,0,bx,1,bz);cap141("09_elder_seated_entry");
 pose141(10,river_z(10)-6,0,2,-.6f,river_z(2)+2);cap141("10_river_bed_waves");
 g.x=10;g.z=river_z(10);g.playerY=0;for(int k=0;k<70;k++)water_tick141(1.f/30);aim_at(15,.3f,river_z(15)+8);cap141("11_swimming_waterline");
 test_reset();g.hours=13;g.season=2;update140_tick(0);carX140[0]=-10;carZ140[0]=-2.75f;PERSON(2).x=-10.1f;PERSON(2).z=-4.2f;npcPrevX[2]=PERSON(2).x;npcPrevZ[2]=PERSON(2).z;pose141(-14,-7.5f,0,-10,1.0f,-2.75f);cap141("12_car_scale");
 g.cigarettes=1;consume_begin(4);itemAnim=consume_duration()*.55f;pose141(-20,-9,0,-20,1.6f,-5);cap141("13_cigarette_in_hand");
 int p=0;panelIndex=0;entryOpen=3;itemAnim=0;pose141(panel_cx(0)+.8f,PANEL_Z+3,9,panel_cx(0)-.8f,11.2f,PANEL_Z+6.5f);cap141("14_panel_roof_stair");
 return 0;}
