#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d3.save";eventpath="build/d3.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 float pts[][6]={{58,-33,58,1.2f,-36,1},{44,17,44,1.2f,20,1},{-29.8f,-25.3f,-26.6f,1.5f,-28.3f,1},{52,-32,52,1.2f,-34.5f,1}};
 for(int i=0;i<4;i++){g.x=pts[i][0];g.z=pts[i][1];g.playerY=0;aim_at(pts[i][2],pts[i][3],pts[i][4]);
  printf("case%d dst=%.2f eye=%.2f look=%d yaw=%.2f pitch=%.2f\n",i,dst(g.x,g.z,pts[i][2],pts[i][4]),player_eye180(),look_at(pts[i][2],pts[i][3],pts[i][4],.92f),yaw,pitch);}
 return 0;}
