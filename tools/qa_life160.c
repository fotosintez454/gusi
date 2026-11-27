#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="release_v160/life.save";eventpath="release_v160/life_events.csv";audio_init();nextWorldSeed=140;test_reset();int bad=0;g.hours=0;g.x=-15;g.z=-8;
 for(int i=0;i<NN;i++){int h=PERSON(i).home;PERSON(i).x=hx[h]+7.55f;PERSON(i).z=hz[h]-9.5f;g.residentHome[i]=0;citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;}
 for(int k=0;k<3600;k++)life_tick(1.f/30);int home=0,sleep=0;for(int i=0;i<NN;i++){home+=life_at_home(i);sleep+=PERSON(i).activity==15;}printf("NIGHT after 120 simulated seconds: inside %d/48, sleeping %d/48\n",home,sleep);if(home!=NN||sleep!=NN)bad++;
 for(int i=0;i<NN;i++)if(PERSON(i).activity!=15)printf("NOT ASLEEP i%d stage%d at %.2f %.2f local %.2f %.2f\n",i,g.residentHome[i],PERSON(i).x,PERSON(i).z,PERSON(i).x-hx[PERSON(i).home],PERSON(i).z-hz[PERSON(i).home]);fflush(stdout);
 g.hours=14;for(int k=0;k<3600;k++)life_tick(1.f/30);int seatedCount=0;for(int i=0;i<NN;i++)seatedCount+=sitting141[i]>.8f;printf("DAY after 120 simulated seconds: %d autonomously seated\n",seatedCount);if(seatedCount<3)bad++;
 g.hours=23;for(int k=0;k<3600;k++)life_tick(1.f/30);int benches=0;for(int i=0;i<NN;i++)benches+=npcBench141[i]>=0;printf("NIGHT: %d occupied bench reservations after return\n",benches);if(benches)bad++;
 printf("LIFE160: 3 scenario checks, %d failures\n",bad);return bad?1:0;}
