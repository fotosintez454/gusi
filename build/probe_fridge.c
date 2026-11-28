#include <stdio.h>
#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
/* Probe: sweep the fridge offset along the entry wall and report which offsets satisfy
   the qa160/qa_repairs160 contract (door faces +b, 0.70 m standing space, no overlap). */
int main(void){
 savepath="/tmp/okno_probe.save";eventpath="/tmp/okno_probe.csv";audio_init();nextWorldSeed=140;test_reset();
 for(int i=0;i<NN;i++){PERSON(i).x=189;i=i;PERSON(i).z=140;}physicsActor150=-1;
 for(float ax=1.38f;ax<=1.38f;ax+=.02f){
  int bad=0,badDoor=0,badStove=0,badJam=0,badWalk=0,worst;
  for(int h=0;h<NH;h++)for(int f=0;f<5;f++)for(int s=0;s<2;s++){
   float fx,fy,fz,frx,frz,sx,sy,sz,srx,srz,tx,ty,tz,trx,trz;
   g.playerY=f*3;object150(h,f,s,0,&fx,&fy,&fz,&frx,&frz);
   float shift=(s?1:-1)*(ax-1.38f);fx+=shift;
   float kx,ky,kz,krx,krz;object150(h,f,s,7,&sx,&sy,&sz,&srx,&srz);object150(h,f,s,1,&kx,&ky,&kz,&krx,&krz);
   if(ab(fx-kx)<=frx+krx&&ab(fz-kz)<=frz+krz)badStove++;
   object150(h,f,s,14,&tx,&ty,&tz,&trx,&trz);
   int okDoor=body_clear150(fx,fz+.70f,f*3,.28f,0);
   int okWalk=body_clear150(fx,fz+.45f,f*3,.28f,0);
   int okStove=ab(fx-sx)>frx+srx||ab(fz-sz)>frz+srz;
   int okToilet=ab(fx-tx)>frx+trx&&ab(fz-tz)>frz+trz;
   int okJam=ab(fx-hx[h])+frx<=.75f+.001f;   /* fridge stays outside the 1.30 m door reveal (edge 1.75 m) */
   (void)okToilet;
   if(!okDoor)badDoor++;if(!okStove)badStove++;if(!okJam)badJam++;if(!okWalk)badWalk++;
   if(!(okDoor&&okWalk&&okStove&&okJam))bad++;
  }
  worst=bad+badDoor+badStove+badJam+badWalk;
  printf("a=%.2f bad=%d door=%d walk=%d stove=%d jamb=%d\n",ax,bad,badDoor,badWalk,badStove,badJam);
  (void)worst;
 }
 return 0;
}
