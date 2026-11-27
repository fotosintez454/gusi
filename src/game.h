#include "flat_layout.h"
#include "engine.h"
static int rail_hit203(float a,float b,float y,float x,float z0,float z1,float y0,float y1){if(ab(a-x)>.055f||b<z0||b>z1)return 0;float base=lerp(y0,y1,(b-z0)/(z1-z0));return y>base-.8f&&y<base+.90f;}
#define NH 24
#define NW 72
#define NN 48
#define OX (-48.f)
#define OZ (-35.f)
#define KX 48.f
#define KZ (-15.f)
#define NJ 3
static const char*names[NH]={"Твой дом","Лидия","Семён","Аркадий","Рита","Валентин","Нина","Степан","Игорь","Галина","Марк","Вера","Борис","Юля","Денис","Зоя","Алексей","Тимур","Саша","Павел","Лена","Кирилл","Лев","Женя"};
static const char*tasknames[]={"Проветрить комнату","Окно перед работой","Окно в санузле","Перед приходом гостей","Перед отпуском","Ночная смена","Сквозняк по инструкции","Срочный вызов"};
static float hx[NH]={-30,-10,10,30,-30,-10,10,30};
static float hz[NH]={-22,-22,-22,-22,22,22,22,22};
/* The starter radio belongs to the connected living-room annex, on its table. */
#define HOME_RADIO_X (hx[0]-9.6f)
#define HOME_RADIO_Z (hz[0]+.4f)
#define HOME_TOILET_X (hx[0]+4.1f)
#define HOME_TOILET_Z (hz[0]+.12f)
typedef struct{float angle,handle,wear;int mode;} WindowState;
typedef struct{int house,type,mode,count,done,mask,pay;float deadline;int apprentice;} Job;
typedef struct{float x,z,tx,tz,facing,wait;int home,activity;float mood;} Citizen;
typedef struct{unsigned int doors,entries;unsigned long long windows;float y[2],target[2],wait[2];} DistrictSave;
typedef struct{unsigned int doors;unsigned long long windows;float y,target,wait;} ResidenceSave;
typedef struct{float x,z,y,yaw,timer,phase,walked;int home,floor,side,role,stage,step,goal,cycle;} Local190;
typedef struct{float x,z,y,yaw,tx,tz,timer,phase;int kind,owner,mode;} Animal190;
typedef struct{float x,z,y,yaw,age;unsigned char type;} Track190;
#define LOCAL190 40
#define ANIMAL190 32
#define TRACK190 8192
typedef struct{float x,y,z,vx,vy,vz,yaw;int kind,active;} Body200;
#define BODY200 96
typedef struct{
 unsigned int magic,version;float x,z,rot,look;float hours,absolute;int day;
 float hunger,thirst,energy,health,bladder,bowel,illness,alcohol,stress;
 int money,reputation,completed,rank,food,water,feed,medicine,cigarettes,beer;
 float petHunger,petBond,petx,petz;
 int apprentices;float apprenticeXP[3],apprenticeTimer[3];int autoWork[3];
 int utilities;float nextBill,crimeHeat,lateDiscovery,weatherTimer;int weather;
 WindowState windows[24];Job jobs[NJ];Citizen people[16];
 int activeJob,stolenMask,daysPaid;float played;int settings[7];float tuning[3];
 unsigned int worldSeed;float stamina;int options[12];float config[4];
 int salary;WindowState extraWindows[48];Citizen extraPeople[32];
 float lifeHunger[NN],lifeEnergy[NN],lifeWallet[NN];int worldRevision;
 int winter,focusMode,uiMotion,popupMode,highContrast,fastMode;float hudOpacity,mapSize,mapOpacity,mapZoom;unsigned int lootMask;float catBond;
 int bottles,scrap,invSlots[48],quick[2],invReady,resourceDay;unsigned int bonusMask[3];
 int homeFeedTaken,radioOn,radioTrack;float playerY;unsigned int panelSavedDoors,panelSavedEntries;float panelSavedY[2],panelSavedTarget[2],panelSavedWait[2];
 int uiRevision;unsigned long long panelWindowOpen;
 DistrictSave district[15];ResidenceSave residence[NH];
 unsigned int districtBonus[50];int districtBonusDay;
 /* v13 append-only expansion; v12 prefix remains byte-compatible. */
 int featureRevision,bizLevel[4],bizCapital,bizReceivable,bizRevenue,bizCosts,bizDay,bizLastGross,bizLastCost,homeLevel,herb,herbDay;float intox;char tvURL[256];
 int workRevision,workTarget[3];
 int update140,bank,deaths,season,seasonDay,economyDay;float economy,atmFee,fallVelocity,fallStart;
 float lootRefill[30];int residentHome[48];float drowning;
 int revision150;unsigned int flatDoors150[NH],balconyDoors150[NH],things150[240];int foodDay150[240];unsigned int zoneFlags150;

unsigned int highDoors180[4],highWindows180[4],highLightOff180[4],highLightTouched180[4],highEntry180;float highY180[4],highTarget180[4],highWait180[4];
int revision190,civicDoors190,hairDay190;Local190 locals190[LOCAL190];Animal190 animals190[ANIMAL190];Track190 tracks190[TRACK190];int trackHead190;float trackClock190,clean190[2];
int revision200; Body200 bodies200[BODY200]; float kettle200[240]; unsigned char kettleOn200[240],chairMoved200[240]; unsigned stairWindows200[NH+15+4];
} State;
static State g;
static int room204=-1,roomHouse204=-1,lamp204=1,ignoreProp204=-1;
static float roomProgress204=0,roomAuto204=0,roomReturnX204,roomReturnZ204,roomReturnY204,roomReturnYaw204,roomReturnPitch204;
static void room_reset204(void);static void spawn_repair204(void);static int npc_clear204(float x,float z,float y,int actor);static int local_seated204(int n);static void local_seat_tick204(int n,float dt);static int public_prop204(int n);static int room_use204(int perform);static int room_controls204(float dt);static void room_tick204(float dt);

static int seated=-1;static int rideIndex202=-1;static float ridePhase202[6],rideAmp202[6],rideOmega202[6],renderAhead202=0;
static void reset202(void);static int ride_start202(int n);static int ride_exit202(void);static int ride_controls202(float dt);static void ride_tick202(float dt);static void ride_camera202(void);static void tracks_tick202(float dt);
static int held200=-1;static float holdDistance200=1.35f;
static void defaults200(void);static void physics_tick200(float dt);static int physics_controls200(void);static int physics_prompt200(void);static int body_spawn200(int kind,float x,float y,float z);static void discard200(int id);static int extra_use200(int perform);static int npc_target200(void);static const char*dialog_name200(int id);static void dialog_profile200(char*out,int size,int id);static int dialog_local200(int topic);static int body_obstacle200(float x,float z);static void kettle_use200(int id);
static int unit150(int h,int f,int s);
static void safe_spawn160(void);
static int props_solid150(float x,float z);static int prop_use150(int perform);
static int navStatic150=0;static int physicsActor150=-1;static float sleepBlend150[NN];
static void defaults150(void);static int room_solid150(float x,float z);static int furniture_solid150(float x,float z);static int body_clear150(float x,float z,float y,float radius,int people);static int use150(int perform);static int schedule150(int i);static int sleep_tick150(int i,float dt,int home);static int bench_release150(int i,float dt);static int npc_age150(int i);static const char*npc_job150(int i);static int object150(int h,int f,int s,int type,float*x,float*y,float*z,float*rx,float*rz);static int balcony_open150(int h,int f,int side);
static float swimAir141=100,swimBlend141=0,sitting141[NN];static int npcBench141[NN],ignoreBench141=-1;
static int npc_elder141(int n);static int bench_route141(int i,float dt);static void bench_position141(int n,float*x,float*z);
static int outside141(float x,float z,float margin);static float divider141(int h,int f);static int furniture141(float x,float z);static int roof_solid141(float x,float z);static float terrain_height141(float x,float z);static void water_tick141(float dt);
static float path_target160(int h);

static int street_prompt205(void);static int street_use205(int perform);static void street_tick205(float dt);static void street_build205(void);static void street_draw205(void);
static unsigned int trafficSeed140=~0u;static float carX140[7],carZ140[7];
static float shopX=52,shopZ=23;
#define ATM_X (shopX-7.f)
#define ATM_Z (shopZ-2.f)
static int home_route140(int i,float dt,int home);static void decor_build140(void);static void traffic_tick140(float dt);static int traffic_ped_clear201(float x,float z,float y,float radius);static void traffic_init140(void);static int traffic_solid140(float x,float z);static void update140_defaults(void);static void update140_tick(float dt);static int update140_use(int perform);static int roof_at(float x,float z,float*height);static void player_die(void);static int market_price(int base);static void bank_withdraw(int amount);static void bank_collect(void);static float river_z(float x);static int river_water(float x,float z);static int bridge_at202(float x,float z);static int garage_roof203(float x,float z,float*height);

