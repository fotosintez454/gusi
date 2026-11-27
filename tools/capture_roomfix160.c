#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="release_v160/roomfix.save";eventpath="release_v160/roomfix_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=16.4f;g.weather=0;realTime=12;screen=0;g.playerY=0;g.x=HOME_RADIO_X;g.z=HOME_RADIO_Z-2.05f;update_interior();aim_at(HOME_RADIO_X,1.28f,HOME_RADIO_Z-.05f);interaction();shot("qa/v160/11_radio_annex_roomfix.ppm");
 screen=0;g.x=hx[0]-2.1f;g.z=hz[0]+1.4f;g.playerY=0;update_interior();aim_at(hx[0]-1.1f,1.0f,hz[0]+3.0f);interaction();shot("qa/v160/12_bedroom_roomfix.ppm");
 screen=0;g.x=hx[0]-2.4f;g.z=hz[0]-1.0f;g.playerY=0;update_interior();aim_at(hx[0]-1.0f,1.02f,hz[0]-1.30f);interaction();shot("qa/v160/14_home_kitchen_roomfix.ppm");
 screen=0;g.x=panel_origin_x(2)+7+4.3f;g.z=panel_origin_z(2)-1.5f;g.playerY=0;update_interior();aim_at(panel_origin_x(2)+7+5.8f,1.0f,panel_origin_z(2)-4.0f);interaction();shot("qa/v160/13_panel_kitchen_roomfix.ppm");return 0;}
