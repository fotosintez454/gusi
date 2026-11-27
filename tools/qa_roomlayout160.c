#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int roomChecks=0,roomFailures=0;
#define ROOM_CHECK(c,m) do{roomChecks++;if(!(c)){roomFailures++;printf("FAIL room: %s [%.2f %.2f]\n",m,g.x,g.z);}}while(0)
int main(void){savepath="release_v160/roomlayout.save";eventpath="release_v160/roomlayout_events.csv";audio_init();nextWorldSeed=140;test_reset();physicsActor150=-1;g.playerY=0;float ax=hx[0]-9.6f,az=hz[0];
 ROOM_CHECK(home_annex_furniture_solid(ax,az+.4f),"dining table has matching player collision");
 ROOM_CHECK(home_annex_furniture_solid(ax-2.7f,az+.4f),"sofa has matching player collision");
 ROOM_CHECK(!home_annex_furniture_solid(ax-2.9f,az+3.2f),"removed TV and console leave no ghost collider");
 ROOM_CHECK(!building_solid(ax+3.45f,az+.4f),"entry-to-living-room passage remains open");
 ROOM_CHECK(body_clear150(ax+1.05f,az+.4f,0,.28f,0),"player can stand beside radio table");
 g.x=ax+3.45f;g.z=az+.4f;update_interior();float before=g.x;move(-2.4f,0);
 ROOM_CHECK(g.x<before-2.2f,"player can walk from annex entrance to radio table");
 aim_at(HOME_RADIO_X,1.00f,HOME_RADIO_Z-.16f);interaction();
 ROOM_CHECK(interactKind==12,"radio prompt is attached to the radio on the table");
 ROOM_CHECK(strstr(prompt,"Радиоприёмник")!=0,"radio prompt is readable at the reachable approach point");
 /* Main flat kettle cabinet: real, small footprint; clear from the shared entry lane. */
 test_reset();physicsActor150=-1;g.playerY=0;float kx=hx[0]-1.0f,kz=hz[0]-1.30f;
 ROOM_CHECK(body_clear150(hx[0]-2.4f,hz[0]-.6f,0,.28f,0),"apartment entry remains clear beside new counter");
 ROOM_CHECK(!body_clear150(kx,kz,0,.28f,0),"kettle cabinet itself is a collidable fixture");
 int solidTypes[]={0,1,2,5,6,7,8,13,14,18,21};
 for(int h=0;h<NH;h++)for(int f=0;f<5;f++)for(int side=0;side<2;side++)for(int ai=0;ai<11;ai++)for(int bi=ai+1;bi<11;bi++){
  float x1,y1,z1,rx1,rz1,x2,y2,z2,rx2,rz2;object150(h,f,side,solidTypes[ai],&x1,&y1,&z1,&rx1,&rz1);object150(h,f,side,solidTypes[bi],&x2,&y2,&z2,&rx2,&rz2);
  int overlap=ab(x1-x2)<rx1+rx2-.03f&&ab(z1-z2)<rz1+rz2-.03f;roomChecks++;if(overlap){roomFailures++;if(roomFailures<20)printf("FAIL room overlap h%d f%d side%d types%d/%d\n",h,f,side,solidTypes[ai],solidTypes[bi]);}
 }
 printf("ROOMLAYOUT160: %d checks, %d failures\n",roomChecks,roomFailures);return roomFailures?1:0;}
