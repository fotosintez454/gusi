#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void snap130(int state,const char*name){char p[220];screen=state;if(state==0)interaction();noticeTime=0;SN(p,sizeof(p),"qa/v130/%s.ppm",name);shot(p);}
int main(void){savepath="release_v130/capture.save";eventpath="release_v130/capture_events.csv";audio_init();test_reset();set_resolution(2);g.hours=12.5f;g.weather=0;realTime=10;work_assign(1,2,3);accept_job(1);g.health=82;g.stamina=63;g.x=-18;g.z=-8;g.playerY=0;aim_at(10,5,-22);snap130(0,"01_winter_hud_goal");
 g.x=hx[0]+6.2f;g.z=hz[0]-3;g.playerY=0;g.residence[0].doors|=1;for(int k=0;k<30;k++)panel_motion(.033f);aim_at(hx[0]+4.93f,1.3f,hz[0]-3);snap130(0,"02_clear_door_landing");
 test_reset();set_resolution(2);g.hours=12.5f;work_assign(0,1,4);accept_job(0);int id=g.workTarget[0];aim_work_test(id);work_interact(id);bonusClock=bonus_duration()*.18f;snap130(0,"03_floor_five_required_check");for(int k=0;k<2;k++){bonusClock=bonus_center()*bonus_duration();pressed[32]=1;bonus_tick(0);pressed[32]=0;}bonusClock=.30f;snap130(0,"04_third_stage_still_closed");bonusClock=bonus_center()*bonus_duration();pressed[32]=1;bonus_tick(0);pressed[32]=0;for(int k=0;k<35;k++)panel_motion(.033f);snap130(0,"05_exact_balcony_open");
 g.x=hx[1]+7.35f;g.z=hz[1]-1.8f;g.playerY=6;work_assign(1,1,4);accept_job(1);aim_at(hx[1]+7.35f,8.4f,hz[1]+1.8f);snap130(0,"06_upper_landing_goal");snap130(3,"07_jobs_floor_address");
 test_reset();set_resolution(2);g.x=-18;g.z=-8;g.playerY=0;aim_at(-8,1.5f,-22);dialogNPC=1;dialog_begin(1);dialog_choose(0);dialog_choose(1);snap130(10,"08_dialogue_work_offer");dialog_choose(3);snap130(10,"09_dialogue_accepted");for(int i=0;i<7;i++)dialog_choose(i%2?0:2);dialogScroll=8;snap130(10,"10_dialogue_scrollback");
 set_resolution(0);dialogScroll=0;snap130(10,"11_dialogue_800px");set_resolution(2);optionsTab=1;snap130(6,"12_options_no_minimap");
 test_reset();set_resolution(2);g.hours=12;g.x=-25;g.z=5.5f;g.playerY=0;aim_at(-23,0.25f,8.3f);snap130(0,"13_geometric_snowbanks");return 0;}
