#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void view170(const char*name,float x,float z,float fy,float tx,float ty,float tz){screen=0;g.x=x;g.z=z;g.playerY=fy;g.fallVelocity=0;update_interior();aim_at(tx,ty,tz);interaction();shot(name);printf("CAPTURE %s\n",name);fflush(stdout);}
int main(void){savepath="build/capture170.save";eventpath="build/capture170.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=15.5f;g.weather=0;realTime=12;showHints=0;
 for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}
 g.flatDoors150[0]|=3;g.things150[0]|=(1u<<16)|(1u<<17);g.balconyDoors150[0]=0;
 view170("qa/v170/01_clear_doorway.ppm",hx[0]-2.3f,hz[0]-1.0f,0,hx[0]-2.3f,1.45f,hz[0]+3.6f);
 view170("qa/v170/02_bathroom_joints.ppm",hx[0]-2.2f,hz[0]-.65f,0,hx[0]-3.45f,1.45f,hz[0]-.7f);
 view170("qa/v170/03_living_room.ppm",hx[0]-2.25f,hz[0]+1.22f,0,hx[0]-1.1f,1.3f,hz[0]+3.65f);
 view170("qa/v170/04_courtyard.ppm",hx[1]+1,hz[1]-18,0,hx[1]+2,6,hz[1]);
 int p=2;float x=panel_origin_x(p)+7,z=panel_origin_z(p);g.district[p].entries=0;
 view170("qa/v170/05_panel_kitchen.ppm",x+4.25f,z-.1f,0,x+6,1.35f,z-3.5f);
 view170("qa/v170/06_panel_exterior.ppm",panel_origin_x(p)+16,z-27,0,panel_origin_x(p),5.5f,z);
 view170("qa/v170/07_annex.ppm",HOME_RADIO_X+3.1f,HOME_RADIO_Z-2,0,HOME_RADIO_X-1.3f,1.3f,HOME_RADIO_Z+.7f);
 return 0;
}
