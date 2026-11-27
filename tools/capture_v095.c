#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose(float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;screen=0;hudMode=0;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void snap(const char*n){char path[160];SN(path,sizeof(path),"qa/v095/%s.ppm",n);shot(path);}
int main(void){savepath="release_v095/capture.save";eventpath="release_v095/capture_events.csv";audio_init();test_reset();set_resolution(2);pose(-9,7,-6,1.1f,14);snap("01_compact_hud_winter");screen=10;dialogNPC=32;dialogTopic=0;snap("02_compact_dialogue");g.money=16000;g.bizLevel[0]=1;g.bizLevel[1]=2;g.bizLevel[3]=1;g.bizCapital=800;g.bizReceivable=540;g.bizLastGross=705;g.bizLastCost=290;screen=19;snap("03_business");insideHouse=0;g.homeLevel=1;screen=21;snap("04_home_upgrades");
 pose(RECYCLE_X,RECYCLE_Z-4.8f,RECYCLE_X,1.5f,RECYCLE_Z+1);navService=1;g.hours=13;snap("05_recycling_yard");pose(RECYCLE_X+2.7f,RECYCLE_Z-.8f,RECYCLE_X+2.6f,.38f,RECYCLE_Z+1.5f);hudMode=2;snap("06_rebar");
 test_reset();set_resolution(2);float x,z;loot_position(0,&x,&z);pose(x+.55f,z-1.6f,x+.55f,.65f,z);trash_start(0);bonusClock=bonus_duration()*(bonus_center()-.16f);snap("07_trash_skill_check");bonusActive=0;pose(-18,8.8f,-18,.6f,10);bench_sit(0);interaction();snap("08_bench_seated");bench_stand();
 g.hours=1;pose(-29,-1,-34.5f,4.7f,-6.4f);snap("09_night_lamps");g.hours=13;g.homeLevel=3;g.residence[0].doors|=32;WIN(2).handle=1;pose(hx[0]-.3f,hz[0]+7,hx[0]+1.4f,1.2f,hz[0]+12.5f);snap("10_expanded_home");pose(hx[0]+2.8f,hz[0]-.5f,hx[0]+3.5f,.65f,hz[0]-1.5f);snap("11_bathroom");
 pose(hx[0]+2.7f,hz[0]+10.1f,hx[0]+3.2f,1.35f,hz[0]+12.5f);tv_set_url("https://youtu.be/dQw4w9WgXcQ");screen=20;snap("12_tv_settings");screen=0;g.herb=1;inv_use(9);consume_tick(3.5f);snap("13_herb_consume");consume_tick(8);pose(hx[0]-6.3f,hz[0]-2.7f,hx[0]-5.1f,1.3f,hz[0]-2.7f);liftUI=30;screen=14;snap("14_lift");test_reset();set_resolution(2);pose(-22,11,-15,.35f,14.3f);snap("15_tire_beds");return 0;}
