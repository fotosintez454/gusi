#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void pose160(int h,int f,int s,float a,float b,float ta,float ty,float tb){float sg=s?1:-1;screen=0;help=0;seated=-1;g.playerY=f*3;g.x=hx[h]+sg*a;g.z=hz[h]+b;g.fallVelocity=0;simAccumulator=1.f/30;update_interior();aim_at(hx[h]+sg*ta,f*3+ty,hz[h]+tb);interaction();noticeTime=0;}
int main(void){savepath="release_v160/capture.save";eventpath="release_v160/capture_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(1);g.hours=16.4f;g.weather=0;realTime=12;
 pose160(0,0,0,2.4f,-.6f,2.4f,1.3f,2.9f);shot("qa/v160/01_spawn_clear.ppm");
 pose160(0,0,0,2.05f,-.3f,.65f,1.0f,-.3f);shot("qa/v160/02_kitchen_stove.ppm");
 pose160(0,0,0,2.5f,1.5f,1.2f,1.1f,3.0f);shot("qa/v160/03_living_bedroom.ppm");
 pose160(0,0,0,2.5f,-.7f,4.15f,1.2f,-.9f);shot("qa/v160/04_bathroom.ppm");
 pose160(0,0,0,2.8f,3.7f,2.8f,1.5f,4.5f);shot("qa/v160/05_window_approach.ppm");
 pose160(12,2,1,2.5f,1.5f,3.8f,1.1f,3.55f);shot("qa/v160/06_other_flat.ppm");
 screen=0;g.x=-4;g.z=-47;g.playerY=0;aim_at(-15,7,-24);shot("qa/v160/07_district_street.ppm");
 screen=11;shot("qa/v160/08_district_map.ppm");
 screen=0;g.playerY=0;g.x=panel_origin_x(2)+7+4.3f;g.z=panel_origin_z(2)-1.5f;aim_at(panel_origin_x(2)+7+6.35f,1.1f,panel_origin_z(2)-4);interaction();shot("qa/v160/09_panel_kitchen.ppm");
 g.x=panel_origin_x(2)+7+3.5f;g.z=panel_origin_z(2)+3;aim_at(panel_origin_x(2)+7+5.1f,1.1f,panel_origin_z(2)+5.2f);interaction();shot("qa/v160/10_panel_bedroom.ppm");
 printf("V160 ten CPU captures complete\n");return 0;}
