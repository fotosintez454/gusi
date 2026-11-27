#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int count160=0,fail160=0;
#define T160(condition,label) do{count160++;if(!(condition)){fail160++;printf("FAIL h=%d f=%d s=%d: %s [%.2f %.2f] prompt=%s\n",hh,ff,ss,label,g.x,g.z,prompt);fflush(stdout);}}while(0)
#define GX160 21
#define GZ160 29
#define GN160 (GX160*GZ160)
static unsigned char walk160[GN160];static int parent160[GN160],queue160[GN160],hh=0,ff=0,ss=0;
static float px160(int n){return hx[hh]+(ss?1:-1)*(.35f+(n%GX160)*.2f);}
static float pz160(int n){return hz[hh]-1.55f+(n/GX160)*.2f;}
static int edge160(int a,int b){return body_clear150((px160(a)+px160(b))*.5f,(pz160(a)+pz160(b))*.5f,ff*3,.28f,0);}
static int plan160(void){int start=-1;float best=1e10f;for(int n=0;n<GN160;n++){walk160[n]=body_clear150(px160(n),pz160(n),ff*3,.28f,0);parent160[n]=-2;float d=dst(px160(n),pz160(n),g.x,g.z);if(walk160[n]&&d<best){best=d;start=n;}}
 if(start<0)return 0;g.x=px160(start);g.z=pz160(start);int head=0,tail=0;queue160[tail++]=start;parent160[start]=-1;
 while(head<tail){int n=queue160[head++],next[]={n-1,n+1,n-GX160,n+GX160};for(int k=0;k<4;k++){int m=next[k];if(m<0||m>=GN160||k==0&&n%GX160==0||k==1&&n%GX160==GX160-1||!walk160[m]||parent160[m]!=-2||!edge160(n,m))continue;parent160[m]=n;queue160[tail++]=m;}}
 return tail;
}
static int reach160(float x,float z,float radius){int best=-1;float d0=1e10f;for(int n=0;n<GN160;n++)if(parent160[n]!=-2){float d=dst(x,z,px160(n),pz160(n));if(d<radius&&d<d0){d0=d;best=n;}}return best;}
static int walk_to160(int n){if(n<0)return 0;int rev[GN160],len=0;for(int a=n;a>=0&&len<GN160;a=parent160[a])rev[len++]=a;if(!len)return 0;g.x=px160(rev[len-1]);g.z=pz160(rev[len-1]);for(int k=len-2;k>=0;k--){float x=px160(rev[k]),z=pz160(rev[k]);move(x-g.x,z-g.z);if(dst(g.x,g.z,x,z)>.035f)return 0;}return 1;}
int main(void){savepath="release_v160/test.save";eventpath="release_v160/events.csv";audio_init();nextWorldSeed=140;test_reset();
 T160(seated==-1,"new game clears seated state");T160(high_at180(cityBlockX[0],cityBlockZ[0])>=0&&!city_solid(cityBlockX[0]-8.2f,cityBlockZ[0]),"visible background buildings block movement only inside their actual footprint");T160(body_clear150(g.x,g.z,g.playerY,.28f,0),"fresh spawn fits full player capsule, not only a point");float x=g.x,z=g.z;move(.3f,0);T160(dst(x,z,g.x,g.z)>.25f,"can immediately walk from spawn");
 for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}physicsActor150=-1;
 for(hh=0;hh<NH;hh++)for(ff=0;ff<5;ff++)for(ss=0;ss<2;ss++){
 int id=unit150(hh,ff,ss);g.things150[id]|=(1u<<16)|(1u<<17);g.x=hx[hh]+(ss?2.4f:-2.4f);g.z=hz[hh]-.6f;g.playerY=ff*3;g.fallVelocity=0;T160(body_clear150(g.x,g.z,g.playerY,.28f,0),"flat safe entry is clear");int reachable=plan160();T160(reachable>100,"kitchen, living and bathroom share a connected walkable component");
 int types[]={0,5,7,8,9,14,18};for(int k=0;k<7;k++){float a,b,c,rx,rz;object150(hh,ff,ss,types[k],&a,&b,&c,&rx,&rz);int cell=reach160(a,c,types[k]==9?.9f:1.15f);T160(cell>=0,"full-body route to essential object exists");if(cell>=0)T160(walk_to160(cell),"actual swept movement follows path without sticking");}float fx,fy,fz,frx,frz,tx,ty,tz,trx,trz;object150(hh,ff,ss,0,&fx,&fy,&fz,&frx,&frz);object150(hh,ff,ss,14,&tx,&ty,&tz,&trx,&trz);T160(ab(fx-tx)>frx+trx&&ab(fz-tz)>frz+trz,"fridge and toilet keep separate non-overlapping footprints");T160(body_clear150(fx,fz+.70f,ff*3,.28f,0),"player can stand in front of the room-facing refrigerator door");
 float bx=hx[hh]+(ss?2.8f:-2.8f),bz=hz[hh]+3.7f;int goal=reach160(bx,bz,.45f);T160(goal>=0&&walk_to160(goal),"walk all the way up to balcony/window opening");
 // Exercise the real hit-test and state change from an accessible cell, not a teleport into furniture.
 float a,b,c,rx,rz;object150(hh,ff,ss,7,&a,&b,&c,&rx,&rz);int cell=reach160(a,c,1.15f);if(cell>=0){walk_to160(cell);aim_at(a,b,c);unsigned before=g.things150[id];int used=use150(1);T160(used&&((before^g.things150[id])&(1u<<7)),"stove can actually be switched from a reachable position");}
 // Each apartment door remains closed-solid and open-passable in common landing.
 float doorx=hx[hh]+(ss?2.4f:-2.4f);g.x=doorx;g.z=hz[hh]-2.7f;g.flatDoors150[hh]&=~(1u<<(ff*2+ss));move(0,1.3f);T160(g.z<hz[hh]-2.1f,"closed apartment door blocks");g.flatDoors150[hh]|=1u<<(ff*2+ss);move(0,1.3f);T160(g.z>hz[hh]-1.6f,"open apartment door admits player");
 }
 hh=ff=ss=0;test_reset();for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}physicsActor150=-1;
 // Reproduce the reported problem and repair using the same load-time recovery helper.
 float ax,ay,az,rx,rz;object150(0,0,0,6,&ax,&ay,&az,&rx,&rz);g.x=ax;g.z=az;T160(!body_clear150(g.x,g.z,0,.28f,0),"test really begins inside a chair");safe_spawn160();T160(body_clear150(g.x,g.z,0,.28f,0),"inside-chair recovery finds a free capsule location");
 g.playerY=0;g.x=hx[0]-2.3f;g.z=hz[0]+.4f;g.things150[0]&=~(1u<<17);move(0,1.1f);T160(g.z<hz[0]+.65f,"closed living-room door really stops the player");g.things150[0]|=1u<<17;move(0,1.1f);T160(g.z>hz[0]+1.4f,"opened living-room door admits the full player body");
 g.x=hx[0]-2.6f;g.z=hz[0]-.7f;g.things150[0]&=~(1u<<16);move(-1,0);T160(g.x>hx[0]-2.95f,"closed bathroom door really blocks");g.things150[0]|=1u<<16;move(-.8f,0);T160(g.x<hx[0]-3.4f,"opened bathroom door is usable");
 g.x=hx[0]-2.7f;g.z=hz[0]+2.57f;object150(0,0,0,6,&ax,&ay,&az,&rx,&rz);aim_at(ax,ay,az);int usedChair=use150(1);T160(usedChair&&seated==1000,"can actually sit on the moved chair");bench_stand();T160(seated==-1&&body_clear150(g.x,g.z,0,.28f,0),"standing up leaves a free full-body position");
 g.x=hx[0]-2.4f;g.z=hz[0]+2.2f;player_sleep(2);T160(lastSleepHours==2&&body_clear150(g.x,g.z,0,.28f,0),"sleep and wake remain usable beside the moved bed");
 short path[MAXPATH];g.x=-20;g.z=-8;for(int h=0;h<NH;h++)T160(nav_plan(-20,-8,hx[h]+7.55f,hz[h]-9.5f,path)>0,"district path to every residence");for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++)T160(nav_plan(-20,-8,panel_origin_x(p)+(w?7:-7),panel_origin_z(p)-9.3f,path)>0,"district path to every long-block entrance");
 // Assert kiosk/bin infrastructure stays on dry land and outside playable buildings.
 for(int n=0;n<15;n++){float a,b;quarter_shop(n,&a,&b);T160(ab(b-river_z(a))>6.8f,"kiosk is not in river");loot_position(n*2,&a,&b);T160(ab(b-river_z(a))>6.8f,"permanent containers are not in river");}
 g.winter=0;g.x=30;g.z=river_z(30);g.playerY=-1.48f;g.stamina=0;g.health=100;swimAir141=100;keys[17]=keys[32]=0;for(int k=0;k<18000;k++)water_tick141(1.f/30);T160(swimAir141==100&&g.health==100,"ten minutes afloat without stamina or oxygen loss");keys[17]=1;for(int k=0;k<180;k++)water_tick141(1.f/30);T160(swimAir141<55,"intentional submerged dive uses oxygen");keys[32]=1;for(int k=0;k<120;k++)water_tick141(1.f/30);T160(swimAir141==100,"Space overrides Ctrl and restores air");keys[17]=keys[32]=0;
 test_reset();g.things150[0]|=(1u<<18)|(1u<<20);g.things150[0]&=~(1u<<17);unsigned fl=g.things150[0];g.x=hx[0]-2.4f;g.z=hz[0]-.6f;save_game();seated=1000;T160(load_game()&&seated==-1&&g.things150[0]==fl,"load clears seating and preserves doors/shower/phone");T160(body_clear150(g.x,g.z,g.playerY,.28f,0),"loaded spawn fits capsule");
 printf("V160: %d checks, %d failures\n",count160,fail160);return fail160?1:0;
}
