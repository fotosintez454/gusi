#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose100(float x,float z,float h,float tx,float ty,float tz){screen=0;g.x=x;g.z=z;g.playerY=h;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void snap100(const char*n){char p[200];SN(p,sizeof(p),"qa/v100/%s.ppm",n);shot(p);}
int main(void){savepath="release_v100/qa.save";eventpath="release_v100/qa_events.csv";audio_init();test_reset();set_resolution(2);district_material_prepare();
realTime=8;g.hours=13;camx=0;camy=1.15f;camz=-3.25f;yaw=0;pitch=-.055f;camera();clear_scene(.82f);surfaceTex=0;box(0,-.05f,0,16,.10f,16,0x50594b);PERSON(2).facing=PI;PERSON(2).activity=0;npcMoving[2]=0;npcGait[2]=0;human(0,0,2,0,0);screenshot("qa/v100/01_npc.ppm");
test_reset();g.hours=13;g.weather=0;pose100(hx[0]-.2f,hz[0]+1.7f,0,hx[0]+3.5f,1.25f,hz[0]+2.6f);snap100("02_bathroom_entry");pose100(hx[0]+2.0f,hz[0]+.9f,0,hx[0]+3.6f,.9f,hz[0]+3.1f);snap100("03_bathroom_inside");pose100(-44,-17,0,-40.5f,.8f,-12);snap100("04_bins");
panelIndex=0;entryOpen=3;flatOpen=65535;for(int n=0;n<20;n++)flatAngles[n]=1;pose100(panel_cx(0)-2.7f,PANEL_Z+.4f,3,panel_cx(0)-4.8f,4.2f,PANEL_Z+5.2f);snap100("05_bedroom");pose100(-87,48,0,-65,5,24);snap100("06_panel_rear");pose100(-65,-4,0,-65,5,24);snap100("07_panel_front");
return 0;}
