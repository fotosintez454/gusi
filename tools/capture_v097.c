#ifndef HEADLESS
#define HEADLESS
#endif
#define main original_game_main
#include "../src/main.c"
#undef main
static void snap(const char*path){render_frame();FILE*f=fopen(path,"wb");if(!f)return;fprintf(f,"P6\n%d %d\n255\n",W,H);for(int i=0;i<W*H;i++){unsigned char p[3]={(frame[i]>>16)&255,(frame[i]>>8)&255,frame[i]&255};fwrite(p,1,3,f);}fclose(f);}
int main(void){savepath="release_v097/capture.save";eventpath="release_v097/capture_events.csv";audio_init();new_game();set_resolution(2);district_material_prepare();screen=0;g.x=-20;g.z=-8;g.playerY=0;camx=g.x;camz=g.z;camy=1.73f;yaw=PI/2;pitch=0;g.rot=yaw;g.look=pitch;realTime=8;g.hours=16;snap("qa/v097_traffic.ppm");
 g.x=-69;g.z=5;camx=g.x;camz=g.z;camy=1.73f;yaw=(float)atan2(0,1);pitch=0;g.rot=yaw;g.look=pitch;snap("qa/v097_bus_stop.ppm");
 insideHouse=1;g.x=hx[1]+1.0f;g.z=hz[1]+1.2f;g.playerY=3;camx=g.x;camz=g.z;camy=g.playerY+1.73f;yaw=PI/2;pitch=-.05f;g.rot=yaw;g.look=pitch;snap("qa/v097_home_interior.ppm");
 screen=1;g.x=-20;g.z=-11;camx=-20;camz=-11;camy=3.1f;yaw=-.64f;pitch=-.07f;snap("qa/v097_menu.ppm");return 0;}
