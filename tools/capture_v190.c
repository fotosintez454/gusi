#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static void cap190(const char*name,float x,float z,float y,float tx,float ty,float tz){g.x=x;g.z=z;g.playerY=y;g.fallVelocity=0;screen=0;update_interior();aim_at(tx,ty,tz);interaction();shot(name);printf("CAPTURE %s\n",name);fflush(stdout);}
int main(void){savepath="build/capture190.save";eventpath="build/capture190.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);g.hours=15;g.weather=0;realTime=10;showHints=0;for(int n=0;n<NN;n++){PERSON(n).x=189;PERSON(n).z=140;}local_init190();g.highEntry180=15;for(int n=0;n<4;n++){g.highDoors180[n]=0xffffff;g.highLightTouched180[n]=0xffffff;g.highLightOff180[n]=0;}for(int n=0;n<ANIMAL190;n++){g.animals190[n].x=-180;g.animals190[n].z=-135;}
 cap190("qa/v190/01_two_entrances.ppm",117,-22,0,74,17,18);
 cap190("qa/v190/02_bathroom.ppm",cityBlockX[0]+6.21f,cityBlockZ[0]-3.72f,9,cityBlockX[0]+7.35f,10,cityBlockZ[0]-4.2f);
 g.highWindows180[0]|=1u<<9;
 cap190("qa/v190/03_balcony.ppm",cityBlockX[0]+4.8f,cityBlockZ[0]+3.75f,12,cityBlockX[0]+4.8f,13.25f,cityBlockZ[0]+7.2f);
 cap190("qa/v190/04_pharmacy_front.ppm",-24,-71,0,-24,1.55f,-60);
 cap190("qa/v190/05_pharmacy_inside.ppm",-24,-61.5f,0,-24,1.25f,-57.5f);
 Local190*c=&g.locals190[0];c->x=-18;c->z=9.45f;c->y=0;c->yaw=PI;c->stage=21;c->role=0;for(int n=8;n<13;n++){Animal190*a=&g.animals190[n];a->x=-18+(n-10)*.40f;a->z=8.3f+(n%2)*.55f;a->y=0;a->yaw=n;a->phase=n;}
 c=&g.locals190[4];c->x=-20.2f;c->z=10;c->y=0;c->yaw=PI;c->stage=21;c->role=4;g.animals190[0].x=-20;g.animals190[0].z=8.2f;g.animals190[0].yaw=PI;g.animals190[0].y=0;
 cap190("qa/v190/06_residents_and_animals.ppm",-13.4f,4.8f,0,-18.2f,1.0f,9.4f);
 c=&g.locals190[5];c->x=-16;c->z=-19;c->y=0;c->yaw=PI;c->stage=21;c->role=5;
 g.x=-12;g.z=-19;for(int n=0;n<50;n++){float z=-25+n*.17f;track_add190(-13.5f+(float)sin(n*.17f)*.3f,z,0,0,0);}g.clean190[0]=.35f;
 cap190("qa/v190/07_janitor_and_snow.ppm",-20,-15,0,-16,1.0f,-19);
 for(int p=0;p<propCount150;p++)if(props150[p].type==2&&p%4!=3){c=&g.locals190[6];c->x=props150[p].x+1.5f;c->z=props150[p].z;c->y=0;c->yaw=0;c->stage=21;c->role=6;cap190("qa/v190/08_exercise.ppm",c->x+4.4f,c->z+4.0f,0,c->x,1.2f,c->z);break;}
 for(int p=0;p<propCount150;p++)if(props150[p].type==5){cap190("qa/v190/09_vehicle.ppm",props150[p].x+4.6f,props150[p].z-4.6f,0,props150[p].x,1,props150[p].z);break;}
 return 0;}
