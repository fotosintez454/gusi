#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
static void pose(float x,float z,float y){g.x=x;g.z=z;g.playerY=y;g.rot=yaw=0;g.look=pitch=0;screen=0;update_interior();prompt[0]=0;noticeTime=0;}
int main(int argc,char**argv){savepath="release_v092/capture.save";eventpath="release_v092/capture_events.csv";audio_init();test_reset();set_resolution(2);
 if(argc>1){set_resolution(0);pose(18,3,0);hudMode=2;g.apprentices=3;FILE*out=popen("ffmpeg -v error -y -f rawvideo -pixel_format bgr0 -video_size 800x450 -framerate 5 -i - -an -c:v libx264 -preset veryfast -crf 23 -pix_fmt yuv420p qa/v092/NPC_HEADLESS_60s.mp4","w");if(!out)return 2;for(int f=0;f<300;f++){tick(.1f);tick(.1f);render_frame();panel(0,H-30,W,30,0x20251d,.98f);text(12,H-24,"OKNO 0.9.2 / 60s FIXED CAMERA / 3 APPRENTICES / HEADLESS",.31f,0xe3dfcd);text(12,H-12,"AUTOMATED SIMULATION - NOT WINDOWS RUNTIME OR HUMAN PLAYTEST",.26f,0xb4b19b);fwrite(frame,4,W*H,out);}return pclose(out);}
 screen=1;shot("qa/v092/01_main.ppm");screen=3;click_action(32,1);shot("qa/v092/02_job_accepted.ppm");click_action(32,1);shot("qa/v092/03_route.ppm");
 pose(hx[0]+1.4f,hz[0]-3.25f,0);aim_at(hx[0]+1.4f,.28f,hz[0]-2.1f);pressed[1]=1;tick(.016f);pressed[1]=0;g.petHunger=20;pose(catX,catZ-1.5f,0);aim_at(catX,.35f,catZ);pressed[1]=1;tick(.016f);pressed[1]=0;click_action(33,0);shot("qa/v092/04_pet_fed.ppm");
 pose(catX-.2f,catZ-1.5f,0);aim_at(catX,.35f,catZ);hudMode=2;shot("qa/v092/05_hryukot.ppm");
 pose(hx[1]-3,hz[1]-3,0);aim_at(hx[1]-3,.96f,hz[1]-1.7f);pressed[1]=1;tick(.016f);pressed[1]=0;shot("qa/v092/06_wallet.ppm");
 screen=7;g.apprentices=3;for(int a=0;a<3;a++)g.apprenticeXP[a]=30;shot("qa/v092/07_crew.ppm");screen=5;shot("qa/v092/08_inventory.ppm");
 pose(-20,-9,0);hudMode=0;shot("qa/v092/09_district.ppm");screen=6;optionsTab=3;shot("qa/v092/10_options.ppm");
 pose(hx[1]-2.8f,hz[1]+2.8f,0);aim_at(hx[1]-2.8f+.62f,1.72f,hz[1]+4.48f-.12f);int id=3;WIN(id).handle=0;WIN(id).angle=0;toggle_window(id);bonusActive=0;shot("qa/v092/11_window_start.ppm");simulate(.3f);shot("qa/v092/12_window_half.ppm");simulate(.3f);shot("qa/v092/13_window_open.ppm");return 0;
}
