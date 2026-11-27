#define HEADLESS
#define main original_game_main
#include GAME_SOURCE
#undef main
int main(void){savepath="benchmark.save";eventpath="benchmark_events.csv";audio_init();new_game();set_resolution(0);detail=1;antialias=0;render_mirror=0;hudMode=2;screen=0;g.winter=winterEnabled=1;printf("scene,width,height,frames,cpu_mean_ms,cpu_p95_ms,software_triangles_mean\n");
 for(int scene=0;scene<3;scene++){float poses[3][5]={{-23,0,-45,-.15f,.24f},{18,0,3,0,0},{-112,0,88,0,.22f}};g.x=poses[scene][0];g.playerY=poses[scene][1];g.z=poses[scene][2];g.rot=yaw=poses[scene][3];g.look=pitch=poses[scene][4];screen=0;for(int k=0;k<5;k++)render_frame();double sums=0,values[30];long tris=0;for(int k=0;k<30;k++){double t=prof_now();tick(1.f/30);render_frame();double ms=(prof_now()-t)*1000;values[k]=ms;sums+=ms;tris+=draws;}for(int a=0;a<30;a++)for(int b=a+1;b<30;b++)if(values[b]<values[a]){double v=values[a];values[a]=values[b];values[b]=v;}printf("%s,%d,%d,30,%.3f,%.3f,%.1f\n",scene==0?"home":scene==1?"civilians":"outer",W,H,sums/30,values[28],tris/30.0);}
 return 0;}
