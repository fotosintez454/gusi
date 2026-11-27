/* One simulated hour, headless fixed 4 Hz. Not a real-time playtest. */
#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
int main(void){savepath="release_v095/soak.save";eventpath="release_v095/soak_events.csv";audio_init();test_reset();g.x=-20;g.z=-9;int errors=0,collisions=0,checksRun=0;for(int step=0;step<14400;step++){/* Economy/needs remain live: an unattended player may receive medical care. */update_interior();simulate(.25f);checksRun++;if(!(g.health>=0&&g.health<=100&&g.hunger>=0&&g.hunger<=100&&g.thirst>=0&&g.thirst<=100&&g.petHunger>=0&&g.petHunger<=100))errors++;for(int n=0;n<NN;n++)if(!nav_clear(PERSON(n).x,PERSON(n).z))collisions++;if(step%1200==0){int money=g.money;save_game();if(!load_game()||g.money!=money)errors++;}}
 printf("{\"simulated_seconds\":3600,\"hz\":4,\"steps\":%d,\"invariant_failures\":%d,\"npc_collision_samples\":%d,\"salary\":%d,\"money\":%d,\"utilities\":%d,\"day\":%d,\"hours\":%.3f,\"human_playtest\":false}\n",checksRun,errors,collisions,g.salary,g.money,g.utilities,g.day,g.hours);return errors||collisions?1:0;}