static void expansion_defaults(void);static void economy_tick(void);
static void inv_reset_ui(void);static void inv_sync(void);static float inv_weight(void);static int inv_add(int id,int amount);
static int bonusActive=0,bonusId=-1;static float bonusClock=0;
static void event_log(const char*kind,int id);
static void bonus_start(int id);static void bonus_tick(float dt);
static int workPending=-1,workHits=0;static float workCenter=.5f,workNoticeUntil=0;
static const char*work_hint(int id);
static int work_interact(int id);static int work_access(int id);static int work_open(int id);static void work_set(int id,int value);static void work_point(int id,float*x,float*y,float*z);static int work_floor(int id);static void work_cancel(void);static void work_check_tick(float dt);static void work_assign(int j,int h,int f);static void work_migrate(void);
static void radio_action(int action);static void radio_world_tick(void);
static int panel_wing(float x,float z);static int panel_solid(float x,float z);static void panel_motion(float dt);static void panel_reset(void);static int panel_interact(void);static int panel_step(float x,float z);static int panel_prompt(void);static void panel_save(void);static void panel_load(void);
#define WIN(i) (*((i)<24?&g.windows[(i)]:&g.extraWindows[(i)-24]))
#define PERSON(i) (*((i)<16?&g.people[(i)]:&g.extraPeople[(i)-16]))
static void life_init(int fresh);
static void loot_position(int n,float*x,float*z);static void env_build(void);static void extended_defaults(int old);
static int navReady=0,navOffice=0,navService=0,shopType=0,insideHouse=-1,hudMode=0,showRadar=0,showGoalMarker=1;
#define RECYCLE_X (-72.f)
#define RECYCLE_Z (-14.f)
static int recycle_near(void){return g.playerY<.3f&&dst(g.x,g.z,RECYCLE_X,RECYCLE_Z)<2.8f;}
static float skillLevel=10,moveVelocityX=0,moveVelocityZ=0,aimMultiplier=.55f,bobAmplitude=.035f;
static int aimHeld=0;
static float jump180=0,jumpV180=0,crouch180=0;
static float player_height180(void){return 1.80f-.68f*crouch180;}
static float player_eye180(void){return (seated>=0?1.15f:1.64f-.64f*crouch180)+jump180;}
static int high_at180(float x,float z);static int high_solid180(float x,float z);static float high_ground180(int n,float x,float z,float old);static int high_use180(int perform);static void high_tick180(float dt);
static int flat_lit180(int h,int f,int side);static U32 flat_color180(int h,int f,int side);
static int civic_solid190(float x,float z);static int civic_use190(int perform);static void local_tick190(float dt);static void local_init190(void);static int crowd_solid190(float x,float z,float y);
static const char*house_label190(int h);static void work_assign190(int j,int n,int f,int side);
static int home_annex(float x,float z);

static void reset_activities(void);
static int home_annex(float x,float z){return x>hx[0]-14.1f&&x<hx[0]-4.7f&&ab(z-hz[0])<4.2f;}
static int home_annex_furniture_solid(float x,float z){float ax=hx[0]-9.6f,az=hz[0];
 if(ab(x-(ax-2.7f))<.53f&&ab(z-(az+.4f))<1.34f)return 1;
 if(ab(x-ax)<.68f&&ab(z-(az+.4f))<.42f)return 1;
 return 0;
}
static void update_interior(void){if(g.playerY<.2f&&home_annex(g.x,g.z)){insideHouse=0;return;}if(g.playerY>.2f){insideHouse=-1;return;}if(insideHouse>=0){int h=insideHouse;if(ab(g.x-hx[h])<4.9f&&ab(g.z-hz[h])<4.38f)return;insideHouse=-1;}for(int h=0;h<NH;h++)if(ab(g.x-hx[h])<4.65f&&ab(g.z-hz[h])<4.12f){insideHouse=h;return;}}
static float skill_effect(float v){return v<=100?v/100.f:1+(v-100)/(v+100);}
static float itemAnim=0;static int itemKind=0,consumeCommitted=0;
static void consume_tick(float dt);static int consume_begin(int kind);
static int running=1,screen=1,subscreen=0,help=0,keys[256],pressed[256],oldkeys[256],mouseLook=1;
static float realTime=0,noticeTime=0,autosave=0,bob=0,shake=0,sensitivity=.0028f,openProgress=0,openStress=0,openTimer=0;
static int targetWindow=-1,opening=-1,practiceMode=0,interactKind=0,interactIndex=-1,headbob=0,easy=1,showFPS=0,invertY=0;
static float fps=60,stepTimer=0,visualAlpha=1;
static int openingBonus=0,dehydrated=0;
static int optionsTab=0,optionsRow=0,optionReturn=1;
static char seedInput140[11]="";static int seedLen140=0;
static unsigned int nextWorldSeed=57291;
static void seed_file140(void){if(seedLen140)return;char b[40];FILE*f=fopen("world_seed.txt","rb");if(!f)return;int n=fread(b,1,39,f);fclose(f);unsigned int v=0;int digits=0;for(int k=0;k<n;k++){if(b[k]<'0'||b[k]>'9')break;v=v*10+(b[k]-'0');digits++;}if(digits)nextWorldSeed=v;}

static int checkActive=0,checkResult=0,openHits=0,openFails=0;
static float checkClock=0,checkCenter=.65f,checkWidth=.32f,checkFeedback=0;
static unsigned int hash32(unsigned int x){x^=x>>16;x*=0x7feb352dU;x^=x>>15;x*=0x846ca68bU;return x^(x>>16);}
static float world_value(unsigned int i){return (hash32(g.worldSeed+i*73471u)>>8)/16777216.f;}
static void generate_world(unsigned int value){g.worldSeed=value;navReady=0;trafficSeed140=~value;
 static const float xx[NH]={-30,-10,10,30,-30,-10,10,30,-77,-57,-77,-57,38,58,38,58,-77,-57,-77,-57,38,58,38,58};
 static const float zz[NH]={-24,-24,-24,-24,29,29,29,29,-64,-64,-88,-88,-64,-64,-88,-88,61,61,121,121,61,61,121,121};
 for(int h=0;h<NH;h++){
  /* Controlled layout variation: no independent free placement. The 0.60m
     envelope preserves entrances, courtyards, drive lanes and minimum gaps. */
  hx[h]=xx[h]+(world_value(1000+h*2)-.5f)*1.20f;
  hz[h]=zz[h]+(world_value(1001+h*2)-.5f)*1.20f;
 }shopX=52+(world_value(910)-.5f)*5;shopZ=23+(world_value(911)-.5f)*4;env_build();}
static float divider141(int h,int f){return h==0&&f==0?.35f:(float[]){-.65f,.35f,1.15f}[hash32(g.worldSeed+h*119+f*641)%3];}
static float nutrition(void){return clamp(g.hunger/100,0,1);}
static float hydration(void){return clamp(g.thirst/100,0,1);}
static float work_efficiency(void){return (.70f+.18f*nutrition()+.12f*hydration())*(1+.12f*skill_effect(skillLevel));}
static int sprintLocked202=0;static float staminaDelay202=0;
static int can_sprint(void){return !sprintLocked202&&g.energy>8&&g.stamina>.05f&&!dehydrated&&g.thirst>=15;}
static void stamina_tick202(float dt,int running,int moving){if(g.stamina<=.05f)sprintLocked202=1;if(g.stamina>=20)sprintLocked202=0;
 if(running&&moving){g.stamina=mx(0,g.stamina-dt*(6.5f-hydration()));staminaDelay202=1.5f;if(g.stamina<=.05f)sprintLocked202=1;}
 else {staminaDelay202=mx(0,staminaDelay202-dt);if(staminaDelay202<=0&&swimBlend141<.1f)g.stamina=mn(100,g.stamina+dt*(1.5f+1.5f*nutrition())*(.6f+.4f*hydration())*(moving?.7f:1));}
}

static char notice[240]="",prompt[240]="";
static const char*savepath="okno_020.save";
static U32 accent=0xb8a77d,white=0xc6cbc4,muted=0x929f9e;
static float UI=1;static int uiPaperMode=0;
static U32 ui_ink(U32 c){int l=(((c>>16)&255)+((c>>8)&255)+(c&255))/3;if(c==accent)return accent;return l>150?0xc6cbc4:0x929f9e;}
static U32 ui_stock(U32 c){if(c==0x465966||c==0x334550||c==0x45545f)return 0x414c53;return 0x242c30;}

