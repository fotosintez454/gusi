#define HEADLESS
#define main original_main
#ifndef BENCH_SOURCE
#define BENCH_SOURCE "../src/main.c"
#endif
#include BENCH_SOURCE
#undef main
static void benchmark202(const char*name){render_frame();render_frame();double values[3];for(int r=0;r<3;r++){double t=prof_now();for(int i=0;i<8;i++)render_frame();values[r]=(prof_now()-t)*1000/8;}printf("BENCH202 %s resolution=%dx%d ms_per_frame=%.3f,%.3f,%.3f draws=%d\n",name,W,H,values[0],values[1],values[2],draws);fflush(stdout);}
int main(void){savepath="build/bench202.save";eventpath="build/bench202.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);viewdist=80;detail=1;antialias=0;g.weather=0;g.hours=22;g.x=hx[0]-5;g.z=hz[0]+10;g.playerY=0;aim_at(hx[0],3,hz[0]);benchmark202("night_windows");
 g.hours=14;g.winter=1;g.season=0;g.x=0;g.z=10;g.playerY=0;aim_at(0,.1f,14);g.trackHead190=TRACK190;for(int n=0;n<TRACK190;n++){g.tracks190[n].x=-10+(n%128)*.16f;g.tracks190[n].z=11+(n/128)*.15f;g.tracks190[n].y=.065f;g.tracks190[n].age=0;g.tracks190[n].yaw=n*.2f;g.tracks190[n].type=n%3;}benchmark202("winter_track_history");
 g.winter=0;g.season=2;g.trackHead190=0;g.x=87;g.z=river_z(90)-10;g.playerY=1.5f;aim_at(96,.2f,river_z(96)+7);benchmark202("river_summer");return 0;}
