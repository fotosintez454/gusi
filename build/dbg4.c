#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d4.save";eventpath="build/d4.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 g.x=58;g.z=-33;g.playerY=0;aim_at(58,1.2f,-36);
 {float dx=(58-g.x)*WORLD_XZ,dy=1.2f-(g.playerY+player_eye180()),dz=(-36-g.z)*WORLD_XZ;float d=(float)sqrt(dx*dx+dy*dy+dz*dz);float dot=dx*(float)sin(yaw)*(float)cos(pitch)+dy*(float)sin(pitch)+dz*(float)cos(yaw)*(float)cos(pitch);
 printf("WORLD_XZ=%.3f dx%.2f dy%.2f dz%.2f d%.3f dot%.3f thr%.3f\n",(double)WORLD_XZ,dx,dy,dz,d,dot,d*.92f);}
 return 0;}
