#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void snap120(int state,const char*name){char p[220];screen=state;noticeTime=0;SN(p,sizeof(p),"qa/v120/%s.ppm",name);shot(p);}
int main(void){savepath="release_v120/capture.save";eventpath="release_v120/capture_events.csv";audio_init();test_reset();set_resolution(2);g.hours=16.4f;g.weather=0;g.x=hx[0]-6.4f;g.z=hz[0]+1.2f;g.playerY=0;update_interior();aim_at(hx[0]-11,1.05f,hz[0]);interaction();showRadar=0;realTime=10;
 snap120(0,"01_world_quiet");snap120(1,"02_main");snap120(2,"03_pause");snap120(22,"04_notebook");snap120(11,"05_map");snap120(3,"06_jobs");
 inv_add(1,2);inv_add(2,3);inv_add(3,1);invSelected=0;snap120(5,"07_inventory");snap120(4,"08_shop");optionsTab=0;snap120(6,"09_options_graphics");optionsTab=1;snap120(6,"10_options_game");optionsTab=2;snap120(6,"11_options_sound");optionsTab=3;snap120(6,"12_options_focus");dialogNPC=1;dialogTopic=0;snap120(10,"13_dialogue");snap120(8,"14_health");snap120(9,"15_wages");snap120(7,"16_team");snap120(15,"17_cat");snap120(17,"18_sleep");snap120(18,"19_recycling");snap120(19,"20_business");snap120(21,"21_home");snap120(20,"22_tv");snap120(16,"23_wallet");snap120(14,"24_stair_help");help=1;snap120(2,"25_help");help=0;
 g.hours=0;g.health=15;g.hunger=18;g.thirst=22;snap120(0,"26_night_needs");
 test_reset();set_resolution(2);realTime=8;camx=0;camy=1.08f;camz=-4.1f;yaw=0;pitch=-.02f;camera();clear_scene(.85f);lighting_reset();surfaceTex=0;box(0,-.04f,0,30,.08f,30,0x6d7874);int ids[]={2,1,5,7};for(int k=0;k<4;k++){int n=ids[k];PERSON(n).facing=PI;npcMoving[n]=0;npcGait[n]=0;human(-1.65f+k*1.1f,.4f,n,0,0);}screenshot("qa/v120/27_bodies.ppm");
 test_reset();set_resolution(2);panelIndex=0;g.playerY=0;g.x=panel_cx(0)-4.8f;g.z=PANEL_Z-3;entryOpen=3;flatOpen=65535;update_interior();aim_at(panel_cx(0)-3.85f,1.15f,PANEL_Z-3);panel_interact();snap120(0,"28_modular_bath_closed");
 set_resolution(0);optionsTab=1;snap120(6,"29_options_800px");return 0;}
