#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(){savepath="release_v141/extra.save";eventpath="release_v141/extra_events.csv";audio_init();nextWorldSeed=140;test_reset();
 g.hours=0;for(int i=0;i<NN;i++){int h=PERSON(i).home;PERSON(i).x=hx[h]+7.55f;PERSON(i).z=hz[h]-9.5f;citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;g.residentHome[i]=0;}
 for(int k=0;k<1200;k++)life_tick(1.f/30);int indoors=0;for(int i=0;i<NN;i++)indoors+=life_at_home(i);printf("Night residents indoors %d / 48\n",indoors);CHECK(indoors>=44,"night return route uses real doors with three partition layouts");
 g.hours=13;for(int k=0;k<1800;k++)life_tick(1.f/30);int sitting=0;for(int i=0;i<NN;i++)sitting+=sitting141[i]>.8f;printf("Daytime NPCs sitting %d\n",sitting);CHECK(sitting>=4,"day schedule autonomously seats residents, not only capture poses");
 test_reset();float lo=2,hi=0;for(int d=1;d<50;d++){g.day=d;g.absolute=d*24.f;update140_tick(0);lo=mn(lo,g.economy);hi=mx(hi,g.economy);}CHECK(lo<1&&hi>1,"economy still rises and falls over seeded day cycle");
 test_reset();g.lootMask=1;g.lootRefill[0]=g.absolute+1;g.absolute+=2;update140_tick(0);CHECK(!(g.lootMask&1),"trash refill deadline still replenishes items");
 int roads=1,trees=1;short path[MAXPATH];for(int s=0;s<8;s++){nextWorldSeed=140+s;test_reset();for(int n=0;n<envCount;n++)if(env[n].kind==0&&!outside141(env[n].x,env[n].z,1.8f))trees=0;for(float x=-125;x<126;x+=3)for(int q=-1;q<=1;q+=2)if(!traffic_clear140(x,q*2.75f))roads=0;CHECK(nav_plan(-20,-9,shopX,shopZ-7,path)>0,"store is reachable from street across seed sample");}
 CHECK(trees,"sampled seeds never put new trees inside building footprint");CHECK(roads,"full-sized car bodies fit main road across seed sample");
 test_reset();for(int i=0;i<NN;i++){PERSON(i).x=190;PERSON(i).z=140;}g.x=hx[1]+4.8f;g.z=hz[1]+4;g.playerY=15.16f;move(0,2);for(int k=0;k<120;k++)update140_tick(1.f/30);CHECK(screen==24&&g.deaths==1,"roof edge fall still triggers death exactly once");
 printf("V141 EXTRA TESTS: %d checks, %d failures\n",checks,failures);return failures?1:0;}
