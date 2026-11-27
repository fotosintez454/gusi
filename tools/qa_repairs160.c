#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main

static int repChecks=0,repFailures=0;
#define CHECK160(v,msg) do{repChecks++;if(!(v)){repFailures++;printf("FAIL: %s\n",msg);}}while(0)

int main(void){
 savepath="release_v160/repairs_test.save";eventpath="release_v160/repairs_events.csv";audio_init();nextWorldSeed=57291;test_reset();
 CHECK160(high_at180(cityBlockX[0],cityBlockZ[0])>=0&&!city_solid(cityBlockX[0],cityBlockZ[0]),"former skyline box now contains an actual stairwell");
 CHECK160(!city_solid(cityBlockX[0]-8.2f,cityBlockZ[0]),"background block collision does not extend beyond visual footprint");
 CHECK160(footstep_sound160(1,1,1,1)==8,"indoors use muted indoor footsteps");
 CHECK160(footstep_sound160(0,0,0,1)==9&&footstep_sound160(0,0,0,0)==10,"dry pavement and earth have different footsteps");
 CHECK160(footstep_sound160(0,0,1,1)==16&&footstep_sound160(0,0,1,0)==17,"rain changes outdoor footsteps by surface");
 CHECK160(footstep_sound160(0,1,0,1)==11&&footstep_sound160(0,1,1,0)==18,"dry snow and wet snow/slush have different footsteps");
 CHECK160(footstep_hard_surface160(0,0)&&footstep_hard_surface160(0,43)&&footstep_hard_surface160(94,20),"main and cross streets resolve to hard-surface footsteps");
 CHECK160(footstep_hard_surface160(0,7.6f),"sidewalk strip resolves to hard-surface footsteps");
 CHECK160(!footstep_hard_surface160(0,18),"unmarked grass courtyard does not incorrectly resolve as asphalt");
 CHECK160(footstep_hard_surface160(hx[0]+7.55f,(hz[0]-8.6f+path_target160(0))*.5f),"small-house entrance path resolves to hard-surface footsteps");
 {float bx,bz;bench_position141(0,&bx,&bz);CHECK160(footstep_hard_surface160(bx,bz-.35f),"courtyard bench concrete pad resolves to hard-surface footsteps");}
 {float px=panel_origin_x(0),pz=panel_origin_z(0)-9.4f;CHECK160(footstep_hard_surface160(px,pz),"panel entrance apron resolves to hard-surface footsteps");}
 int oldOverlap=0,oldApproach=0;
 for(int h=0;h<NH;h++)for(int f=0;f<5;f++)for(int side=0;side<2;side++){
  float fx,fy,fz,frx,frz,tx,ty,tz,trx,trz;g.playerY=f*3;
  object150(h,f,side,0,&fx,&fy,&fz,&frx,&frz);object150(h,f,side,14,&tx,&ty,&tz,&trx,&trz);
  if(ab(fx-tx)<frx+trx&&ab(fz-tz)<frz+trz)oldOverlap=1;
  if(!body_clear150(fx,fz+.70f,f*3,.28f,0))oldApproach=1;
 }
 CHECK160(!oldOverlap,"old-flat toilet never intersects refrigerator volume");
 CHECK160(!oldApproach,"old-flat refrigerator has reachable standing space at the front");
 int panelWindowBlocked=0,panelFixtureOverlap=0,panelBathDoorFailed=0;
 for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++)for(int f=0;f<PANEL_FLOORS;f++)for(int side=-1;side<=1;side+=2){
  panelIndex=p;g.playerY=f*3;panelRuntime[p].entries=0;panelRuntime[p].flats=65535;
  float cx=panel_cx(w),ox=cx+side*6.18f,oz=PANEL_Z-.4f;
  float tx=cx+side*2.75f,tz=PANEL_Z-4.05f;
  if(ab(ox-tx)<.49f||ab(oz-tz)<.65f)panelFixtureOverlap=1;
  if(!body_clear150(cx+(w?4:-4),PANEL_Z+7.35f,f*3,.28f,0))panelWindowBlocked=1;
  float bx=cx+side*3.85f;
  if(panel_one_solid(bx,PANEL_Z-3))panelBathDoorFailed=1;
 }
 CHECK160(!panelFixtureOverlap,"panel-flat toilet and refrigerator are separated in the mirrored layouts");
 CHECK160(!panelWindowBlocked,"approach corridor to each panel-flat window is walkable");
 CHECK160(!panelBathDoorFailed,"open bathroom door gap stays passable across panel flats");
 printf("REPAIRS160: %d checks, %d failures\n",repChecks,repFailures);
 return repFailures?1:0;
}
