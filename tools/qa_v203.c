#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int count203,failed203,miss203;
#define T203(c,m) do{count203++;if(!(c)){failed203++;if(failed203<25)printf("FAIL203 %s line %d at %.2f %.2f %.2f\n",m,__LINE__,g.x,g.playerY,g.z);}}while(0)
static void away203(void){for(int i=0;i<NN;i++){PERSON(i).x=185;PERSON(i).z=135;}local_init190();for(int i=0;i<LOCAL190;i++){g.locals190[i].x=185;g.locals190[i].z=135;g.locals190[i].y=39;}navStatic150=1;physicsActor150=-1;}
static int drawn_floor203(float x,float y,float z){for(int n=0;n<floorCount203;n++){FloorRect203*r=&floorRects203[n];if(ab(x-r->x)<=r->w*.5f+.015f&&ab(z-r->z)<=r->d*.5f+.015f&&ab(y-r->y)<.105f)return 1;}return 0;}
static void audit_rails203(void){for(int n=0;n<railCount203;n++){RailPost203*r=&railPosts203[n];T203(drawn_floor203(r->x,r->base,r->z),"railing post is anchored on an actual tread");}}
static void audit_point203(float x,float y,float z){if(!body_clear150(x,z,y,.17f,0))return;g.x=x;g.z=z;g.playerY=y;int ok=drawn_floor203(x,y,z);if(!ok&&miss203++<15)printf("UNCOVERED203 %.3f %.3f %.3f rects=%d\n",x,y,z,floorCount203);T203(ok,"walkable floor point has an authored slab or tread");}
int main(void){savepath="build/test203.save";eventpath="build/test203.csv";audio_init();nextWorldSeed=140;test_reset();away203();set_resolution(0);g.weather=0;
 // Sample the actual collision-clear rooms, public landings and stair treads, every level.
 floorAudit203=1;
 for(int h=0;h<NH;h++)for(int f=0;f<5;f++){g.x=camx=hx[h];g.z=camz=hz[h];g.playerY=f*3;camy=g.playerY+1.64f;yaw=pitch=0;camera();floorCount203=railCount203=0;residence_upper_draw(h);audit_rails203();
 for(float a=-4.4f;a<4.5f;a+=.8f)for(float b=-3.8f;b<4.2f;b+=.8f)audit_point203(hx[h]+a,f*3,hz[h]+b);
 for(float a=5.3f;a<8.1f;a+=.6f)for(float b=-8.f;b<3.2f;b+=.6f){g.playerY=f*3;float y=residence_ground(h,hx[h]+a,hz[h]+b,f*3);if(y> -900&&!residence_cabin(h,hx[h]+a,hz[h]+b))audit_point203(hx[h]+a,y,hz[h]+b);}}
 for(int p=0;p<PANEL_COUNT;p++)for(int f=0;f<PANEL_FLOORS;f++){panelIndex=p;g.x=camx=panel_origin_x(p);g.z=camz=panel_origin_z(p);g.playerY=f*3;camy=g.playerY+1.64f;camera();floorCount203=railCount203=0;panel_one_draw();audit_rails203();for(int w=0;w<2;w++)for(float a=-6.4f;a<6.5f;a+=.8f)for(float b=-7.2f;b<7.3f;b+=.8f){panelIndex=p;float x=panel_cx(w)+a,z=PANEL_Z+b;g.playerY=f*3;float y=panel_ground(x,z,f*3);if(y> -900&&!panel_cabin(w,x,z))audit_point203(x,y,z);}}
 panelIndex=0;
 for(int n=0;n<4;n++)for(int f=0;f<high_floors180(n);f++){g.x=camx=cityBlockX[n];g.z=camz=cityBlockZ[n];g.playerY=f*3;camy=g.playerY+1.64f;camera();floorCount203=railCount203=0;high_draw180();audit_rails203();for(float a=-7.2f;a<7.3f;a+=.8f)for(float b=-4.8f;b<5.f;b+=.8f){float x=cityBlockX[n]+a,z=cityBlockZ[n]+b;g.playerY=f*3;float y=high_ground180(n,x,z,f*3);if(y> -900)audit_point203(x,y,z);}}
 floorAudit203=0;navStatic150=0;
 // Old saves cannot send a four-storey lift to the roof.
 for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++){g.district[p].y[w]=12;g.district[p].target[w]=12;}g.x=185;g.z=135;g.playerY=0;panel_load();for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++)T203(panelRuntime[p].y[w]<=9&&panelRuntime[p].target[w]<=9,"legacy lift height clamps below the roof");
 for(int h=0;h<NH;h++){g.residence[h].y=0;T203(residence_ground(h,hx[h]+6.1f,hz[h]-6,6)<-900,"no phantom residential floor in empty shaft");}
 for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;liftY[0]=0;T203(panel_ground(panel_cx(0)-3.5f,PANEL_Z-6,6)<-900,"no phantom panel floor in empty shaft");}panelIndex=0;
 for(int n=0;n<4;n++){g.highY180[n]=0;T203(high_ground180(n,cityBlockX[n],cityBlockZ[n]-5.7f,6)<-900,"no phantom highrise floor in empty shaft");}
 props_init150();int garages=0,brick=0;for(int n=0;n<propCount150;n++)if(props150[n].type==4||props150[n].type==11){garages++;brick+=props150[n].type==11;T203(body_clear150(props150[n].x,props150[n].z,0,.2f,0)==0,"closed garage has matching collision");float roof;T203(garage_roof203(props150[n].x,props150[n].z,&roof)&&roof>2.7f&&roof<3,"garage visible roof is supported");}T203(garages==24&&brick==8,"sixteen usable garages plus eight new brick garages");T203(props150[53].type==8&&props150[54].type==9&&props150[56].type==8&&props150[57].type==9,"persistent ride indices unchanged");
 // JSON serialization and strict app-ID handling work without Discord or Windows.
 char id[24],json[1400];T203(discord_id203("123456789012345678\n",id),"real-length application ID accepted");T203(!discord_id203("0",id)&&!discord_id203("1234not-a-token",id)&&!discord_id203("123456789012345678901",id),"disabled and invalid IDs never connect");int len=discord_payload203(json,sizeof(json),123,7,1700000000,"Окно \"кухня\"","День 2 / Зима",0);T203(len>0&&discord_has203(json,"SET_ACTIVITY")&&discord_has203(json,"\\\"кухня\\\""),"presence is bounded escaped JSON");FILE*j=fopen("build/discord203_payload.json","w");fwrite(json,1,len,j);fclose(j);T203(discord_payload203(json,8,123,8,0,"a","b",0)==0,"small output buffer safely rejected");T203(discord_payload203(json,sizeof(json),123,9,0,"","",1)>0&&discord_has203(json,"\"activity\":null"),"shutdown clears activity");
 // Real software renders for review.
 test_reset();away203();navStatic150=0;set_resolution(2);g.winter=0;g.season=2;winterEnabled=0;g.weather=0;g.hours=14;noticeTime=0;
 g.x=hx[0]+7.35f;g.z=hz[0]-1.2f;g.playerY=.30f;aim_at(hx[0]+6.7f,2.1f,hz[0]+1.8f);shot("qa/v203/01_residence_stairs.ppm");
 g.x=panel_origin_x(0)-7+.9f;g.z=panel_origin_z(0)+3;g.playerY=2.7f;aim_at(panel_origin_x(0)-7-.3f,2.5f,panel_origin_z(0)+4);shot("qa/v203/02_panel_stairs.ppm");
 g.x=panel_origin_x(0)-10.5f;g.z=panel_origin_z(0)-6;g.playerY=12.12f;aim_at(panel_origin_x(0)-7,13.4f,panel_origin_z(0)+4.25f);shot("qa/v203/03_roof_no_lift.ppm");
 g.x=-169;g.z=-57;g.playerY=0;aim_at(-173,1.3f,-43);shot("qa/v203/04_garages.ppm");
 g.x=cityBlockX[0]-.87f;g.z=cityBlockZ[0]-1.2f;g.playerY=.3f;g.highDoors180[0]|=1;aim_at(cityBlockX[0]-.2f,2.0f,cityBlockZ[0]+1.5f);shot("qa/v203/05_highrise.ppm");
 printf("V203: %d checks, %d failures, %d uncovered floor points\n",count203,failed203,miss203);return !!failed203;
}
