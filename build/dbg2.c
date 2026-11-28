#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void probe(float x,float z,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=0;screen=0;aim_at(tx,ty,tz);
 printf("(%0.f,%0.f): room%d phys%d extra%d npc%d civic%d upd140%d high%d panel%d direct%d\n",x,z,
 room_use204(0),physics_prompt200(),extra_use200(0),npc_target200(),civic_use190(0),update140_use(0),high_use180(0),panel_prompt(),s205_pick());}
int main(void){savepath="build/d2.save";eventpath="build/d2.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 probe(58,-33,58,1.2f,-36);probe(44,17,44,1.2f,20);probe(hx[0]+3.2f,hz[0]-1.6f,hx[0]+3.2f,1.5f,hz[0]-4.62f);probe(52,-32,52,1.2f,-34.5f);
 return 0;}
