#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/door.save";eventpath="build/door.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=16.4f;g.weather=0;realTime=12;
 screen=0;g.x=hx[0]-2.3f;g.z=hz[0]-1.2f;g.playerY=0;g.things150[unit150(0,0,0)]&=~(1u<<17);update_interior();aim_at(hx[0]-2.3f,1.2f,hz[0]+3.0f);interaction();shot("build/door_closed.ppm");
 g.things150[unit150(0,0,0)]|=(1u<<17);screen=0;update_interior();aim_at(hx[0]-2.3f,1.2f,hz[0]+3.0f);interaction();shot("build/door_open.ppm");return 0;}
