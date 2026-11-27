#define PANEL_FLOORS 4 /* walk-up Soviet panel block: two flats per floor, eight per entrance */
#define PANEL_COUNT 15
static int panelIndex=0;
static float panel_origin_x(int n){static const float x[15]={-126,-126,-126,-126,-126,-126,120,120,120,120,120,120,-20,14,-20};return x[n];}
static float panel_origin_z(int n){static const float z[15]={-124,-80,-35,22,63,124,-124,-80,-35,22,63,124,124,124,-124};return z[n];}
#define PANEL_X (panel_origin_x(panelIndex))
#define PANEL_Z (panel_origin_z(panelIndex))
typedef struct{float y[2],target[2],wait[2],angles[20];int door[2];unsigned int flats,entries;} PanelRuntime;
static PanelRuntime panelRuntime[PANEL_COUNT];
#define liftY (panelRuntime[panelIndex].y)
#define liftTarget (panelRuntime[panelIndex].target)
#define liftWait (panelRuntime[panelIndex].wait)
#define liftDoor (panelRuntime[panelIndex].door)
#define flatOpen (panelRuntime[panelIndex].flats)
#define entryOpen (panelRuntime[panelIndex].entries)
#define flatAngles (panelRuntime[panelIndex].angles)
static float panelWindowAngle[PANEL_COUNT][40],residenceWindowAngle[NH][24],residenceDoorAngle[NH][5],residenceFrontDoorAngle[NH];
static unsigned char panelStoveOn[PANEL_COUNT][16];
static float panelKettleHeat[PANEL_COUNT][16];
static int liftUI=0;
static unsigned long long*panel_windows(void){return panelIndex?&g.district[panelIndex].windows:&g.panelWindowOpen;}
static float panel_cx(int wing){return PANEL_X+(wing?7:-7);}
static int panel_one_wing(float x,float z){if(ab(z-PANEL_Z)>8.3f)return -1;for(int w=0;w<2;w++)if(ab(x-panel_cx(w))<7.15f)return w;return -1;}
static int panel_floor(void){return (int)clamp((float)floor((g.playerY+.15f)/3),0,PANEL_FLOORS-1);}
static void panel_one_reset(void){memset(panelStoveOn[panelIndex],0,sizeof(panelStoveOn[panelIndex]));memset(panelKettleHeat[panelIndex],0,sizeof(panelKettleHeat[panelIndex]));for(int n=0;n<2;n++){liftY[n]=liftTarget[n]=liftWait[n]=0;liftDoor[n]=1;}flatOpen=0;entryOpen=0;memset(flatAngles,0,sizeof(flatAngles));liftUI=0;}
static int panel_cabin(int w,float x,float z){return ab(x-(panel_cx(w)-3.5f))<1.05f&&ab(z-(PANEL_Z-6))<1.0f;}
static int panel_bath_closed(int w,int f,int side){int id=w*8+f*2+(side>0);return !!(g.district[panelIndex].entries&(1u<<(2+id)));}
static int panel_one_solid(float x,float z){int w=panel_one_wing(x,z);if(w<0)return 0;
 float a=x-panel_cx(w),b=z-PANEL_Z;int f=panel_floor();
 for(int k=0;k<PANEL_FLOORS;k++)if(rail_hit203(a,b,g.playerY,-.18f,2.125f,5.875f,k*3+.09375f,k*3+1.5f)||rail_hit203(a,b,g.playerY,.18f,2.125f,5.875f,k*3+3,k*3+1.59375f))return 1;
 if(ab(a)>6.72f||b>7.72f)return 1;
 if(b< -7.72f&&(ab(a)>1.18f||g.playerY>.3f||!(entryOpen&(1u<<w))))return 1;
 /* Two apartment walls with real doorway gaps and hinged state. */
 if(ab(ab(a)-1.8f)<.27f&&b> -4.9f&&b<7.6f){int side=a>0,bit=w*(PANEL_FLOORS*2)+f*2+side;if(ab(b)>.85f||!(flatOpen&(1u<<bit)))return 1;}
 /* Apartment fronts keep public foyer separated. */
 if(ab(b+5)<.25f&&ab(a)>2.15f)return 1;
 /* Restored shaft; side door faces the public entrance lobby. */
 if(a> -4.85f&&a< -2.15f&&b> -7.25f&&b< -4.35f){
 if(a< -4.6f||b< -7.05f||b> -4.95f)return 1;
 if(a> -2.4f&&(ab(b+6)>.82f||!liftDoor[w]||ab(liftY[w]-f*3)>.15f))return 1;
 return 0;}
 /* Mirrored 63m2 flats: entry corridor, wet room, kitchen, bedroom. */
 float aa=ab(a);
 if(aa>1.95f&&aa<6.7f){if(ab(b-2.3f)<.13f&&(aa<2.85f||aa>4.65f))return 1;
 if(ab(aa-3.85f)<.13f&&b> -4.9f&&b< -1.15f&&(b< -3.65f||b> -2.35f||panel_bath_closed(w,f,a>0?1:-1)))return 1;
 if(ab(b+1.15f)<.13f&&aa<3.85f)return 1;
 if(aa>4.35f&&aa<5.85f&&b>4.05f&&b<6.3f)return 1;
 if(aa>6.0f&&b> -4.8f&&b< -3.15f)return 1;
 if(aa>5.65f&&aa<6.7f&&b> -1.05f&&b<.25f)return 1;
 if(aa>4.73f&&aa<5.97f&&b> -3.14f&&b< -2.26f)return 1;
 if(aa>5.09f&&aa<5.61f&&b> -1.91f&&b< -1.39f)return 1;}

 return 0;
}
static float panel_ground(float x,float z,float old){int w=panel_one_wing(x,z);if(w<0)return ab(old)<.3f?0:-999;float a=x-panel_cx(w),b=z-PANEL_Z;
if(panel_cabin(w,x,z))return ab(old-liftY[w])<.35f?liftY[w]:-999;
float best=-999,dist=1e20f;
 if(ab(a)<1.55f&&b>=1.85f){for(int f=0;f<PANEL_FLOORS;f++){float h=-999;if(b>6&&b<7.7f)h=f*3+1.5f;else if(b<=12&&a<-.10f)h=f*3+clamp((b-2)/4,0,1)*1.5f;else if(b<=12&&a>.10f)h=f*3+3-clamp((b-2)/4,0,1)*1.5f;if(h>=0&&ab(h-old)<dist){best=h;dist=ab(h-old);}}if(dist<.34f)return best;return ab(old)<.3f?0:-999;}
 for(int f=0;f<PANEL_FLOORS;f++)if(ab(f*3-old)<dist){best=f*3;dist=ab(best-old);}return dist<.34f?best:-999;
}

