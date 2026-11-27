#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/d5.save";eventpath="build/d5.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 for(int k=0;k<POI205_COUNT;k++){Poi205*p=(Poi205*)&poi205[k];float z=p->z+2.9f;if(solid(p->x,z))printf("buried approach type%d %.1f %.1f\n",p->type,p->x,z);}
 float pts[][5]={{52,-32.4f,52,1.2f,-34.5f},{52,-32,52,1.2f,-34.5f}};
 for(int i=0;i<2;i++){g.x=pts[i][0];g.z=pts[i][1];g.playerY=0;aim_at(pts[i][2],pts[i][3],pts[i][4]);printf("i%d look=%d pick=%d prompt=[%s]\n",i,look_at(pts[i][2],pts[i][3],pts[i][4],.92f),s205_pick(),prompt);interaction();printf("  interaction prompt=[%s]\n",prompt);}
 T205X: ;
 int b0=g.beer,m=g.money;g.x=58;g.z=-34.1f;aim_at(58,1.2f,-36);interaction();printf("kiosk prompt=[%s]\n",prompt);
 street_use205(1);printf("money %d->%d beer %d->%d invbeer?\n",m,g.money,b0,g.beer);
 return 0;}
