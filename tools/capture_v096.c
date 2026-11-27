#ifndef HEADLESS
#define HEADLESS
#endif
#define main original_game_main
#include "../src/main.c"
#undef main
static void snap(const char*path){render_frame();FILE*f=fopen(path,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int i=0;i<W*H;i++){unsigned char p[3]={(frame[i]>>16)&255,(frame[i]>>8)&255,frame[i]&255};fwrite(p,1,3,f);}fclose(f);}
int main(void){savepath="release_v095/capture.save";eventpath="release_v095/capture_events.csv";audio_init();new_game();set_resolution(2);screen=0;g.x=-20;g.z=-8;g.playerY=0;camx=g.x;camz=g.z;camy=1.73f;yaw=PI/2;pitch=.0f;g.rot=yaw;g.look=pitch;realTime=8;g.hours=16;snap("qa/v096_traffic.ppm");g.x=-68;g.z=-29;camx=g.x;camz=g.z;camy=1.73f;yaw=(float)atan2(-14.0f,6.0f);pitch=.02f;g.rot=yaw;g.look=pitch;snap("qa/v096_city.ppm");return 0;}
