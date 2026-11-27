#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int checks201,fail201;
#define Q(c,m) do{checks201++;if(!(c)){fail201++;printf("FAIL201 %s\n",m);fflush(stdout);}}while(0)
static void people_away201(void){for(int i=0;i<NN;i++){PERSON(i).x=180;PERSON(i).z=130;}local_init190();for(int i=0;i<LOCAL190;i++){g.locals190[i].x=180;g.locals190[i].z=130;g.locals190[i].y=39;}g.x=180;g.z=-130;g.playerY=0;navStatic150=0;physicsActor150=-1;}
static void eye201(float x,float z,float y,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=y;aim_at(tx,ty,tz);screen=0;interaction();}
int main(void){savepath="build/test201.save";eventpath="build/test201.csv";audio_init();nextWorldSeed=140;test_reset();people_away201();
 for(int n=0;n<7;n++){carX140[n]=-90+n*25;carZ140[n]=(n&1)?2.75f:-2.75f;}
 float before[7];for(int n=0;n<7;n++)before[n]=carX140[n];traffic_tick140(.25f);for(int n=0;n<7;n++)Q((carX140[n]-before[n])*traffic_heading201(n)>0,"each traffic car moves toward its model front");
 float x=carX140[0],z=carZ140[0];Q(!nav_clear(x,z),"legacy resident cannot enter moving car footprint");Q(!local_clear190(0,x,z,0),"additional resident cannot enter moving car footprint");Q(!traffic_ped_clear201(x+2.9f,z,0,.25f),"pedestrian waits before entering car's short predicted path");Q(traffic_ped_clear201(x,z+1.8f,0,.25f),"sidewalk beyond vehicle width remains walkable");Q(traffic_ped_clear201(x,z,3,.25f),"cars do not block residents upstairs");navStatic150=1;Q(nav_clear(x,z),"static navigation map does not permanently bake moving cars");navStatic150=0;
 // Both kinds of walkers cross the actual two-way traffic simulation in small steps.
 people_away201();for(int n=0;n<7;n++){carX140[n]=110+n*2;carZ140[n]=30;}
 carX140[0]=-40;carZ140[0]=-2.75f;carX140[1]=40;carZ140[1]=2.75f;
 PERSON(0).x=-10;PERSON(0).z=-6;g.locals190[0].x=10;g.locals190[0].z=6;g.locals190[0].y=0;int intersections=0;
 for(int frameNo=0;frameNo<1800;frameNo++){float dt=1.f/30;physicsActor150=0;float nz=mn(6,PERSON(0).z+dt*1.1f);if(nav_clear(PERSON(0).x,nz))PERSON(0).z=nz;physicsActor150=-1;local_move190(0,10,-6,1.18f,dt);traffic_tick140(dt);for(int n=0;n<2;n++){if(ab(PERSON(0).x-carX140[n])<2.3f&&ab(PERSON(0).z-carZ140[n])<1.13f+.20f)intersections++;if(ab(g.locals190[0].x-carX140[n])<2.3f&&ab(g.locals190[0].z-carZ140[n])<1.13f+.17f)intersections++;}}
 Q(intersections==0,"sixty-second two-way road crossing has zero car/pedestrian intersections");Q(PERSON(0).z>5.8f&&g.locals190[0].z<-5.8f,"both resident populations finish crossing without deadlock or teleport");
 people_away201();carX140[0]=-30;carZ140[0]=-2.75f;PERSON(0).x=-25.5f;PERSON(0).z=-2.75f;x=carX140[0];traffic_tick140(1);Q(carX140[0]==x,"traffic stops before a resident ahead, including a long frame");
 props_init150();for(int i=0;i<propCount150;i++)if(props150[i].type==5)Q(props150[i].rx>=2.30f&&props150[i].rz>=1.13f&&props150[i].height>=2.04f,"parked vehicle collision covers visible body, wheels and van roof");
 for(int h=0;h<NH;h++)for(int f=0;f<5;f++)for(int side=0;side<2;side++){float rx,rz,xx,yy,zz;object150(h,f,side,19,&xx,&yy,&zz,&rx,&rz);float door=hx[h]+(side?2.8f:-2.8f);Q(ab(xx-door)>.65f+.40f+.1f,"radiator and pipes stay outside balcony door span");Q(ab(xx-hx[h])+.40f<4.8f,"relocated radiator stays inside exterior wall");}
 V3 unit=cig_unit201(vp(0,1,0));Q(ab(unit.y-1)<.0001f,"cigarette axis handles a vertical pose without NaNs");
 // Deterministic scenes for visual review. No postprocessed/mock imagery.
 test_reset();people_away201();set_resolution(2);g.weather=0;realTime=12;g.hours=22;for(int i=0;i<240;i++)g.things150[i]&=~8u;
 eye201(hx[0]-14,hz[0]+35,7.5f,hx[0]+1,7.2f,hz[0]+4.5f);shot("qa/v201/01_windows_far.ppm");
 eye201(hx[0]-5,hz[0]+10,0,hx[0]-2.2f,1.7f,hz[0]+4.48f);shot("qa/v201/02_windows_near.ppm");
 g.hours=16.4f;eye201(hx[0]-2.7f,hz[0]+2.4f,0,hx[0]-3.5f,.9f,hz[0]+4.05f);shot("qa/v201/03_balcony_radiator.ppm");
 people_away201();carX140[0]=-8;carZ140[0]=-2.75f;carX140[1]=8;carZ140[1]=2.75f;eye201(0,13,0,0,1,-.4f);shot("qa/v201/04_two_way_cars.ppm");
 g.hours=14;eye201(180,130,0,180,1.4f,135);itemKind=4;itemAnim=consume_duration()*.5f;consumeCommitted=1;shot("qa/v201/05_cigarette_hand.ppm");itemAnim=0;
 camx=0;camy=1.2f;camz=0;yaw=pitch=0;camera();lighting_reset();clear_scene(.8f);cigarette141(vp(-.052f,1.2f,.30f),vp(.052f,1.2f,.30f),1);screenshot("qa/v201/06_cigarette_model.ppm");
 printf("V201: %d checks, %d failures\n",checks201,fail201);return !!fail201;
}
