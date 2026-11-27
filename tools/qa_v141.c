#define HEADLESS
#define main old_main141
#include "../src/main.c"
#undef main
static void empty141(void){for(int i=0;i<NN;i++){PERSON(i).x=190;PERSON(i).z=140;}}
static void steps141(float x,float z,int n){for(int k=0;k<n;k++)move(x,z);}
int main(void){savepath="release_v141/qa.save";eventpath="release_v141/qa_events.csv";audio_init();nextWorldSeed=140;test_reset();empty141();
 for(int h=0;h<NH;h++){
  g.x=hx[h]+7.55f;g.z=hz[h]-9.5f;g.playerY=0;g.fallVelocity=0;steps141(0,.05f,70);CHECK(ab(g.z-hz[h]+6)<.1f,"street entrance to enclosed lift lobby");
  steps141(-.05f,0,29);CHECK(ab(g.x-hx[h]-6.1f)<.1f&&residence_cabin(h,g.x,g.z),"public lobby to internal cabin, without crossing apartment");
  lift_request(30+h,4);for(int k=0;k<400;k++)update140_tick(1.f/30);CHECK(ab(g.playerY-12)<.02f,"internal lift carries passenger to top residential floor");
  steps141(.05f,0,29);CHECK(g.x>hx[h]+7.45f&&ab(g.playerY-12)<.03f,"leave cabin at matching upper landing");
  g.residence[h].doors|=1<<4;g.x=hx[h]+7.55f;g.z=hz[h]-3;g.playerY=12;steps141(-.05f,0,82);CHECK(g.x<hx[h]+3.6f,"actual upper apartment doorway is traversable when open");
  g.residence[h].doors&=~(1<<4);g.x=hx[h]+5.65f;g.z=hz[h]-3;g.playerY=12;steps141(-.05f,0,32);CHECK(g.x>hx[h]+5.05f,"closed apartment door stops player");
  g.x=hx[h]+7.35f;g.z=hz[h]-2.15f;g.playerY=12;steps141(0,.05f,89);steps141(-.05f,0,30);steps141(0,-.05f,89);CHECK(g.playerY>=14.98f&&g.fallVelocity==0,"real fifth-floor switchback stairs reach roof landing");
  steps141(0,-.05f,17);steps141(-.05f,0,38);CHECK(g.x<hx[h]+4.4f&&ab(g.playerY-15.16f)<.02f,"walk through headhouse door onto residence roof");
 }
 test_reset();empty141();for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++){
  panelIndex=p;float x=panel_cx(w),z=PANEL_Z;entryOpen=3;g.x=x-3.5f;g.z=z-6;g.playerY=0;g.fallVelocity=0;panelIndex=0;lift_request(p*2+w,99);for(int k=0;k<350;k++)panel_motion(1.f/30);CHECK(ab(g.playerY-9)<.02f,"panel lift capped at residential top floor, not roof");
  g.x=x-.8f;g.z=z+1.8f;g.playerY=9;steps141(0,.05f,88);steps141(.05f,0,32);steps141(0,-.05f,84);CHECK(g.playerY>11.8f&&g.fallVelocity==0,"panel top-floor stair flight exists and supports walking");
  steps141(-.05f,0,16);steps141(0,-.05f,44);CHECK(g.z<z+.5f&&ab(g.playerY-12.12f)<.04f,"panel roof access through stair headhouse doorway");
 }
 test_reset();int decorOK=1,benches=0;for(int n=0;n<envCount;n++){if(env[n].kind==0&&!outside141(env[n].x,env[n].z,.3f))decorOK=0;if(env[n].kind==6)benches++;}CHECK(decorOK,"all trees outside complete building footprints");CHECK(benches==54,"54 allocated entrance benches are indexed as solid geometry");
 int variants=0;for(int h=0;h<NH;h++)for(int f=0;f<5;f++){float d=divider141(h,f);variants|=d<0?1:d>.5f?4:2;}CHECK(variants==7,"three real partition layouts across addresses and floors");
 unsigned sd=g.worldSeed;float xx=hx[7],zz=hz[7],div=divider141(7,2);generate_world(sd);CHECK(hx[7]==xx&&hz[7]==zz&&divider141(7,2)==div,"same seed repeats lots and apartment plan");generate_world(sd+1);CHECK(hx[7]!=xx||hz[7]!=zz||divider141(7,2)!=div,"changed seed changes visible generation");
 test_reset();g.season=2;g.seasonDay=g.day;update140_tick(0);g.x=10;g.z=river_z(10);g.playerY=0;for(int k=0;k<120;k++)water_tick141(1.f/30);CHECK(g.playerY< -1.45f&&g.playerY> -1.65f,"physical swimming buoyancy holds body below waterline");
 keys[17]=1;for(int k=0;k<240;k++)water_tick141(1.f/30);CHECK(g.playerY< -2.2f&&swimAir141<10,"diving depletes air below the water surface");keys[17]=0;g.z=river_z(10)+4.4f;water_tick141(.1f);CHECK(g.playerY==0,"shore exit restores supported ground height");
 g.season=0;g.seasonDay=g.day;update140_tick(0);g.z=river_z(10);water_tick141(.1f);CHECK(ab(g.playerY+.18f)<.01f,"winter ice supports player above riverbed");CHECK(terrain_height141(0,river_z(0))==.36f,"bridges are supported above water level");
 test_reset();g.bank=1000;g.money=0;g.x=ATM_X;g.z=ATM_Z-1;bank_withdraw(100);CHECK(g.bank==900&&g.money==87,"ATM banking regression: 13 percent base commission");
 workPending=1;workHits=0;CHECK(bonus_duration()>=1.5f&&bonus_width()>=.17f,"skill checks have wider sector and slower sweep");workPending=-1;
 test_reset();int i=1;float bx,bz;bench_position141(PERSON(i).home,&bx,&bz);PERSON(i).x=bx-.58f;PERSON(i).z=bz-1.3f;g.hours=13;for(int k=0;k<180;k++)bench_route141(i,1.f/30);CHECK(sitting141[i]>.95f&&dst(PERSON(i).x,PERSON(i).z,bx-.58f,bz-.07f)<.11f,"elder walks into assigned bench seat and enters sitting pose");g.hours=22;CHECK(!bench_route141(i,.1f),"night schedule releases bench for return home");
 RigPose rm[23];rig_pose(rm,0,0,0,12);CHECK(rm[15].p.z>.3f&&rm[15].p.y<.6f&&rm[0].p.y<.5f,"seated skeleton bends hips and knees at bench height");
 test_reset();g.season=3;g.bank=781;g.x=10;g.z=river_z(10);g.playerY=-1.55f;save_game();CHECK(load_game()&&g.version==15&&g.bank==781&&g.season==3&&g.playerY< -1,"save roundtrip preserves economy and swimming location");
 test_reset();g.x=shopX;g.z=shopZ-8;g.playerY=0;empty141();steps141(0,.05f,100);CHECK(g.z>shopZ-3.1f,"shop front real central doorway admits player");g.x=shopX+3;g.z=shopZ-6.8f;steps141(0,.05f,36);CHECK(g.z<shopZ-6.0f,"shop display glazing blocks passage");
 printf("V141 TESTS: %d checks, %d failures\n",checks,failures);return failures?1:0;}