static void ur(int x,int y,int w,int h,U32 c){rect((int)(x*UI),(int)(y*UI),(int)(w*UI),(int)(h*UI),c);}
static void up(int x,int y,int w,int h,U32 c,float a){panel((int)(x*UI),(int)(y*UI),(int)(w*UI),(int)(h*UI),uiPaperMode?ui_stock(c):c,a);}
static void ut(int x,int y,const char*s,float sc,U32 c){text((int)(x*UI),(int)(y*UI),s,sc*UI,uiPaperMode?ui_ink(c):c);}
static void uc(int y,const char*s,float sc,U32 c){center((int)(y*UI),s,sc*UI,uiPaperMode?ui_ink(c):c);}
static void say(const char*s){SN(notice,sizeof(notice),"%s",s);noticeTime=2;}
static void save_game(void){panel_save();
 g.magic=0x4f4b4e4f;g.version=17;g.workRevision=1;g.uiRevision=10;g.options[9]=hudMode;g.options[10]=0;g.options[11]=showGoalMarker;g.config[1]=skillLevel;g.config[2]=aimMultiplier;g.config[3]=bobAmplitude;g.settings[0]=quality;g.settings[1]=headbob;g.settings[2]=easy;g.settings[3]=showFPS;g.settings[4]=invertY;g.settings[5]=soundEnabled;g.settings[6]=3;
 g.tuning[0]=sensitivity;g.tuning[1]=viewdist;g.tuning[2]=field;g.rot=yaw;g.look=pitch;
 g.options[0]=antialias;g.options[1]=detail;g.options[2]=frameLimit;g.options[3]=skillAssist;g.options[4]=autoChecks;g.options[5]=showHints;g.options[6]=masterVolume;g.options[7]=ambientVolume;g.options[8]=effectsVolume;g.config[0]=hudScale;
 char temp[1280];int length=SN(temp,sizeof(temp),"%s.tmp",savepath);if(length<=0||length>=(int)sizeof(temp)){say("Сохранение не записано: слишком длинный путь.");return;}FILE*f=fopen(temp,"wb");if(!f){say("Не удалось записать сохранение. Проверь доступ к папке.");return;}float sx204=g.x,sz204=g.z,sy204=g.playerY,sr204=g.rot,sl204=g.look;if(room204>=0){g.x=roomReturnX204;g.z=roomReturnZ204;g.playerY=roomReturnY204;g.rot=roomReturnYaw204;g.look=roomReturnPitch204;}int complete=fwrite(&g,sizeof(g),1,f)==1;g.x=sx204;g.z=sz204;g.playerY=sy204;g.rot=sr204;g.look=sl204;int closed=fclose(f)==0;if(!complete||!closed){say("Сохранение не записано полностью. Предыдущий файл сохранён.");return;}
#ifdef HEADLESS
 int replaced=rename(temp,savepath)==0;
#else
 int replaced=MoveFileExA&&MoveFileExA(temp,savepath,9);
#endif
 if(!replaced)say("Не удалось заменить сохранение. Предыдущий файл сохранён.");
}
static int load_game(void){
 State t;memset(&t,0,sizeof(t));FILE*f=fopen(savepath,"rb");if(!f)return 0;size_t n=fread(&t,1,sizeof(t),f);fclose(f);
 int legacy=t.version==4&&n==1316,previous=t.version==5&&n==1388,third=t.version==6&&n==3892,fourth=t.version==7&&n==3892;int fifth=t.version==8&&n==(size_t)((char*)&t.bottles-(char*)&t);int sixth=t.version==9&&n==(size_t)((char*)&t.homeFeedTaken-(char*)&t);int seventh=t.version==10&&n==(size_t)((char*)&t.uiRevision-(char*)&t);int eighth=t.version==11&&n==(size_t)((char*)&t.district-(char*)&t);int ninth=t.version==12&&n==(((size_t)((char*)&t.featureRevision-(char*)&t)+7)&~(size_t)7);if((!legacy&&!previous&&!third&&!fourth&&!fifth&&!sixth&&!seventh&&!eighth&&!ninth&&!(t.version==13&&n==(((size_t)((char*)&t.workRevision-(char*)&t)+7)&~(size_t)7))&&!(t.version==14&&n==(((size_t)((char*)&t.update140-(char*)&t)+7)&~(size_t)7))&&!(t.version==15&&n==(((size_t)((char*)&t.revision150-(char*)&t)+7)&~(size_t)7))&&(!((t.version==17&&n==sizeof(t))||(t.version==16&&(n==(((size_t)((char*)&t.revision200-(char*)&t)+7)&~(size_t)7)||n==(((size_t)((char*)&t.revision190-(char*)&t)+7)&~(size_t)7))))))||t.magic!=0x4f4b4e4f||t.money<0||t.money>10000000||t.activeJob<0||t.activeJob>2||t.apprentices<0||t.apprentices>3||!(t.x>=-192&&t.x<=192&&t.z>=-144&&t.z<=144))return 0;
 float needs[]={t.hunger,t.thirst,t.energy,t.health,t.bladder,t.bowel,t.illness,t.alcohol,t.stress,t.petHunger};for(int k=0;k<10;k++)if(!(needs[k]>=0&&needs[k]<=100))return 0;
 if(t.day<1||t.day>500000)return 0;
 if(!(t.hours>=0&&t.hours<24)||!(t.absolute>=0&&t.absolute<=10000000)||!(t.rot>=-100000&&t.rot<=100000)||!(t.look>=-2&&t.look<=2)||t.salary<0||t.salary>10000000)return 0;
 int inventory[]={t.food,t.water,t.feed,t.medicine,t.cigarettes,t.beer,t.bottles,t.scrap};for(int k=0;k<8;k++)if(inventory[k]<0||inventory[k]>999)return 0;
 if(!legacy&&!previous){for(int i=0;i<NN;i++){Citizen*c=i<16?&t.people[i]:&t.extraPeople[i-16];if(!(c->x>=-192&&c->x<=192&&c->z>=-144&&c->z<=144)||c->home<0||c->home>=NH||!(t.lifeHunger[i]>=0&&t.lifeHunger[i]<=100)||!(t.lifeEnergy[i]>=0&&t.lifeEnergy[i]<=100)||!(t.lifeWallet[i]>=0&&t.lifeWallet[i]<=100000))return 0;}}
 for(int j=0;j<NJ;j++)if(t.jobs[j].house<1||t.jobs[j].house>=NH+4||t.jobs[j].type<0||t.jobs[j].type>7||t.jobs[j].count<1||t.jobs[j].count>3)return 0;
 if(legacy){char bp[1200];SN(bp,sizeof(bp),"%s.v01.bak",savepath);FILE*old=fopen(bp,"rb");if(old)fclose(old);else{old=fopen(bp,"wb");if(old){fwrite(&t,1316,1,old);fclose(old);}}}
 if(fourth){char bp[1200];SN(bp,sizeof(bp),"%s.v04.bak",savepath);FILE*old=fopen(bp,"rb");if(old)fclose(old);else{old=fopen(bp,"wb");if(old){fwrite(&t,3892,1,old);fclose(old);}}}
 if(third){char bp[1200];SN(bp,sizeof(bp),"%s.v03.bak",savepath);FILE*old=fopen(bp,"rb");if(old)fclose(old);else{old=fopen(bp,"wb");if(old){fwrite(&t,3892,1,old);fclose(old);}}}
 if(previous){char bp[1200];SN(bp,sizeof(bp),"%s.v02.bak",savepath);FILE*old=fopen(bp,"rb");if(old)fclose(old);else{old=fopen(bp,"wb");if(old){fwrite(&t,1388,1,old);fclose(old);}}}
 if(t.version>=13){if(t.featureRevision!=1||t.homeLevel<0||t.homeLevel>3||t.herb<0||t.herb>999||!(t.intox>=0&&t.intox<=100)||t.bizCapital<0||t.bizReceivable<0||t.bizCapital>10000000||t.bizReceivable>10000000)return 0;for(int i=0;i<4;i++)if(t.bizLevel[i]<0||t.bizLevel[i]>3)return 0;t.tvURL[255]=0;}
 if(t.version<13)memset(&t.featureRevision,0,sizeof(t)-(size_t)((char*)&t.featureRevision-(char*)&t));if(t.version<16)memset(&t.revision150,0,sizeof(t)-(size_t)((char*)&t.revision150-(char*)&t));g=t;room_reset204();reset202();held200=-1;jump180=jumpV180=crouch180=0;seated=-1;defaults150();swimAir141=100;swimBlend141=0;update140_defaults();workPending=-1;opening=-1;checkActive=0;expansion_defaults();work_migrate();navOffice=navService=0;inv_reset_ui();itemAnim=0;consumeCommitted=0;bonusActive=0;reset_activities();panel_reset();bobAmplitude=(!legacy&&!previous&&!third)?clamp(g.config[3],0,.07f):.035f;hudMode=(!legacy&&!previous&&!third)?(int)clamp(g.options[9],0,2):0;showRadar=(!legacy&&!previous&&!third)?!!g.options[10]:1;skillLevel=(!legacy&&!previous&&!third)?clamp(g.config[1],0,10000):10;aimMultiplier=(!legacy&&!previous&&!third)?clamp(g.config[2],.2f,1):.55f;insideHouse=-1;moveVelocityX=moveVelocityZ=0;generate_world(legacy?0:g.worldSeed);defaults200();if(g.worldRevision<6&&g.playerY<.3f&&ab(g.x-hx[0])<4.7f&&g.z>hz[0]+4.1f&&g.z<hz[0]+13.5f){g.x=hx[0]-9.6f;g.z=hz[0];}yaw=g.rot;pitch=g.look;g.weather=g.weather==1?1:0;life_init(legacy||previous);
 if(legacy){g.stamina=100;set_resolution(4);easy=1;headbob=0;antialias=1;skillAssist=1;}
 else{set_resolution(g.settings[0]);easy=!!g.settings[2];headbob=!!g.settings[1];antialias=!!g.options[0];detail=(int)clamp(g.options[1],0,2);frameLimit=(int)clamp(g.options[2],30,120);skillAssist=!!g.options[3];autoChecks=!!g.options[4];showHints=!!g.options[5];masterVolume=(int)clamp(g.options[6],0,100);ambientVolume=(int)clamp(g.options[7],0,100);effectsVolume=(int)clamp(g.options[8],0,100);hudScale=clamp(g.config[0],.75f,1.15f);}
 showFPS=!!g.settings[3];invertY=!!g.settings[4];soundEnabled=!!g.settings[5];sensitivity=clamp(g.tuning[0],.001f,.006f);viewdist=clamp(g.tuning[1],55,125);field=clamp(g.tuning[2],55,90);if(third&&field==65)field=80;extended_defaults(legacy||previous||third||fourth);for(int j=0;j<NJ;j++)g.jobs[j].mode=1;if(!sixth&&!seventh&&!eighth&&t.version<12){g.homeFeedTaken=1;}g.radioOn=!!g.radioOn;panel_load();for(int h=0;h<NH;h++)if(g.playerY<.3f&&ab(g.x-hx[h])<1.6f&&ab(g.z-hz[h]+4.42f)<.7f){g.x=hx[h]+7.55f;g.z=hz[h]-9.5f;break;}showRadar=0;showGoalMarker=g.version>=14?!!g.options[11]:1;navReady=0;inv_sync();if(g.health<=0)screen=24;else safe_spawn160();g.version=17;
#ifndef HEADLESS
 if(!gpuActive){set_resolution(0);antialias=0;}
#endif
 return 1;
}
static void windowpos(int id,float*x,float*z){int h=id/3,k=id%3;*x=hx[h]+window_a180(1,k);*z=hz[h]+4.48f;}
static void window_handle_pos(int id,float*x,float*y,float*z){windowpos(id,x,z);float angle=WIN(id).angle;*x=*x-WINDOW180_W*.5f+WINDOW180_W*.91f*(float)cos(angle);*z-=WINDOW180_W*.91f*(float)sin(angle)+.10f;*y=1.86f;}
static void create_job(int j){
 Job*q=&g.jobs[j];int house=1+(int)(rnd()*(g.rank<2?7:NH-1));for(int attempt=0;attempt<NH-1;attempt++){int occupied=(house==q->house);for(int a=0;a<NJ;a++)if(a!=j&&g.jobs[a].house==house)occupied=1;if(!occupied)break;house=1+house%(NH-1);}
 q->house=house;q->type=(int)(rnd()*8);q->mode=1;q->count=q->type==2?1:1+(int)(rnd()*3);q->done=0;q->mask=0;q->pay=100+q->count*65+q->type*12+g.rank*30;q->deadline=g.absolute+18+rnd()*14;q->apprentice=-1;if(j==2)work_assign190(j,(int)(rnd()*4)%4,1+(int)(rnd()*8),g.completed%2);else work_assign(j,house,j==0?0:1+(int)(rnd()*4));

}
static void new_game(void){room_reset204();reset202();jump180=jumpV180=crouch180=0;seated=-1;swimAir141=100;swimBlend141=0;seed_file140();showGoalMarker=1;workNoticeUntil=0;workPending=-1;workHits=0;itemAnim=0;itemKind=0;dehydrated=0;inv_reset_ui();consumeCommitted=0;panel_reset();bonusActive=0;reset_activities();memset(&g,0,sizeof(g));g.workRevision=1;generate_world(nextWorldSeed);seed=hash32(g.worldSeed^0x81a4u);nextWorldSeed=hash32(nextWorldSeed+1);g.stamina=100;insideHouse=-1;skillLevel=10;moveVelocityX=moveVelocityZ=0;g.x=hx[0]-2.4f;g.z=hz[0]-.6f;g.rot=0;g.look=0;g.hours=16.4f;g.absolute=16.4f;g.day=1;g.money=3500;g.reputation=10;g.rank=1;
 g.hunger=85;g.thirst=85;g.energy=90;g.health=100;g.food=g.water=g.feed=g.cigarettes=g.beer=g.medicine=0;g.petHunger=90;g.petBond=40;g.petx=-28;g.petz=-25;g.nextBill=30;g.weatherTimer=180;
 for(int i=0;i<NN;i++){Citizen*c=&PERSON(i);c->home=i%NH;c->x=hx[c->home]+rnd()*6-3;c->z=hz[c->home]-6-rnd()*3;c->tx=c->x;c->tz=c->z;c->mood=40+rnd()*50;c->wait=rnd()*5;}
 g.winter=1;update140_defaults();for(int j=0;j<NJ;j++)create_job(j);yaw=0;pitch=0;opening=-1;openProgress=0;autosave=0;practiceMode=0;checkActive=0;checkFeedback=0;openFails=0;
 update140_defaults();defaults150();autoChecks=0;skillAssist=0;extended_defaults(1);inv_sync();event_log("inventory_spawned_empty",0);g.resourceDay=g.day;life_init(1);expansion_defaults();navOffice=0;navService=0;shopType=0;itemAnim=0;
 defaults200();held200=-1;safe_spawn160();noticeTime=0;
}
#define LOOT_COUNT 30
static float panel_origin_x(int n);static float panel_origin_z(int n);
#include "environment.h"
static int look_at(float x,float y,float z,float radius);static void nav_refresh(float x,float z);
#include "district180.h"
/* Collision is shared by movement, line of sight and the simulator tests. */
static int room_solid(float x,float z){return room_solid150(x,z);}
static int building_solid(float x,float z){if(civic_solid190(x,z))return 1;
 float roof; if(roof_at(x,z,&roof)&&g.playerY>=roof-.15f)return roof_solid141(x,z);
 if(g.fallVelocity<0&&g.playerY>.5f)return 0;
 if(g.playerY>11.9f&&roof_at(g.x,g.z,&roof)&&ab(g.playerY-roof)<.22f)return roof_solid141(x,z);
 if(g.playerY<2.8f&&x>hx[0]-14.5f&&x<hx[0]-4.85f&&ab(z-hz[0])<4.7f&&(x<hx[0]-14.15f||ab(z-hz[0])>4.18f))return 1;
 if(g.playerY<1.45f&&home_annex_furniture_solid(x,z))return 1;
 if(city_solid(x,z))return 1;
 if(g.playerY<2.8f&&quarter_solid(x,z))return 1;
 if(g.playerY<15&&(room_solid(x,z)||furniture141(x,z)))return 1;
 if(x< -191||x>191||z< -143||z>143)return 1;
 if(panel_solid(x,z))return 1;
 if(g.playerY<1.4f&&ab(x-RECYCLE_X)<1.7f&&ab(z-(RECYCLE_Z+1.15f))<.4f)return 1;
 /* Shop side walls, rear counter is accessible from the south. */
 if(g.playerY<3.2f&&ab(x-shopX)<5.5f&&ab(z-shopZ)<6.1f){if(x<shopX-5||x>shopX+5||z>shopZ+5.5f||(z<shopZ-5.7f&&ab(x-shopX)>1.3f)||(ab(x-shopX)>4.1f&&z>shopZ-2.2f&&z<shopZ+4.7f))return 1;}
 float a=x-OX,b=z-OZ;if(ab(a)<8.2f&&ab(b)<5.8f){if(ab(a)>7.65f||b>5.25f||(b< -5.25f&&ab(a)>1.35f))return 1;}
 a=x-KX;b=z-KZ;if(ab(a)<2.2f&&ab(b)<1.25f)return 1;
 return 0;
}
static int solid(float x,float z){return body_obstacle200(x,z)||crowd_solid190(x,z,g.playerY)||building_solid(x,z)||(g.playerY<.3f&&env_solid(x,z))||props_solid150(x,z);}
static int visible(float ax,float az,float bx,float bz){float d=dst(ax,az,bx,bz);int steps=(int)(d*3)+1;for(int n=1;n<steps;n++){float t=(float)n/steps;if(building_solid(lerp(ax,bx,t),lerp(az,bz,t)))return 0;}return 1;}
static int witnessed(void){for(int i=0;i<NN;i++){Citizen*c=&PERSON(i);float d=dst(g.x,g.z,c->x,c->z);if(d<10&&visible(g.x,g.z,c->x,c->z)){float forward=(float)cos(c->facing)*(g.z-c->z)+(float)sin(c->facing)*(g.x-c->x);if(d<2.2f||forward>0)return 1;}}return 0;}
static void move_step(float dx,float dz){float nx=g.x+dx,nz=g.z+dz;if(body_clear150(nx,g.z,g.playerY+jump180,.27f/WORLD_XZ,1)&&panel_step(nx,g.z))g.x=nx;if(body_clear150(g.x,nz,g.playerY+jump180,.27f/WORLD_XZ,1)&&panel_step(g.x,nz))g.z=nz;}
static int citizen_blocks(float x,float z){if(g.playerY>.3f)return 0;for(int i=0;i<NN;i++)if(dst(x,z,PERSON(i).x,PERSON(i).z)<.52f)return 1;return 0;}
static void move(float dx,float dz){int steps=1+(int)(mx(ab(dx),ab(dz))/.035f);for(int k=0;k<steps;k++)move_step(dx/steps,dz/steps);}
#include "life.h"
#include "extensions.h"
/* Job.apprentice == -2 is an explicit player acceptance marker; layout unchanged. */
static int job_accepted(int j){return j>=0&&j<NJ&&g.jobs[j].apprentice==-2;}
static int accept_job(int j){if(j<0||j>=NJ)return 0;if(job_accepted(j))return 1;for(int k=0;k<NJ;k++)if(k!=j&&job_accepted(k))g.jobs[k].apprentice=-1;g.activeJob=j;g.jobs[j].apprentice=-2;work_cancel();work_set(g.workTarget[j],0);navOffice=0;navService=0;navHouse=g.jobs[j].house;event_log("job_accepted",j);save_game();say("Заявка принята. Метка ведёт к подъезду, этажу и нужной створке.");return 1;}
static void paid_job(int j,int apprentice){audio(1);Job*q=&g.jobs[j];int pay=q->pay*(100+g.bizLevel[0]*8)/100;if(apprentice>=0)pay=pay*65/100;else pay+=(int)(clamp(1-openStress,0,1)*35);g.salary+=pay;g.completed++;g.reputation+=(apprentice>=0?1:3);g.rank=1+g.completed/5;skillLevel+=1.5f;
 char b[220];SN(b,sizeof(b),"%s: начислено %d руб. Забери зарплату в офисе. %s",house_label190(q->house),pay,apprentice>=0?"Доля ученика уже выплачена.":"Открыто профессионально.");say(b);
 if(apprentice<0)for(int a=0;a<g.apprentices;a++)g.apprenticeXP[a]=mn(100,g.apprenticeXP[a]+7);
 eventValue=(float)pay;event_log("job_completed",j);if(apprentice<0){navOffice=1;navService=0;navHouse=-1;}create_job(j);
}
static void finish_window(int id){if(id<0||id>=3)return;audio(12);
 WindowState*w=&WIN(id);w->mode=1;w->handle=1;practiceMode=1;event_log("open_window",id);
 int matched=0;for(int j=0;j<NJ;j++){Job*q=&g.jobs[j];int k=id%3;if(q->house!=id/3||!job_accepted(j))continue;if(q->type==2&&k!=2)continue;if(q->mask&(1<<k))continue;
  if(q->mode!=practiceMode){say("Нужен другой режим. Q — сменить режим; C — закрыть окно.");continue;}
  q->mask|=1<<k;q->done++;matched=1;if(q->done>=q->count)paid_job(j,-1);else say("Створка открыта. Продолжай с соседними окнами этого дома.");break;
 }
 if(matched){g.salary+=openingBonus;}openingBonus=0;
 if(!matched)say("Окно открыто. На этом окне сейчас нет подходящего заказа.");
 if(matched)for(int a=0;a<g.apprentices;a++)if(!g.autoWork[a])g.apprenticeXP[a]=mn(100,g.apprenticeXP[a]+5);
 opening=-1;openProgress=0;openStress=0;checkActive=0;
}
static int window_access(int id);
static int bench_sit(int n);static void bench_stand(void);static int trash_start(int n);
static int toggle_window(int id){return work_interact(id);}

