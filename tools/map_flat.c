#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
/* ASCII reachability map of the player's home floor.
   '#' wall/furniture (point solid), '.' standable (r=.28), 'o' reachable from spawn, 'S' spawn. */
static unsigned char st[400][400];
int main(int argc,char**argv){savepath="build/map.save";eventpath="build/map.csv";audio_init();nextWorldSeed=140;test_reset();physicsActor150=-1;
 int h=argc>1?atoi(argv[1]):0;float fl=argc>2?atof(argv[2]):0;g.playerY=fl;
 float x0=hx[h]-15.5f,x1=hx[h]+9.f,z0=hz[h]-9.5f,z1=hz[h]+5.5f,step=.1f;int W=(int)((x1-x0)/step),H=(int)((z1-z0)/step);
 float sx=g.x,sz=g.z;if(h!=0){sx=hx[h]-2.4f;sz=hz[h]-1.0f;}
 for(int j=0;j<H;j++)for(int i=0;i<W;i++){float x=x0+i*step,z=z0+j*step;st[j][i]=solid(x,z)?1:(body_clear150(x,z,fl,.28f,0)?2:0);}
 int si=(int)((sx-x0)/step),sj=(int)((sz-z0)/step);static int q[160000];int qh=0,qt=0;if(st[sj][si]==2){st[sj][si]=3;q[qt++]=sj*400+si;}
 while(qh<qt){int c=q[qh++],j=c/400,i=c%400;int d[4][2]={{1,0},{-1,0},{0,1},{0,-1}};for(int k=0;k<4;k++){int a=j+d[k][0],b=i+d[k][1];if(a<0||b<0||a>=H||b>=W)continue;if(st[a][b]==2){st[a][b]=3;q[qt++]=a*400+b;}}}
 printf("house %d floor y=%.1f spawn %.2f %.2f (rel %.2f %.2f) x0=%.1f z0=%.1f step=%.1f\n",h,fl,sx,sz,sx-hx[h],sz-hz[h],x0-hx[h],z0-hz[h],step);
 for(int j=H-1;j>=0;j-=2){for(int i=0;i<W;i+=1){char c=st[j][i]==1?'#':st[j][i]==2?'.':st[j][i]==3?'o':' ';if(j/2==sj/2&&i==si)c='S';if(ab(x0+i*step-hx[h])<.05f)c=c=='#'?'#':'|';putchar(c);}printf("  z=%+.1f\n",z0+j*step-hz[h]);}
 return 0;}
