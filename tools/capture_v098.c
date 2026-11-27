#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose(float x,float z,float height,float tx,float ty,float tz){screen=0;g.x=x;g.z=z;g.playerY=height;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void snap(const char*n){char p[200];SN(p,sizeof(p),"qa/v098/%s.ppm",n);shot(p);}
static void studio(const char*n,int act,float time){realTime=time;g.hours=13;camx=0;camy=1.15f;camz=-3.25f;yaw=0;pitch=-.055f;camera();clear_scene(.82f);surfaceTex=0;box(0,-.05f,0,16,.10f,16,0x50594b);box(0,1.8f,2,10,4,.1f,0x323c33);PERSON(1).facing=PI;PERSON(1).activity=act;npcMoving[1]=0;npcGait[1]=act?0:1;human(0,0,1,1.2f,0);UI=W/800.f;ut(24,22,"ОКНО / ORIGINAL CIVILIAN",.45f,0xd5c99e);ut(24,413,"552 vertices / 872 triangles / 23 joints / 256px atlas",.30f,0xc3c4aa);char p[160];SN(p,sizeof(p),"qa/v098/%s.ppm",n);screenshot(p);}
int main(void){savepath="release_v098/capture.save";eventpath="release_v098/capture_events.csv";audio_init();test_reset();set_resolution(2);district_material_prepare();studio("01_new_mesh_walk",0,8);studio("02_new_mesh_drink",10,.35f);studio("03_new_mesh_smoke",9,.35f);
 test_reset();set_resolution(2);g.hours=13;realTime=8;pose(-65,-4,0,-65,4.8f,24);snap("04_two_entrances");screen=1;snap("05_main_menu");screen=22;snap("06_mouse_notebook");screen=5;g.water=2;g.food=1;g.beer=1;inv_sync();invFullInfo=1;snap("07_inventory");screen=10;dialogNPC=32;dialogTopic=0;snap("08_dialogue");
 panelIndex=0;entryOpen=3;flatOpen=65535;for(int n=0;n<20;n++)flatAngles[n]=1;pose(panel_cx(0)-2.7f,PANEL_Z+.4f,3,panel_cx(0)-5.1f,4,PANEL_Z+5.2f);snap("09_flat_bedroom");pose(panel_cx(0)-4.3f,PANEL_Z-3.2f,3,panel_cx(0)-2.75f,3.7f,PANEL_Z-4.05f);snap("10_flat_bathroom");
 pose(hx[0]+1.8f,hz[0]+1.35f,0,hx[0]+3.45f,.8f,hz[0]+2.75f);snap("11_home_wet_room");pose(hx[0]-1.8f,hz[0]+1.7f,0,hx[0]-3.3f,1.55f,hz[0]+2.7f);snap("12_home_radio");
 g.stress=95;pose(-18,4,0,-8,1.4f,10);snap("13_stress");g.focusMode=1;g.uiMotion=0;snap("14_reduced_motion");return 0;}