static void household_bills(void){int added=0;while(g.nextBill>0&&g.absolute>=g.nextBill){g.utilities+=g.homeLevel>=2?240:280;g.nextBill+=24;g.stolenMask=0;added++;}if(added){eventValue=(float)added;event_log("utilities_billed",added);if(g.lateDiscovery>0){g.crimeHeat+=g.lateDiscovery;g.lateDiscovery=0;}say("Пришёл счёт за коммуналку. Оплата — в заявках.");}}
static int steal_cash(int h){if(h<=0||h>=NH||g.playerY>.3f||ab(g.x-hx[h])>=4.6f||ab(g.z-hz[h])>=4.05f||dst(g.x,g.z,hx[h]-3,hz[h]-1.7f)>2.3f||!visible(g.x,g.z,hx[h]-3,hz[h]-1.7f)){say("Подойди к кошельку внутри квартиры.");return 0;}int bit=1<<h;if(g.stolenMask&bit){say("Кошелёк уже пуст.");return 0;}int seen=witnessed();g.stolenMask|=bit;int cash=45+(int)(rnd()*45);g.money+=cash;g.lateDiscovery+=7;if(seen){g.crimeHeat=mn(100,g.crimeHeat+48);g.reputation=(int)mx(0,g.reputation-7);}eventValue=(float)cash;event_log(seen?"theft_witnessed":"theft_unseen",h);say(seen?"Тебя заметили. Житель сообщил о краже.":"Деньги взяты. Пропажу могут обнаружить позже.");save_game();return 1;}
static void simulate(float dt){
 if(screen==24)return;
 if(room204>=0){room_tick204(dt);return;}
 update140_tick(dt);if(screen==24)return;local_tick190(dt);physics_tick200(dt);ride_tick202(dt);
 bonus_tick(dt);
 if(g.day!=g.resourceDay){g.resourceDay=g.day;memset(g.bonusMask,0,sizeof(g.bonusMask));}
 consume_tick(dt);
 float previous=g.absolute;g.absolute+=dt/60.f;g.hours=(float)fmod(g.absolute,24);g.day=1+(int)(g.absolute/24);g.played+=dt;panel_motion(dt);
 economy_tick();g.intox=mx(0,g.intox-dt*.12f);
 household_bills();
 float rate=easy?.65f:1;
 g.hunger-=dt*.030f*rate;g.thirst-=dt*.045f*rate;g.energy-=dt*.016f*rate;g.bladder+=dt*.045f;g.bowel+=dt*.022f;g.petHunger-=dt*.034f;
 g.alcohol=mx(0,g.alcohol-dt*.2f);g.stress=clamp(g.stress+dt*(g.utilities>600?.015f:-.025f),0,100);
 if(g.thirst<15)dehydrated=1;else if(g.thirst>=20)dehydrated=0;
 if((!easy&&(g.hunger<=0||g.thirst<=0))||g.energy<=0)g.health-=dt*.015f;
 else if(g.health>0&&g.hunger>40&&g.thirst>40&&g.illness<15)g.health+=dt*.025f;
 if(g.bladder>99||g.bowel>99){g.stress+=8;g.reputation=(int)mx(0,g.reputation-1);g.bladder=45;g.bowel=40;say("Не дотерпел. Дома есть туалет. Пора привести себя в порядок.");}
 int sheltered=insideHouse>=0||panel_wing(g.x,g.z)>=0||(ab(g.x-OX)<7.6f&&ab(g.z-OZ)<5.2f)||(g.x>43.5f&&g.x<53.5f&&g.z>-5.5f&&g.z<5.5f);
 g.illness=clamp(g.illness+dt*(g.winter&&!sheltered?(g.weather?.007f:.003f):-.0015f),0,100);
 if(g.illness>40){g.health-=dt*.025f;g.energy-=dt*.025f;}
 if(g.petHunger<15)g.petBond=mx(0,g.petBond-dt*.025f);
 g.hunger=clamp(g.hunger,0,100);g.thirst=clamp(g.thirst,0,100);g.energy=clamp(g.energy,0,100);g.health=clamp(g.health,0,100);g.bladder=clamp(g.bladder,0,100);g.bowel=clamp(g.bowel,0,100);g.petHunger=clamp(g.petHunger,0,100);
 if(g.health<=0){player_die();return;}
 g.weatherTimer-=dt;g.weather=g.weather==1?1:0;if(g.weatherTimer<=0){g.weather=rnd()>.55f;g.weatherTimer=240+rnd()*240;}
 for(int j=0;j<NJ;j++){if(g.absolute>g.jobs[j].deadline){int accepted=job_accepted(j);if(accepted){g.reputation=(int)mx(0,g.reputation-2);navHouse=-1;navOffice=0;}create_job(j);if(accepted)say("Принятый вызов просрочен. В заявках есть новый.");}}
 life_tick(dt);cat_tick(dt);street_tick205(dt);
 /* One local cat; no player-follow component. */
 g.petx=catX;g.petz=catZ;
 for(int a=0;a<g.apprentices;a++){if(g.autoWork[a]){g.apprenticeTimer[a]+=dt;if(g.apprenticeTimer[a]>75+(100-g.apprenticeXP[a])*1.5f){g.apprenticeTimer[a]=0;int j=a%NJ;if(job_accepted(j))j=(j+1)%NJ;if(g.apprenticeXP[a]>=25){paid_job(j,a);g.apprenticeXP[a]=mn(100,g.apprenticeXP[a]+3);}else{g.autoWork[a]=0;say("Ученик пока не справился. Покажи ему ещё несколько открываний.");}}}}
 if(g.crimeHeat>0)g.crimeHeat=mx(0,g.crimeHeat-dt*.012f);
 for(int k=0;k<NW;k++){if(k==opening)continue;float goal=WIN(k).handle?(WIN(k).mode?1.48f:.24f):0;WIN(k).angle+=clamp(goal-WIN(k).angle,-dt*2.47f,dt*2.47f);}
}
static int look_at(float x,float y,float z,float radius){float dx=(x-g.x)*WORLD_XZ,dy=y-(g.playerY+player_eye180()),dz=(z-g.z)*WORLD_XZ;float d=(float)sqrt(dx*dx+dy*dy+dz*dz);if(d>3.8f)return 0;float dot=dx*(float)sin(yaw)*(float)cos(pitch)+dy*(float)sin(pitch)+dz*(float)cos(yaw)*(float)cos(pitch);return dot>d*radius;}
static int visible3d180(float ax,float ay,float az,float bx,float by,float bz){float old=g.playerY;int ok=1;for(int k=1;k<25;k++){float t=k/25.f;g.playerY=lerp(ay,by,t);if(building_solid(lerp(ax,bx,t),lerp(az,bz,t))){ok=0;break;}}g.playerY=old;return ok;}
static int window_access(int id){if(id<0||id>=NW||g.playerY>.3f)return 0;float x,z,wx,wy,wz;windowpos(id,&x,&z);window_handle_pos(id,&wx,&wy,&wz);int h=id/3;return ab(g.x-hx[h])<4.6f&&g.z>hz[h]-4.05f&&g.z<hz[h]+4.05f&&(dst(g.x,g.z,wx,wz)<=2.6f||dst(g.x,g.z,x+.35f,z-.12f)<=2.5f)&&visible3d180(g.x,g.playerY+player_eye180(),g.z,wx,wy,wz-.1f);}
static void interaction(void){targetWindow=-1;interactKind=0;interactIndex=-1;prompt[0]=0;if(room_use204(0))return;if(seated>=0){SN(prompt,sizeof(prompt),rideIndex202>=0?"E — встать / Пробел — раскачать или остановить":"Сидишь / E или движение — встать");return;}if(physics_prompt200()||extra_use200(0)||npc_target200())return;if(civic_use190(0))return;if(update140_use(0))return;if(high_use180(0))return;if(panel_prompt())return;if(street_prompt205())return;
 for(int k=0;k<NW;k++){float x,z;windowpos(k,&x,&z);float wx,wy,wz;window_handle_pos(k,&wx,&wy,&wz);if(window_access(k)&&(look_at(wx,wy,wz,.94f)||look_at(x+.35f,1.72f,z-.12f,.96f))){targetWindow=k;break;}}
 if(targetWindow>=0){SN(prompt,sizeof(prompt),"%s",work_hint(targetWindow));return;}
 if(use150(0)||prop_use150(0))return;
 for(int k=0;k<NW;k++){float x,z;windowpos(k,&x,&z);if(g.z>z&&!home_annex(g.x,g.z)&&dst(g.x,g.z,x,z)<2.6f){SN(prompt,sizeof(prompt),"Ручка с внутренней стороны. Войди в дом через дверь.");break;}}
 if(0){interactKind=1;SN(prompt,sizeof(prompt),"Кровать / сон и пропуск времени");}
 if(g.playerY<.3f&&dst(g.x,g.z,HOME_TOILET_X,HOME_TOILET_Z)<1.9f){interactKind=2;SN(prompt,sizeof(prompt),"E — туалет  |  U — помочиться  |  K — по-большому");}

 for(int p=0;p<15;p++){float qx,qz;quarter_shop(p,&qx,&qz);if(g.playerY<.3f&&g.z<qz-1.2f&&dst(g.x,g.z,qx,qz-1.3f)<2.5f&&look_at(qx,1.3f,qz,.88f)){interactKind=4;shopType=0;SN(prompt,sizeof(prompt),"ЛКМ — продукты / квартальный ларёк");return;}}
 if(ab(g.x-shopX)<4.5f&&g.z>shopZ-5.8f&&g.z<shopZ+3&&dst(g.x,g.z,shopX,shopZ)<5.8f){interactKind=4;shopType=0;SN(prompt,sizeof(prompt),"E — магазин «У дома»");}
 if(dst(g.x,g.z,OX,OZ+1.8f)<2.4f){interactKind=7;SN(prompt,sizeof(prompt),"E — касса evroremont prodaction. К выплате: %d руб.",g.salary);}
 if(dst(g.x,g.z,KX,KZ)<3.5f&&!solid(g.x,g.z)){interactKind=8;SN(prompt,sizeof(prompt),"E — «Табак • Напитки»: сигареты и пиво");}
 if(recycle_near()&&look_at(RECYCLE_X,1.1f,RECYCLE_Z+1.2f,.8f)){interactKind=14;SN(prompt,sizeof(prompt),"Приём стекла и металла / открыть");return;}
 for(int h=1;h<NH;h++)if(g.playerY<.3f&&!(g.stolenMask&(1<<h))&&ab(g.x-hx[h])<4.6f&&ab(g.z-hz[h])<4.05f&&dst(g.x,g.z,hx[h]-3,hz[h]-1.7f)<2.3f&&look_at(hx[h]-3,.96f,hz[h]-1.7f,.90f)){interactKind=5;interactIndex=h;SN(prompt,sizeof(prompt),"Чужой кошелёк / осмотреть");return;}
 if(g.playerY<.3f&&home_annex(g.x,g.z)&&dst(g.x,g.z,HOME_RADIO_X,HOME_RADIO_Z)<2.25f&&look_at(HOME_RADIO_X,1.00f,HOME_RADIO_Z-.16f,.94f)){interactKind=12;interactIndex=0;if(look_at(HOME_RADIO_X+.16f,1.03f,HOME_RADIO_Z-.18f,.985f))interactIndex=1;else if(look_at(HOME_RADIO_X-.16f,1.03f,HOME_RADIO_Z-.18f,.985f))interactIndex=-1;SN(prompt,sizeof(prompt),interactIndex?"Радиоприёмник / канал":"Радиоприёмник / питание");return;}
 if(g.playerY<.3f&&g.homeLevel>=3&&dst(g.x,g.z,hx[0]-11.7f,hz[0]-3.5f)<2&&look_at(hx[0]-11.7f,.6f,hz[0]-3.5f,.92f)){interactKind=18;SN(prompt,sizeof(prompt),"Кашпо / собрать траву");return;}
 if(!g.homeFeedTaken&&g.playerY<.3f&&dst(g.x,g.z,hx[0]+1.4f,hz[0]-2.1f)<2.5f&&look_at(hx[0]+1.4f,.28f,hz[0]-2.1f,.96f)){interactKind=13;SN(prompt,sizeof(prompt),"Взять корм для кота");return;}
 if(g.playerY<.3f&&dst(g.x,g.z,catX,catZ)<2.5f&&look_at(catX,.35f,catZ,.90f)){interactKind=11;SN(prompt,sizeof(prompt),"Кот / покормить или погладить");return;}
 for(int n=0;n<54;n++){float bx,bz;bench_position141(n,&bx,&bz);if(dst(g.x,g.z,bx,bz-.8f)<2.5f&&look_at(bx,.7f,bz,.92f)){interactKind=9;interactIndex=n;SN(prompt,sizeof(prompt),"E / ЛКМ — сесть на скамейку");return;}}
 for(int n=0;n<LOOT_COUNT;n++)if(!(g.lootMask&(1u<<n))){float x,z;loot_position(n,&x,&z);if(dst(g.x,g.z,x,z)<2.5f&&look_at(x+.55f,.6f,z,.94f)){interactKind=10;interactIndex=n;SN(prompt,sizeof(prompt),"Осмотреть контейнер");return;}}
 /* Residents, old and new, are selected with floor and visibility checks above. */
}
static int cat_in_reach(void){return g.playerY<.3f&&ab(g.x-hx[0])<4.65f&&ab(g.z-hz[0])<4.12f&&dst(g.x,g.z,catX,catZ)<=3&&visible(g.x,g.z,catX,catZ);}
static int feed_cat(void){if(!cat_in_reach()){say("Подойди к коту в домашней квартире.");return 0;}if(g.feed<=0){say("Корма нет. Пакет лежит дома или продаётся в магазине.");return 0;}if(g.petHunger>=99.5f){say("Кот сыт. Корм оставлен в рюкзаке.");return 0;}g.feed--;g.petHunger=mn(100,g.petHunger+45);g.petBond=mn(100,g.petBond+8);g.catBond=mn(100,g.catBond+8);catState=4;catTimer=4;catTX=catX;catTZ=catZ;eventValue=g.petHunger;event_log("cat_fed",0);audio(2);inv_sync();save_game();say("Хрюкот ест. Одна порция корма использована.");return 1;}
static int pet_cat(void){if(!cat_in_reach()){say("Кот далеко.");return 0;}catState=3;catTimer=2;catTX=hx[0]+1.4f;catTZ=hz[0]-1;g.catBond=mn(100,g.catBond+3);audio(2);event_log("cat_interaction",0);return 1;}
static void spend(int cost,const char*what){if(g.money<cost){say("Не хватает денег.");return;}g.money-=cost;say(what);}
static void shopbuy(int k){int price[]={190,65,110,580,90,150};if(k<0||k>5)return;for(int n=0;n<6;n++)price[n]=market_price(price[n]);if(g.money<price[k]){say("Не хватает денег.");return;}if(!inv_add(k+1,k==4?5:1))return;g.money-=price[k];audio(0);say("Покупка в рюкзаке. Спасибо, заходи ещё.");}
static float consume_duration(void){return (itemKind==4||itemKind==6)?5.5f:itemKind==1?2.6f:itemKind==3?3.1f:3.4f;}
static int consume_begin(int kind){if(itemAnim>0||kind<1||kind>6)return 0;int available=kind==1?g.food:kind==2?g.water:kind==3?g.medicine:kind==4?g.cigarettes:kind==5?g.beer:g.herb;if(available<=0)return 0;itemKind=kind;consumeCommitted=0;itemAnim=consume_duration();if(screen==5||screen==4)screen=0;audio(0);return 1;}
static void consume_tick(float dt){if(itemAnim<=0)return;float total=consume_duration();itemAnim=mx(0,itemAnim-dt);if(!consumeCommitted&&itemAnim<total*.40f){consumeCommitted=1;
 if(itemKind==1&&g.food>0){g.food--;g.hunger=mn(100,g.hunger+38);g.bowel=mn(100,g.bowel+12);audio(7);}
 if(itemKind==3&&g.medicine>0){g.medicine--;g.illness=mx(0,g.illness-55);g.health=mn(100,g.health+15);audio(7);}
 if(itemKind==5&&g.beer>0){g.beer--;g.bottles++;g.alcohol=mn(100,g.alcohol+35);g.bladder=mn(100,g.bladder+20);g.stress=mx(0,g.stress-15);audio(7);}
 if(itemKind==2&&g.water>0){g.water--;g.thirst=mn(100,g.thirst+45);g.bladder=mn(100,g.bladder+15);audio(7);}
 if(itemKind==6&&g.herb>0){g.herb--;g.intox=mn(100,g.intox+45);g.energy=mx(0,g.energy-8);g.health=mx(1,g.health-2);audio(7);}
 if(itemKind==4&&g.cigarettes>0){g.cigarettes--;g.stress=mx(0,g.stress-20);g.health=mx(1,g.health-3);audio(7);}
 } }