static void lift_one_request(int w,int floor){if(w<0||w>1)return;liftTarget[w]=clamp(floor,0,PANEL_FLOORS-1)*3;liftDoor[w]=0;liftWait[w]=.6f;audio(4);}
static void panel_one_motion(float dt){for(int j=0;j<16;j++)panelKettleHeat[panelIndex][j]=clamp(panelKettleHeat[panelIndex][j]+dt*(panelStoveOn[panelIndex][j]?.10f:-.06f),0,1);for(int k=0;k<40;k++){float target=(*panel_windows()&(1ull<<k))?1.25f:0;panelWindowAngle[panelIndex][k]+=clamp(target-panelWindowAngle[panelIndex][k],-dt*2.1f,dt*2.1f);}for(int k=0;k<20;k++)flatAngles[k]=lerp(flatAngles[k],flatOpen&(1u<<k)?1.0f:0,mn(1,dt*9));for(int w=0;w<2;w++){
 int rider=panel_cabin(w,g.x,g.z)&&ab(g.playerY-liftY[w])<.3f;
 if(liftWait[w]>0){liftWait[w]=mx(0,liftWait[w]-dt);continue;}
 float delta=liftTarget[w]-liftY[w];if(ab(delta)>.005f){liftY[w]+=clamp(delta,-dt*1.3f,dt*1.3f);if(rider)g.playerY=liftY[w];}
 else if(!liftDoor[w]){liftY[w]=liftTarget[w];if(rider)g.playerY=liftY[w];liftDoor[w]=1;audio(4);event_log("lift_arrived",w);}
}}
static int panel_one_interact(void){int w=panel_one_wing(g.x,g.z);float best=1e20f;int wing=-1;for(int n=0;n<2;n++){float d=dst(g.x,g.z,panel_cx(n),PANEL_Z-8);if(d<best){best=d;wing=n;}}
 if(best<1.8f&&g.playerY<.3f&&look_at(panel_cx(wing),1.4f,PANEL_Z-8,.8f)){entryOpen^=1u<<wing;audio(0);event_log(entryOpen&(1u<<wing)?"door_opened":"door_closed",2000+panelIndex*22+20+wing);event_log("open_door",2000+panelIndex*22+20+wing);nav_refresh(g.x,g.z);return 1;}
 if(w<0)return 0;
 int floor=panel_floor();for(int side=-1;side<=1;side+=2){float bx=panel_cx(w)+side*3.85f,bz=PANEL_Z-3;if(dst(g.x,g.z,bx,bz)<2.1f&&look_at(bx,floor*3+1.12f,bz,.93f)){if(!panel_bath_closed(w,floor,side)&&ab(g.x-bx)<.35f)return 1;int id=w*8+floor*2+(side>0);g.district[panelIndex].entries^=1u<<(2+id);audio(12);nav_refresh(g.x,g.z);return 1;}}
 int unusedFloor=floor; (void)unusedFloor;for(int side=-1;side<=1;side+=2){int id=w*8+floor*2+(side>0);float sx=panel_cx(w)+side*6.35f,sz=PANEL_Z-4;if(dst(g.x,g.z,sx,sz)<2.5f&&look_at(sx,floor*3+.8f,sz,.96f)){panelStoveOn[panelIndex][id]^=1;audio(0);event_log(panelStoveOn[panelIndex][id]?"stove_on":"stove_off",panelIndex*16+id);say(panelStoveOn[panelIndex][id]?"Плита включена. Чайник нагревается.":"Плита выключена.");return 1;}}for(int k=0;k<4;k++){float wx=panel_cx(w)+(k<2?-1:1)*(k%2?6:4);if(g.z<PANEL_Z+7.6f&&dst(g.x,g.z,wx,PANEL_Z+7.85f)<2.65f&&look_at(wx,floor*3+1.7f,PANEL_Z+7.85f,.965f)){int id=w*PANEL_FLOORS*4+floor*4+k;work_interact(200+panelIndex*40+id);return 1;}}
float a=g.x-panel_cx(w),b=g.z-PANEL_Z;int f=panel_floor();
for(int side=-1;side<=1;side+=2)if(dst(g.x,g.z,panel_cx(w)+side*1.8f,PANEL_Z)<2.1f&&look_at(panel_cx(w)+side*1.8f,f*3+1.25f,PANEL_Z,.88f)){int bit=w*(PANEL_FLOORS*2)+f*2+(side>0);flatOpen^=1u<<bit;audio(0);event_log(flatOpen&(1u<<bit)?"door_opened":"door_closed",2000+panelIndex*22+bit);event_log("open_door",2000+panelIndex*22+bit);nav_refresh(g.x,g.z);return 1;}return 0;
}

