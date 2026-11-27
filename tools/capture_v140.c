#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose(float x,float z,float height,float tx,float ty,float tz){screen=0;g.x=x;g.z=z;g.playerY=height;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void snap(const char*n){char p[200];SN(p,sizeof(p),"qa/v140/%s.ppm",n);shot(p);}
int main(void){savepath="release_v140/capture.save";eventpath="release_v140/capture_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=13;realTime=8;district_material_prepare();g.weather=0;
 pose(-55,-7,0,-27,8,-22);snap("01_winter_district");
 pose(shopX+2,shopZ-11,0,shopX,1.9f,shopZ);snap("02_shop_off_road");
 pose(10,76,0,1,1.5f,88);snap("03_frozen_river");
 g.season=2;g.seasonDay=g.day;update140_tick(0);pose(10,76,0,1,1.5f,88);snap("04_summer_river");
 g.season=3;update140_tick(0);pose(-55,-7,0,-27,8,-22);snap("05_autumn_district");
 g.season=1;update140_tick(0);pose(-55,-7,0,-27,8,-22);snap("06_spring_district");
 int h=2;g.residence[h].doors=0xffffffff;pose(hx[h]-.5f,hz[h]-2.8f,3,hx[h]-4.1f,4.2f,hz[h]+.6f);snap("07_shared_flat");
 g.season=0;update140_tick(0);pose(hx[h]+1,hz[h]+1,15.16f,hx[h]-10,14,hz[h]-30);snap("08_rooftop");
 pose(ATM_X,ATM_Z-1.5f,0,ATM_X,1.3f,ATM_Z);snap("09_atm_street");g.bank=2400;screen=23;snap("10_atm_menu");screen=11;snap("11_map");
 screen=0;g.weather=1;g.weatherTimer=10;pose(hx[h],hz[h]+2,3,hx[h],4.6f,hz[h]+12);snap("12_snow_through_window");
 pose(hx[h]-6.4f,hz[h]-2.1f,0,hx[h]-5.0f,1.2f,hz[h]-2.1f);screen=14;liftUI=30+h;snap("13_elevator_menu");
 g.health=0;screen=24;snap("14_death");screen=25;snap("15_seed_menu");
 test_reset();g.hours=13;work_assign(0,1,2);accept_job(0);aim_work_test(g.workTarget[0]);work_interact(g.workTarget[0]);bonusClock=.2f;snap("16_skill_check");
 return 0;}