static void use_items(void){
 if(pressed['1']){if(g.food){consume_begin(1);}else say("Еда закончилась. Загляни в магазин.");}
 if(pressed['2']){if(g.water){consume_begin(2);}else say("Воды больше нет.");}
 if(pressed['3']){if(g.medicine){consume_begin(3);}else say("Лекарств нет.");}
 if(pressed['4']){if(g.cigarettes){consume_begin(4);}}
 if(pressed['5']){if(g.beer){consume_begin(5);}}
 if(pressed['F'])feed_cat();
 if(pressed['U']||pressed['K']){int toilet=g.playerY<.3f&&dst(g.x,g.z,HOME_TOILET_X,HOME_TOILET_Z)<2.4f;if(pressed['U'])g.bladder=0;if(pressed['K'])g.bowel=0;if(!toilet){g.stress+=4;if(witnessed()){g.money=(int)mx(0,g.money-35);g.reputation=(int)mx(0,g.reputation-2);say("Жители возмущены. Штраф за нарушение порядка.");}else say("Нужда справлена. В следующий раз лучше дойти до туалета.");}else say("Туалет. Теперь легче.");}
}
#include "inventory.h"
#include "panelki.h"
#include "activities.h"
#include "expansion.h"
#include "prowork.h"
#include "update140.h"
#include "update141.h"
#include "update150.h"
#include "update150_props.h"
/* Classify the actual authored ground surfaces instead of guessing from one
   broad world-coordinate condition. This keeps footsteps aligned with roads,
   pavements, residential paths and concrete pads as the seeded layout varies. */
