#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose094(float x,float z,float y){g.x=x;g.z=z;g.playerY=y;g.rot=yaw=0;g.look=pitch=0;screen=0;update_interior();prompt[0]=0;noticeTime=0;}
static void snap094(const char*name){char path[180];SN(path,sizeof(path),"qa/v094/%s.ppm",name);if(screen==0)interaction();shot(path);}
int main(void){savepath="release_v094/capture.save";eventpath="release_v094/capture_events.csv";audio_init();test_reset();set_resolution(2);screen=1;snap094("01_main");pose094(-20,-9,0);snap094("02_hud_idle");accept_job(1);noticeTime=0;aim_at(hx[g.jobs[1].house],1.5f,hz[g.jobs[1].house]-6);snap094("03_hud_client");screen=3;snap094("04_jobs");
 pose094(hx[g.jobs[1].house]-2.2f,hz[g.jobs[1].house]+2.8f,0);float x,y,z;window_handle_pos(g.jobs[1].house*3,&x,&y,&z);aim_at(x,y,z);snap094("05_window_target");toggle_window(g.jobs[1].house*3);for(int i=0;i<24;i++)simulate(1.f/30);bonusActive=0;window_handle_pos(g.jobs[1].house*3,&x,&y,&z);aim_at(x,y,z);noticeTime=0;snap094("06_window_open");
 pose094(RECYCLE_X,RECYCLE_Z-3.3f,0);navService=1;navOffice=0;aim_at(RECYCLE_X,1.3f,RECYCLE_Z+1.1f);snap094("07_recycle_world");pose094(RECYCLE_X,RECYCLE_Z,0);g.bottles=4;g.scrap=3;screen=18;snap094("08_recycle_menu");
 pose094(hx[0]-3,hz[0]-2,0);navService=2;screen=17;snap094("09_sleep_menu");g.hunger=85;g.thirst=85;player_sleep(8);screen=0;aim_at(hx[0],1.3f,hz[0]+2);snap094("10_after_sleep");
 test_reset();set_resolution(2);pose094(0,-7,0);for(int n=0;n<NN;n++){PERSON(n).x=170;PERSON(n).z=100;}int ids[]={1,3,4};for(int n=0;n<3;n++){int id=ids[n];PERSON(id).x=-1.15f+n*1.15f;PERSON(id).z=-3;PERSON(id).facing=PI;npcPrevX[id]=PERSON(id).x;npcPrevZ[id]=PERSON(id).z;}aim_at(0,1.3f,-3);hudMode=2;snap094("11_npc_lineup");
 screen=2;snap094("12_pause");screen=5;g.bottles=g.scrap=0;inv_sync();snap094("13_inventory");screen=6;optionsTab=3;snap094("14_focus");screen=10;dialogNPC=3;dialogTopic=1;snap094("15_dialogue");screen=8;snap094("16_stats");
 /* Offline PCM audition file, with a second of silence between examples. */
 FILE*f=fopen("qa/v094/AUDIO_DEMO.wav","wb");if(!f)return 1;unsigned int bytes=8*22050*2;unsigned char header[44]={0};memcpy(header,"RIFF",4);*(unsigned int*)(header+4)=bytes+36;memcpy(header+8,"WAVEfmt ",8);*(unsigned int*)(header+16)=16;*(unsigned short*)(header+20)=1;*(unsigned short*)(header+22)=1;*(unsigned int*)(header+24)=22050;*(unsigned int*)(header+28)=44100;*(unsigned short*)(header+32)=2;*(unsigned short*)(header+34)=16;memcpy(header+36,"data",4);*(unsigned int*)(header+40)=bytes;fwrite(header,1,44,f);int sounds[]={12,13,14,1,8,11,7,15};ambientVolume=0;short pcm[22050];for(int n=0;n<8;n++){audio(sounds[n]);mix_audio(pcm,22050);fwrite(pcm,2,22050,f);}fclose(f);return 0;
}
