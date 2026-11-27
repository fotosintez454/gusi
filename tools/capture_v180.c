#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void v180(const char*path,float x,float z,float base,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=base;g.fallVelocity=0;screen=0;update_interior();aim_at(tx,ty,tz);interaction();shot(path);printf("CAPTURE %s\n",path);fflush(stdout);}
int main(void){savepath="build/capture180.save";eventpath="build/capture180.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=16.5f;g.weather=0;realTime=12;showHints=0;for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}g.flatDoors150[0]|=3;g.things150[0]|=(1u<<16)|(1u<<17);
 v180("qa/v180/01_passage.ppm",hx[0]-2.4f,hz[0]-1,0,hx[0]-2.4f,1.45f,hz[0]+3.4f);
 v180("qa/v180/02_rear_window.ppm",hx[0]-2.5f,hz[0]+1.2f,0,hx[0]-1.0f,1.5f,hz[0]+4.5f);
 v180("qa/v180/03_no_television.ppm",HOME_RADIO_X+3.1f,HOME_RADIO_Z-2,0,HOME_RADIO_X-1.3f,1.3f,HOME_RADIO_Z+.7f);
 v180("qa/v180/04_twelve_floors.ppm",cityBlockX[1]+22,cityBlockZ[1]-30,0,cityBlockX[1],13,cityBlockZ[1]);
 int n=0,f=0,side=0;for(n=0;n<4;n++)for(f=0;f<high_floors180(n);f++)for(side=0;side<2;side++)if(high_color180(n,f,side)==0xab83ce)goto violet;
 violet:;float sg=side?1:-1,x=cityBlockX[n],z=cityBlockZ[n];unsigned bit=1u<<(f*2+side);g.highLightTouched180[n]|=bit;g.highLightOff180[n]&=~bit;
 v180("qa/v180/05_violet_on.ppm",x+sg*4.45f,z+1.3f,f*3,x+sg*4.8f,f*3+1.4f,z+5.2f);
 g.highLightOff180[n]|=bit;
 v180("qa/v180/06_same_room_off.ppm",x+sg*4.45f,z+1.3f,f*3,x+sg*4.8f,f*3+1.4f,z+5.2f);
 g.highEntry180|=1u;g.highDoors180[0]=3;
 v180("qa/v180/07_stairs_and_lobby.ppm",cityBlockX[0]+1.1f,cityBlockZ[0]-4.2f,0,cityBlockX[0]-.7f,1.5f,cityBlockZ[0]+2.5f);
 return 0;}
