#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void pose(float x,float z,float y,float tx,float ty,float tz){screen=0;help=0;seated=-1;simAccumulator=1.f/30;g.x=x;g.z=z;g.playerY=y;g.fallVelocity=0;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
int main(void){savepath="release_v151/capture.save";eventpath="release_v151/capture_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(1);g.hours=16.7f;g.weather=0;realTime=18;
 pose(21,-5,0,-8,6,-22);shot("qa/v151/01_dense_homes.ppm");
 pose(-13,-47,0,-15,1,-35);g.zoneFlags150|=7u<<16;shot("qa/v151/02_playground.ppm");
 g.hours=18;pose(-39,-113,0,-38,6,-89);shot("qa/v151/03_north_front_windows.ppm");
 pose(-40,-76,0,-38,6,-89);shot("qa/v151/04_north_rear_windows.ppm");
 pose(-18,7,0,-18,6,22);shot("qa/v151/05_evening_homes.ppm");
 screen=1;shot("qa/v151/06_menu.ppm");screen=5;shot("qa/v151/07_inventory.ppm");screen=11;shot("qa/v151/08_map.ppm");
 screen=0;g.winter=winterEnabled=0;g.season=2;g.x=25;g.z=river_z(25);g.playerY=-1.48f;g.stamina=0;
 for(int k=0;k<300;k++)water_tick141(1.f/30);pose(g.x,g.z,g.playerY,20,1,river_z(20)+2);shot("qa/v151/09_surface_no_air_hud.ppm");keys[17]=1;
 for(int k=0;k<180;k++)water_tick141(1.f/30);pose(g.x,g.z,g.playerY,30,-1.8f,river_z(30));shot("qa/v151/10_dive_air_hud.ppm");
 printf("V151 ten CPU captures complete\n");return 0;}