static int footstep_hard_surface160(float x,float z){
 float ax=ab(x),az=ab(z);
 /* Main asphalt streets and the two cross streets. */
 if(ax<132&&az<6)return 1;
 if(ax<127&&(ab(z-43)<4||ab(z+43)<4))return 1;
 if(ab(ax-94)<4&&az<87)return 1;
 /* Sidewalks flanking the main avenue. */
 if(ax<132&&ab(az-7.6f)<1.4f)return 1;
 /* Residential access lanes and their two vertical connectors. */
 if(ab(x+5)<82&&(ab(z+76)<2.25f||ab(z-72)<2.25f))return 1;
 if((ab(x+86)<2.25f&&z>-106&&z<-43)||(ab(x-78)<2.25f&&z>43&&z<72))return 1;
 /* The narrow paved paths leading to each small-house entrance. */
 for(int h=0;h<NH;h++){
  float tx=path_target160(h),lo=mn(hz[h]-8.6f,tx),hi=mx(hz[h]-8.6f,tx);
  if(ab(x-(hx[h]+7.55f))<.72f&&z>=lo-.12f&&z<=hi+.12f)return 1;
  if(ab(x-(hx[h]+7.5f))<.75f&&ab(z-(hz[h]-9.2f))<.85f)return 1;
 }
 /* Long-panel entry aprons and all courtyard bench pads are concrete/paving. */
 for(int p=0;p<PANEL_COUNT;p++){
  float px=panel_origin_x(p),pz=panel_origin_z(p)-9.4f;
  if(ab(x-px)<15&&ab(z-pz)<.8f)return 1;
 }
 for(int n=0;n<54;n++){
  float bx,bz;bench_position141(n,&bx,&bz);
  if(ab(x-bx)<2.4f&&ab(z-(bz-.35f))<1.15f)return 1;
 }
 /* Shop, kiosk, office and recycling hardstandings. */
 if(ab(x-shopX)<4.1f&&z>shopZ+1.0f&&z<shopZ+6.1f)return 1;
 for(int n=0;n<PANEL_COUNT;n++){
  float qx,qz;quarter_shop(n,&qx,&qz);
  if(ab(x-qx)<4.0f&&ab(z-(qz+3.5f))<2.5f)return 1;
  if(ab(x-qx)<2.2f&&ab(z-(qz-1.8f))<1.9f)return 1;
 }
 if(ab(x-RECYCLE_X)<4.5f&&ab(z-RECYCLE_Z)<2.7f)return 1;
 if(ab(x-OX)<7.5f&&ab(z-OZ)<5.5f)return 1;
 return 0;
}
static float lastSleepHours=0,sleepNoticeUntil=0;static char sleepSummary[240]="";
static int player_sleep(float hours){if(!(hours>0&&hours<=24)||g.playerY>.3f||dst(g.x,g.z,hx[0]-1.1f,hz[0]+3.0f)>=2){say("Спать можно у своей кровати.");return 0;}float beforeH=g.hunger,beforeT=g.thirst;life_skip_hours(hours);update140_tick(0);economy_tick();g.intox=mx(0,g.intox-hours*7.2f);g.energy=mn(100,g.energy+hours*(11+g.homeLevel));g.stamina=100;g.hunger=mx(0,g.hunger-hours*4);g.thirst=mx(0,g.thirst-hours*5);if(hours>=8){g.hunger=mn(g.hunger,35);g.thirst=mn(g.thirst,30);}g.bladder=mn(95,g.bladder+hours*7);g.bowel=mn(95,g.bowel+hours*3);g.petHunger=mx(0,g.petHunger-hours*1.9f);g.alcohol=mx(0,g.alcohol-hours*12);g.illness=mx(0,g.illness-hours);if(g.hunger<=0||g.thirst<=0)g.health=mx(1,g.health-hours*.8f);else g.health=mn(100,g.health+hours);dehydrated=g.thirst<15;household_bills();for(int j=0;j<NJ;j++)if(g.jobs[j].deadline<g.absolute){if(job_accepted(j)){g.reputation=(int)mx(0,g.reputation-2);navHouse=-1;navOffice=navService=0;}create_job(j);}g.resourceDay=g.day;bonusActive=0;lastSleepHours=hours;SN(sleepSummary,sizeof(sleepSummary),"Сон %.0f ч. Еда -%.0f, вода -%.0f. Проверь туалет и Хрюкота.",hours,beforeH-g.hunger,beforeT-g.thirst);eventValue=hours;event_log("player_slept",0);audio(15);save_game();say(sleepSummary);noticeTime=6;sleepNoticeUntil=realTime+6;return 1;}
static int recycle_sell(int id){if(!recycle_near()){say("Подойди к приёмщику на отдельной площадке западнее офиса.");return 0;}int bottles=(id==0||id==7)?g.bottles:0,scrap=(id==0||id==8)?g.scrap:0;if(id!=0&&id!=7&&id!=8)return 0;int amount=bottles*10+scrap*18;if(amount<=0){say("Нет стеклотары или металла для сдачи.");return 0;}g.bottles-=bottles;g.scrap-=scrap;g.money+=amount;inv_sync();eventValue=(float)amount;event_log("recycling_sold",id);audio(1);save_game();char b[180];SN(b,sizeof(b),"Сдано: стекло %d, металл %d. Получено %d руб.",bottles,scrap,amount);say(b);return amount;}
static float check_duration(void){return (easy?3.8f:3.2f)*(g.thirst<15?.95f:1);}
static float check_phase(void){return clamp(checkClock/check_duration(),0,1);}
static float check_distance(float phase){float d=ab(phase-checkCenter);return mn(d,1-d);}
static void begin_check(void){checkActive=1;checkClock=0;checkCenter=.48f+rnd()*.30f;checkWidth=(easy?.34f:.26f)+(skillAssist?.04f:0)+.04f*skill_effect(skillLevel)+mn(openFails*.05f,.12f);}
static void resolve_check(int success,int great){checkActive=0;checkFeedback=.65f;checkResult=success?(great?2:1):-1;audio(success?5:6);if(success){skillLevel+=great?1.0f:.35f;openHits++;openProgress=mn(.98f,openProgress+(great?.07f:.025f));if(great)openingBonus+=10;}else{openFails++;openStress+=.08f;openProgress=mx(.10f,openProgress-.035f);say("Следующая попытка проще. Здоровье и заказ не теряются.");if(skillAssist&&openFails>=3){openHits++;say("Подстраховка помогла пройти проверку. Продолжай открывание.");}}}
static void opening_tick(float dt){
 if(opening<0)return;float x,z;windowpos(opening,&x,&z);if(!window_access(opening)){opening=-1;checkActive=0;say("Подойди к окну и нажми E снова.");return;}
 openTimer+=dt;if(checkFeedback>0){checkFeedback=mx(0,checkFeedback-dt);return;}
 if(checkActive){checkClock+=dt;float ph=check_phase();if(((autoChecks&&check_distance(ph)<checkWidth*.4f)||pressed[32])){int hit=check_distance(ph)<=checkWidth*.5f;resolve_check(hit,hit&&check_distance(ph)<(.045f+.015f*skill_effect(skillLevel)));}
  else if(checkClock>check_duration())resolve_check(0,0);
 }else{openProgress+=dt*.205f*work_efficiency();if((openHits==0&&openProgress>=.24f)||(openHits==1&&openProgress>=.62f))begin_check();else if(openProgress>=1&&openHits>=2)finish_window(opening);}
 if(opening>=0){float a=clamp((openProgress-.26f)/.74f,0,1);a=a*a*(3-2*a);WIN(opening).angle=practiceMode?a*1.48f:a*.24f;}
}
static int option_count(void){return optionsTab==0?8:optionsTab==1?12:optionsTab==2?4:9;}
static void change_option(int dir){
 if(optionsTab==0)switch(optionsRow){case 0:set_resolution((quality+dir+5)%5);break;case 1:if(dir>0){set_resolution(2);viewdist=80;detail=1;antialias=1;}else{set_resolution(1);viewdist=65;detail=0;antialias=0;}break;case 2:antialias=!antialias;break;case 3:viewdist=clamp(viewdist+dir*10,55,125);break;case 4:detail=(detail+dir+3)%3;break;case 5:field=clamp(field+dir*5,55,90);break;case 6:hudScale=clamp(hudScale+dir*.05f,.75f,1.15f);break;case 7:frameLimit=(int)clamp(frameLimit+dir*30,30,120);break;}
 else if(optionsTab==1)switch(optionsRow){case 0:easy=!easy;break;case 1:sensitivity=clamp(sensitivity+dir*.0003f,.001f,.006f);break;case 2:headbob=!headbob;break;case 3:invertY=!invertY;break;case 4:skillAssist=!skillAssist;break;case 5:autoChecks=!autoChecks;break;case 6:showHints=!showHints;break;case 7:hudMode=(hudMode+dir+3)%3;break;case 8:showRadar=0;showGoalMarker=!showGoalMarker;break;case 9:aimMultiplier=clamp(aimMultiplier+dir*.05f,.2f,1);break;case 10:bobAmplitude=clamp(bobAmplitude+dir*.005f,0,.07f);break;case 11:npcDebugEnabled=!npcDebugEnabled;break;}
 else if(optionsTab==3){switch(optionsRow){case 0:focus_profile(!g.focusMode);break;case 1:g.hudOpacity=clamp(g.hudOpacity+dir*.1f,0,1);break;case 2:g.uiMotion=(g.uiMotion+dir+3)%3;break;case 3:g.popupMode=!g.popupMode;break;case 4:g.highContrast=!g.highContrast;break;case 5:g.fastMode=!g.fastMode;break;case 6:g.mapSize=clamp(g.mapSize+dir*.05f,.75f,1.2f);break;case 7:g.mapOpacity=clamp(g.mapOpacity+dir*.1f,.1f,1);break;case 8:g.season=(g.season+dir+4)%4;g.seasonDay=g.day;g.winter=g.season==0;if(!g.winter)g.trackHead190=0;winterEnabled=g.winter;navReady=0;season_prepare();break;}}
 else switch(optionsRow){case 0:masterVolume=(int)clamp(masterVolume+dir*10,0,100);break;case 1:ambientVolume=(int)clamp(ambientVolume+dir*10,0,100);break;case 2:effectsVolume=(int)clamp(effectsVolume+dir*10,0,100);audio(5);break;case 3:soundEnabled=!soundEnabled;break;}
}
static void actor_tick180(float dt){
 float want=keys[17]?1:0;crouch180+=clamp(want-crouch180,-dt*7,dt*7);
 if(pressed[32]&&!bonusActive&&!microActive&&opening<0&&jump180<=0&&g.fallVelocity==0&&seated<0&&swimBlend141<.1f&&g.stamina>7){jumpV180=4.0f;g.stamina-=7;}
 if(jumpV180||jump180>0){jumpV180-=9.81f*dt;jump180+=jumpV180*dt;
  int indoors=home_annex(g.x,g.z)||insideHouse>=0||panel_wing(g.x,g.z)>=0;float limit=2.80f-player_height180()-.04f;
  if(indoors&&jump180>limit){jump180=limit;jumpV180=mn(0,jumpV180);}
  if(jump180<=0){jump180=jumpV180=0;}
 }
}

