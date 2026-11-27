#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose101(float x,float z,float h,float tx,float ty,float tz){screen=0;g.x=x;g.z=z;g.playerY=h;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void snap101(const char*n){char p[200];SN(p,sizeof(p),"qa/v101/%s.ppm",n);shot(p);}
static void civilians(void){test_reset();set_resolution(2);realTime=6;camx=0;camy=1.05f;camz=-4.5f;yaw=0;pitch=-.02f;camera();clear_scene(.88f);lighting_reset();surfaceTex=0;box(0,-.04f,0,30,.08f,30,0x6d7874);int ids[]={2,1,5,7};for(int k=0;k<4;k++){int n=ids[k];PERSON(n).facing=PI;PERSON(n).activity=0;npcMoving[n]=0;npcGait[n]=k==3?.6f:0;human(-1.65f+k*1.1f,.4f,n,k==3?1.5f:0,0);}screenshot("qa/v101/01_residents.ppm");}
int main(void){savepath="release_v101/capture.save";eventpath="release_v101/capture_events.csv";audio_init();test_reset();set_resolution(2);district_material_prepare();civilians();test_reset();set_resolution(2);g.hours=12;g.weather=0;
pose101(hx[0]-7.2f,hz[0]+.5f,0,hx[0]-11.2f,1.0f,hz[0]+1.3f);snap101("02_living_day");g.hours=0;snap101("03_living_night");g.hours=12;
WIN(2).angle=1.0f;WIN(2).handle=1;
pose101(hx[0]+3.3f,hz[0]+.9f,0,hx[0]+3.0f,1.1f,hz[0]+4.5f);snap101("04_bathroom_outside_window");
g.residence[0].doors|=256;pose101(hx[0]+.1f,hz[0]+1.55f,0,hx[0]+1.55f,1.2f,hz[0]+1.55f);snap101("05_bathroom_closed_door");g.residence[0].doors&=~256;
pose101(hx[1]-10,hz[1]-5,0,hx[1]-4.9f,7.0f,hz[1]-2.7f);snap101("06_closed_gable");
g.residence[1].doors|=1;residenceDoorAngle[1][0]=1;pose101(hx[1]+4.0f,hz[1]-3,0,hx[1],1.2f,hz[1]-2);snap101("07_clear_entrance");
pose101(hx[0]-2.3f,hz[0]+1.3f,0,hx[0]-8.5f,1.1f,hz[0]+1.3f);snap101("08_side_connection");
screen=2;snap101("09_pause");screen=22;snap101("10_notebook");
return 0;}
