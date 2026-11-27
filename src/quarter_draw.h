/* Shared quarter POIs, not streamed chunks. All kiosks have an actual shop interaction. */
#include "quarter_sign.h"
static void quarter_details(void){for(int n=0;n<PANEL_COUNT;n++){float x,z;quarter_shop(n,&x,&z);if(dst(camx,camz,x,z)>viewdist+10)continue;
 surfaceTex=&mat_pavement;box(x,.015f,z+3.5f,8,.04f,5,0x838777);box(x,.025f,z-1.8f,4.4f,.06f,3.8f,0x757b6a);surfaceTex=&mat_metal;U32 color=n%3==0?0x687365:n%3==1?0x737764:0x727b7a;
 box(x,.62f,z,3.2f,1.24f,2.4f,color);box(x,1.7f,z+1.15f,3.2f,2.2f,.14f,color);for(int s=-1;s<=1;s+=2){box(x+s*1.55f,1.7f,z,.13f,2.2f,2.4f,color);box(x+s*.84f,1.7f,z-1.19f,1.35f,1.05f,.12f,color);}box(x,2.43f,z-1.18f,3.2f,.45f,.13f,color);box(x,2.78f,z,3.7f,.16f,2.9f,0x4b584b);surfaceTex=&mat_wood;box(x,1.22f,z-1.32f,1.1f,.10f,.45f,0x8c8060);surfaceTex=0;
 box(x,1.83f,z+.96f,.83f,1.08f,.04f,0x222c22);for(int k=0;k<3;k++){box(x-.26f+k*.24f,1.4f,z+.7f,.12f,.26f,.10f,0x8d9b73);box(x-.23f+k*.24f,1.9f,z+.7f,.13f,.17f,.12f,0xaf9e6b);}plane(x,2.43f,z-1.27f,2.5f,.4f,0xffffff,&quarterSign);
 for(int side=-1;side<=1;side+=2){box(x+side*6.8f,.04f,z+3.8f,3,.06f,4.4f,0x70786b);box(x+side*6.8f,.04f,z+6.1f,2.4f,.08f,1.1f,0x717c6d);}
 /* Controlled surface marks and litter only in designated patches; no random world scatter. */
 for(int k=0;k<5;k++){float px=x+4.3f+k*.31f,pz=z+5.1f+(k%2)*.22f;box(px,.065f,pz,.20f,.045f,.13f,k%2?0x97937c:0x6e7665);}surfaceTex=0;}}
