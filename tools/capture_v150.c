#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void pose150(float x,float z,float y,float tx,float ty,float tz){screen=0;help=0;seated=-1;simAccumulator=1.f/30;g.x=x;g.z=z;g.playerY=y;g.fallVelocity=0;update_interior();aim_at(tx,ty,tz);interaction();noticeTime=0;}
static void cap150(const char*n){char p[200];SN(p,sizeof(p),"qa/v150/%s.ppm",n);shot(p);printf("CAPTURE %s\n",n);fflush(stdout);}
int main(void){savepath="release_v150/capture.save";eventpath="release_v150/capture_events.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(1);g.hours=13.8f;g.weather=0;realTime=12;
 int h=2;float x=hx[h],z=hz[h];pose150(x+4,z-3,0,x-1,1.3f,z-1.6f);cap150("01_two_apartment_landing");
 pose150(x-2.4f,z+.45f,0,x-2.9f,1.3f,z+4.5f);cap150("02_studio_and_balcony_closed");
 g.balconyDoors150[h]|=1;pose150(x-2.8f,z+2.9f,0,x-2.8f,1.2f,z+5.9f);cap150("03_full_height_balcony_door_open");
 pose150(x-2.8f,z+5.5f,0,x-2.2f,1.0f,z+1.0f);cap150("04_walked_onto_balcony");
 pose150(x+2.5f,z+.4f,6,x+2.8f,7.2f,z+3.1f);cap150("05_other_apartment_upper_floor");
 pose150(x+11,z-12,0,x+2,6.5f,z-1);cap150("06_entry_and_facade");
 pose150(x+7.7f,z-4.4f,0,x+5.8f,1.3f,z-6);cap150("07_lift_lobby");
 g.hours=19;pose150(x-2.5f,z+15,0,x,7,z+3);cap150("08_warm_windows_evening");
 pose150(-11,-95,0,0,7,-121);cap150("09_connected_panel_row");
 g.hours=14;pose150(-74,34.3f,0,-74.6f,.9f,36);cap150("10_permanent_container_site");
 pose150(-8,4.9f,0,-6,1.2f,7.7f);cap150("11_snowman_collision_object");
 g.zoneFlags150=1;pose150(-167,-83.5f,0,-167,1.2f,-77);cap150("12_open_garage_interior");
 pose150(-37,-56,0,-29,1,-51);cap150("13_courtyard_parking");
 pose150(-21,-67,0,-25,1.4f,-60);cap150("14_old_sports_bars");
 pose150(hx[0]-2.4f,hz[0]+.4f,0,hx[0]-2.8f,1,hz[0]+4);screen=1;cap150("15_graphite_main_menu");screen=5;cap150("16_graphite_inventory");screen=11;cap150("17_graphite_map");
 dialogNPC=1;dialog_begin(1);screen=10;cap150("18_resident_identity_profile");
 test_reset();g.hours=0;for(int i=0;i<NN;i++){int a=PERSON(i).home;PERSON(i).x=hx[a]+7.55f;PERSON(i).z=hz[a]-9.5f;g.residentHome[i]=0;citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;}
 for(int k=0;k<2100;k++)life_tick(1.f/30);for(int i=0;i<NN;i++)if(PERSON(i).activity==15){int a=PERSON(i).home;pose150(hx[a]+(i/NH?2.8f:-2.8f),hz[a]+.9f,0,PERSON(i).x,.85f,PERSON(i).z+.8f);cap150("19_autonomous_sleep_in_bed");break;}
 g.hours=14;for(int k=0;k<2700;k++)life_tick(1.f/30);for(int i=0;i<NN;i++)if(sitting141[i]>.8f){float a=PERSON(i).x,b=PERSON(i).z;pose150(a-2,b-3.2f,0,a,.9f,b);cap150("20_live_simulation_seated_resident");break;}
 g.hours=22.6f;for(int k=0;k<120;k++)life_tick(1.f/30);float a,b;bench_position141(PERSON(1).home,&a,&b);pose150(a-2,b-3.2f,0,a,.9f,b);cap150("21_bench_after_resident_leaves");
 test_reset();g.hours=14;g.winter=winterEnabled=0;g.season=2;g.x=25;g.z=river_z(25);g.playerY=-1.48f;g.stamina=0;for(int k=0;k<300;k++)water_tick141(1.f/30);pose150(g.x,g.z,g.playerY,20,1,river_z(20)+2);cap150("22_surface_breathing_zero_stamina");keys[17]=1;for(int k=0;k<180;k++)water_tick141(1.f/30);pose150(g.x,g.z,g.playerY,30,-1.8f,river_z(30));cap150("23_submerged_oxygen");
 printf("V150 CAPTURES COMPLETE\n");return 0;}
