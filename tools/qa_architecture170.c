#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int count170=0,bad170=0;
#define A170(c,label) do{count170++;if(!(c)){bad170++;printf("FAIL170 h=%d f=%d side=%d %s\n",hh,ff,ss,label);}}while(0)
int main(void){savepath="build/arch170.save";eventpath="build/arch170.csv";audio_init();nextWorldSeed=140;test_reset();physicsActor150=-1;int hh=0,ff=0,ss=0;
 for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}
 camx=camz=camy=yaw=pitch=0;camera();PV p=transform(1,1,5,0,0);
 A170(ab(p.x-WORLD_XZ)<.0001f&&ab(p.y-1)<.0001f&&ab(p.z-5*WORLD_XZ)<.0001f,"horizontal world is expanded in physical metres, vertical height is preserved");
 A170(WORLD_XZ>=1.3f&&HOUSE_HALF_W*2*WORLD_XZ>12.5f,"buildings have full-size physical width");
 A170(!(hx[0]-14.3f<OX+8.2f&&hx[0]-4.8f>OX-8.2f&&hz[0]-4.4f<OZ+5.8f&&hz[0]+4.4f>OZ-5.8f),"home annex does not overlap the office");
 for(hh=0;hh<NH;hh++)for(ff=0;ff<5;ff++)for(ss=0;ss<2;ss++){
  int id=unit150(hh,ff,ss);float sg=ss?1:-1;g.playerY=ff*3;g.fallVelocity=0;g.flatDoors150[hh]|=1u<<(ff*2+ss);g.things150[id]|=(1u<<16)|(1u<<17);
  g.x=hx[hh]+sg*2.4f;g.z=hz[hh]-.6f;
  A170(body_clear150(g.x,g.z,g.playerY,.27f/WORLD_XZ,0),"full capsule fits the entry lane");
  move(0,2.6f);A170(ab(g.z-(hz[hh]+2.0f))<.035f,"actual movement from kitchen into room is unobstructed");
  move(0,-2.6f);A170(ab(g.z-(hz[hh]-.6f))<.035f,"actual return movement through room doorway is unobstructed");
  g.things150[id]&=~(1u<<17);move(0,2.6f);A170(g.z<hz[hh]+ROOM_WALL_B-.25f,"closed room door blocks the physical capsule");
  g.x=hx[hh]+sg*2.4f;g.z=hz[hh]-.6f;aim_at(hx[hh]+sg*ROOM_DOOR_A,ff*3+1.1f,hz[hh]+ROOM_WALL_B);
  A170(use150(1)&&!!(g.things150[id]&(1u<<17)),"room door can be opened using the aimed interaction");
  for(int n=0;n<6;n++){float b=FLAT_ENTRY_B+.05f+n*(BATH_DOOR_B0-FLAT_ENTRY_B-.1f)/5;
   A170(room_solid150(hx[hh]+sg*BATH_A,hz[hh]+b),"lower bathroom wall span is continuously sealed");}
  for(int n=0;n<6;n++){float b=BATH_DOOR_B1+.05f+n*(BATH_B-BATH_DOOR_B1-.1f)/5;
   A170(room_solid150(hx[hh]+sg*BATH_A,hz[hh]+b),"upper bathroom wall span is continuously sealed");}
 }
 printf("ARCHITECTURE170: %d checks, %d failures\n",count170,bad170);return bad170?1:0;
}