static int panel_one_prompt(void){for(int n=0;n<2;n++)if(g.playerY<.3f&&dst(g.x,g.z,panel_cx(n),PANEL_Z-8)<1.9f&&look_at(panel_cx(n),1.4f,PANEL_Z-8,.8f)){SN(prompt,sizeof(prompt),"Подъезд / открыть или закрыть дверь");return 1;}int w=panel_one_wing(g.x,g.z);if(w<0)return 0;float a=g.x-panel_cx(w),b=g.z-PANEL_Z;for(int side=-1;side<=1;side+=2){float bx=panel_cx(w)+side*3.85f,bz=PANEL_Z-3;if(dst(g.x,g.z,bx,bz)<2.1f&&look_at(bx,panel_floor()*3+1.12f,bz,.93f)){SN(prompt,sizeof(prompt),"Санузел / открыть или закрыть дверь");return 1;}}for(int side=-1;side<=1;side+=2){float sx=panel_cx(w)+side*6.35f,sz=PANEL_Z-4;if(dst(g.x,g.z,sx,sz)<2.5f&&look_at(sx,panel_floor()*3+.8f,sz,.96f)){SN(prompt,sizeof(prompt),"ЛКМ — включить / выключить плиту");return 1;}}for(int k=0;k<4;k++){float wx=panel_cx(w)+(k<2?-1:1)*(k%2?6:4);if(g.z<PANEL_Z+7.6f&&dst(g.x,g.z,wx,PANEL_Z+7.85f)<2.65f&&look_at(wx,panel_floor()*3+1.7f,PANEL_Z+7.85f,.965f)){SN(prompt,sizeof(prompt),"Окно / открыть или закрыть");return 1;}}for(int side=-1;side<=1;side+=2)if(dst(g.x,g.z,panel_cx(w)+side*1.8f,PANEL_Z)<2.1f&&look_at(panel_cx(w)+side*1.8f,panel_floor()*3+1.25f,PANEL_Z,.88f)){SN(prompt,sizeof(prompt),"ЛКМ — дверь квартиры");return 1;}if(b< -6.4f&&ab(a)<1.6f){SN(prompt,sizeof(prompt),"Подъезд / дверь");return 1;}return 0;}


#include "district.h"
