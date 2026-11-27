#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
int main(void){savepath="release_v095/night.save";eventpath="release_v095/night_events.csv";audio_init();test_reset();set_resolution(2);g.x=-29;g.z=-1;g.hours=1;aim_at(-34.5f,4.7f,-6.4f);update_interior();interaction();shot("qa/v095/09_night_lamps.ppm");return 0;}
