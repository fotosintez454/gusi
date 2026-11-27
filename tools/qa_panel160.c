#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
#define PX160 23
#define PZ160 61
#define PN160 (PX160*PZ160)
static unsigned char open160[PN160];static int prev160[PN160],q160[PN160];static float ox160,oz160,sg160;static int checks160,bad160;
static float xx160(int n){return ox160+sg160*(2.1f+(n%PX160)*.2f);}
static float zz160(int n){return oz160-4.6f+(n/PX160)*.2f;}
static int panel_walk160(float x,float z){int goal=-1;float best=1e10f;for(int n=0;n<PN160;n++)if(prev160[n]!=-2){float d=dst(xx160(n),zz160(n),x,z);if(d<best){best=d;goal=n;}}if(goal<0||best>.85f)return 0;int rev[PN160],len=0;for(int n=goal;n>=0&&len<PN160;n=prev160[n])rev[len++]=n;g.x=xx160(rev[len-1]);g.z=zz160(rev[len-1]);for(int k=len-2;k>=0;k--){float x=xx160(rev[k]),z=zz160(rev[k]);move(x-g.x,z-g.z);if(dst(g.x,g.z,x,z)>.04f)return 0;}return 1;}
int main(void){savepath="release_v160/panel.save";eventpath="release_v160/panel_events.csv";audio_init();nextWorldSeed=140;test_reset();for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}physicsActor150=-1;
 for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++)for(int f=0;f<PANEL_FLOORS;f++)for(int side=-1;side<=1;side+=2){ox160=panel_origin_x(p)+(w?7:-7);oz160=panel_origin_z(p);sg160=side;g.playerY=f*3;g.fallVelocity=0;g.district[p].entries=0;
 int start=-1;float best=1e10f;for(int n=0;n<PN160;n++){open160[n]=body_clear150(xx160(n),zz160(n),f*3,.28f,0);prev160[n]=-2;float d=dst(xx160(n),zz160(n),ox160+side*2.6f,oz160);if(open160[n]&&d<best){start=n;best=d;}}
 int head=0,tail=0;if(start>=0){q160[tail++]=start;prev160[start]=-1;}while(head<tail){int n=q160[head++],nxt[]={n-1,n+1,n-PX160,n+PX160};for(int k=0;k<4;k++){int m=nxt[k];if(m<0||m>=PN160||k==0&&n%PX160==0||k==1&&n%PX160==PX160-1||!open160[m]||prev160[m]!=-2)continue;if(!body_clear150((xx160(n)+xx160(m))*.5f,(zz160(n)+zz160(m))*.5f,f*3,.28f,0))continue;prev160[m]=n;q160[tail++]=m;}}
 float target[][2]={{5.4f,-4.3f},{2.8f,-3},{3.6f,3.3f},{4,6.9f},{6,6.9f}};for(int k=0;k<5;k++){checks160++;if(!panel_walk160(ox160+side*target[k][0],oz160+target[k][1])){bad160++;printf("FAIL p%d w%d f%d side%d target%d\n",p,w,f,side,k);}}
 }
 printf("PANEL160: %d actual-movement route checks, %d failures\n",checks160,bad160);return bad160?1:0;}