static void controls(float dt){
 if(screen==24){if(pressed[13]){g.health=100;g.hunger=g.thirst=70;g.energy=80;g.x=hx[0]-2.4f;g.z=hz[0]-.6f;g.playerY=0;g.fallVelocity=g.drowning=0;screen=0;save_game();}return;}
 if(screen==25){for(int k=0;k<10;k++)if(pressed['0'+k]&&seedLen140<10){seedInput140[seedLen140++]='0'+k;seedInput140[seedLen140]=0;}if(pressed[8]&&seedLen140){seedInput140[--seedLen140]=0;}if(pressed[27]){screen=1;return;}if(pressed[13]){if(seedLen140){unsigned long long seed=0;for(int k=0;k<seedLen140;k++)seed=seed*10+seedInput140[k]-'0';if(seed>4294967295ull){say("Сид должен помещаться в 32 бита.");return;}nextWorldSeed=(unsigned int)seed;}new_game();screen=0;}return;}
 if(help){if(pressed[112]||pressed[27])help=0;return;}
 if(pressed[112]){help=1;return;}
 if(pressed[27]){work_cancel();if(screen==0){screen=2;}else if(screen==1){running=0;}else if(screen==6){screen=optionReturn;}else{screen=0;subscreen=0;}return;}
 if(screen==1){if(pressed[13]){if(!load_game()){new_game();}screen=g.health<=0?24:0;}if(pressed['N']){screen=25;}if(pressed['R']){new_game();screen=1;}if(pressed['O']){optionReturn=1;screen=6;}return;}
 if(screen==2){if(pressed[13])screen=0;if(pressed['O']){optionReturn=2;screen=6;}if(pressed['S']){save_game();say("Игра сохранена.");}if(pressed['M']){save_game();screen=1;}if(pressed['X']){save_game();running=0;}return;}
 if(screen==6){if(pressed[9]){optionsTab=(optionsTab+1)%4;optionsRow=0;}if(pressed[38])optionsRow=(optionsRow+option_count()-1)%option_count();if(pressed[40])optionsRow=(optionsRow+1)%option_count();if(pressed[37])change_option(-1);if(pressed[39]||pressed[13])change_option(1);if(pressed[112])help=1;return;}
 if(pressed['V']){screen=screen==8?0:8;return;}
 if(screen==8)return;
 if(screen==10){for(int k=0;k<3;k++)if(pressed['1'+k]){dialog_choose(k==2&&dialogOffer>=0?3:k);}if(pressed['4'])screen=0;return;}
 if(screen==22||screen==11||screen==14||screen==15||screen==16||screen==17||screen==18||screen==19||screen==20||screen==21)return;
 if(screen==12){screen=0;return;}

 if(screen==9){if(pressed[13])collect_salary();return;}
 if(screen==23){if(pressed[13]){bank_withdraw(0);}return;}
 if(screen==24){if(pressed[13]){g.health=100;g.hunger=70;g.thirst=70;g.energy=80;g.x=hx[0]-2.4f;g.z=hz[0]-.6f;g.playerY=0;g.fallVelocity=0;g.drowning=0;insideHouse=0;screen=0;save_game();}return;}
 if(pressed[9]){screen=screen==3?0:3;return;}
 if(pressed['I']){screen=screen==5?0:5;return;}
 if(pressed['H']){screen=screen==7?0:7;return;}
 if(pressed[112]){help=!help;return;}
 if(screen==3){if(pressed['O']){navOffice=1;screen=0;return;}for(int j=0;j<3;j++)if(pressed['1'+j]){accept_job(j);screen=0;}if(pressed['B']){if(g.utilities==0)say("Все счета уже оплачены.");else if(g.money>=g.utilities){g.money-=g.utilities;g.utilities=0;g.daysPaid++;say("Коммуналка оплачена. Дома спокойно.");}else say("Не хватает денег на коммуналку.");}return;}
 if(screen==4){for(int k=0;k<(shopType?2:6);k++)if(pressed['1'+k])shopbuy(shopType?k+4:k);return;}
 if(screen==5){use_items();return;}
 if(screen==7){if(pressed['R']){int cost=12000+g.apprentices*6000;if(g.apprentices>=3)say("В бригаде уже три ученика.");else if(g.money<cost)say("Не хватает денег на обучение.");else{g.money-=cost;g.apprenticeXP[g.apprentices]=0;g.apprentices++;say("Ученик принят в бригаду. Ожидает в офисе, за игроком не ходит.");}}
 for(int a=0;a<g.apprentices;a++)if(pressed['1'+a]){if(g.apprenticeXP[a]<25)say("Сначала покажи ученику несколько открываний. Нужно 25 опыта.");else{g.autoWork[a]=!g.autoWork[a];g.apprenticeTimer[a]=0;say(g.autoWork[a]?"Ученик отправлен на самостоятельные вызовы.":"Ученик ожидает в офисе.");}}return;}
 if(screen!=0)return;
 if(room_controls204(dt))return;if(pressed['E']&&room_use204(1))return;
 if(ride_controls202(dt))return;
 if(physics_controls200())return;
 if(pressed['E']&&interactKind==6){dialogNPC=interactIndex;dialogTopic=0;dialog_begin(dialogNPC);if(dialogNPC<NN)chatUntil[dialogNPC]=lifeClock+4;screen=10;return;}
 if(pressed['E']&&extra_use200(1))return;
 if(pressed['M']||pressed[4]){screen=11;return;}
 
 if(pressed['E']&&(civic_use190(1)||update140_use(1)))return;
 if(seated>=0){if(pressed['E']||pressed[1]||keys['W']||keys['A']||keys['S']||keys['D']||pressed[32]){bench_stand();say("Ты встал.");}else stamina_tick202(dt,0,0);return;}
 if(targetWindow<0&&(pressed['E']||pressed[1])&&(use150(1)||prop_use150(1)))return;
 if(pressed[1]&&!keys[18]&&mouseLook){if(microActive||bonusActive)pressed[32]=1;if(!microActive)pressed['E']=1;}
 actor_tick180(dt);
 if(bonusActive&&pressed[32]){int working=workPending>=0;bonus_tick(0);if(working)pressed['E']=0;}
 if(microActive){micro_play(dt);return;}if(!bonusActive)micro_update(dt);if(microActive)return;
 if(pressed[113]){hudMode=(hudMode+1)%3;}
 if(pressed[114]){screen=11;return;}
 if(pressed['J']){navHouse=-1;navOffice=!navOffice;say(navOffice?"Маршрут: офис evroremont prodaction.":"Маршрут: активный заказ.");}
 if(pressed['Q']){practiceMode=!practiceMode;opening=-1;checkActive=0;openProgress=0;}
 if(pressed['C']&&targetWindow>=0&&WIN(targetWindow).handle)toggle_window(targetWindow);
 stamina_tick202(0,0,0);int sprint=keys[16]&&crouch180<.1f&&can_sprint()&&opening<0;float speed=(sprint?(g.fastMode?7.8f:6.5f):(g.fastMode?4.f:3.8f))*(.78f+.12f*nutrition()+.10f*hydration())*(g.illness>55?.85f:1);if(!g.winter&&river_water(g.x,g.z)&&g.playerY<.5f)speed*=.45f;if(opening>=0)speed*=.35f;if(inv_weight()>20)speed*=clamp(1-(inv_weight()-20)*.04f,.6f,1);
 speed*=1-.55f*crouch180;speed/=WORLD_XZ; /* Preserve physical walking speed after metric expansion. */
 float forward=(keys['W']?1.f:0)-(keys['S']?1.f:0),side=(keys['D']?1.f:0)-(keys['A']?1.f:0);float norm=(float)sqrt(forward*forward+side*side);if(norm>0){forward/=norm;side/=norm;move(((float)sin(yaw)*forward+(float)cos(yaw)*side)*speed*dt,((float)cos(yaw)*forward-(float)sin(yaw)*side)*speed*dt);bob+=dt*speed*2.4f;if(sprint){g.thirst=mx(0,g.thirst-dt*.012f);}stepTimer+=dt;if(stepTimer>(sprint?.33f:.48f)){audio(footstep_sound160(insideHouse>=0||panel_wing(g.x,g.z)>=0,winterEnabled,g.weather,footstep_hard_surface160(g.x,g.z)));stepTimer=0;}}
 stamina_tick202(dt,sprint,norm>0);
 if(keys[37])yaw-=dt*1.5f;if(keys[39])yaw+=dt*1.5f;if(keys[38])pitch+=dt;if(keys[40])pitch-=dt;
 pitch=clamp(pitch,-1.2f,1.2f);if(pressed['L']){mouseLook=!mouseLook;say(mouseLook?"Обзор мышью включён.":"Обзор мышью выключен. Используй стрелки.");}
 aimHeld=keys[2];
 use_items();
 if(pressed['G']&&interactKind==5)steal_cash(interactIndex);
 if(itemAnim>0){pressed['E']=0;targetWindow=-1;}
 
 if(pressed['E']&&(civic_use190(1)||update140_use(1)))return;
 if(pressed['E']&&high_use180(1))return;
 if(pressed['E']&&panel_interact())return;
 if(pressed['E']&&street_use205(1))return;
 if(pressed['E']&&targetWindow>=0)toggle_window(targetWindow);
 /* Ordered balcony stays shut until all three mandatory checks succeed. */
 if(pressed['E']&&targetWindow<0){switch(interactKind){
  case 1:screen=17;break;
  case 2:g.bladder=0;g.bowel=0;say("Нужды справлены. Руки вымыты.");break;
  case 3:g.petBond=mn(100,g.petBond+2);g.stress=mx(0,g.stress-5);say("Хрюкот: мяу-хрю. Кажется, вы понимаете друг друга.");break;
  case 4:audio(0);shopType=0;screen=4;break;
  case 5:screen=16;break;
  case 7:screen=9;break;
  case 8:shopType=1;screen=4;break;
  case 9:bench_sit(interactIndex);say("Скамейка: отдых. E или движение — встать.");break;
  case 10:trash_start(interactIndex);break;
  case 11:screen=15;break;
  case 12:radio_action(interactIndex);event_log("radio_interaction",interactIndex);break;
  case 14:screen=18;break;
  case 17:screen=20;break;
  case 18:herb_pick();break;
  case 13:if(!g.homeFeedTaken&&inv_add(3,1)){g.homeFeedTaken=1;event_log("cat_food_collected",0);}break;
  case 6:{dialogNPC=interactIndex;dialogTopic=0;dialog_begin(dialogNPC);if(dialogNPC<NN)chatUntil[dialogNPC]=lifeClock+4;screen=10;break;}
 }}
 autosave+=dt;if(autosave>45){autosave=0;save_game();}
}

#include "life190.h"
#include "update200.h"
#include "update202.h"

static int garage_roof203(float x,float z,float*height){if(ab(x)<160)return 0;if(!propCount150)props_init150();for(int n=0;n<propCount150;n++){Prop150*p=&props150[n];if((p->type==4||p->type==11)&&ab(x-p->x)<3.6f&&ab(z-p->z)<3.1f){*height=2.80f+.05f*clamp((z-p->z)/3.1f,-1,1)+(g.winter?.035f:0);return 1;}}return 0;}

#include "update204.h"
