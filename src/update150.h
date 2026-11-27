/* v150: two real apartments per landing, one shared collision contract and persistent household state. */
static int unit150(int h,int f,int s){return h*10+f*2+s;}
static void defaults150(void){if(g.revision150!=150){g.revision150=150;memset(g.flatDoors150,0,sizeof(g.flatDoors150));memset(g.balconyDoors150,0,sizeof(g.balconyDoors150));for(int i=0;i<240;i++){g.things150[i]=1|(1u<<16)|(1u<<17)|((i>1&&hash32(g.worldSeed+i*71)%100>=68)?8:0);g.foodDay150[i]=-1;}memset(g.residentHome,0,sizeof(g.residentHome));}memset(sleepBlend150,0,sizeof(sleepBlend150));}
/* Household is a shared state: facade, ceiling lamp and actual room illumination agree. */
static U32 flat_color180(int h,int f,int side){unsigned n=hash32(g.worldSeed+h*389+f*173+side*71);return n%7==0?0xab83ce:(U32[]){0xe3c590,0xcac7aa,0xbac8cd,0xd1b68e}[n%4];}
static int flat_lit180(int h,int f,int side){if(h<NH)return !(g.things150[unit150(h,f,side)]&8);unsigned n=hash32(g.worldSeed+h*389+f*173+side*71);int chance=g.hours>=16&&g.hours<23?68:g.hours>=6.5f&&g.hours<9?40:g.hours>=9&&g.hours<16?18:8;return n%100<chance;}
static int balcony_open150(int h,int f,int s){if(g.balconyDoors150[h]&(1u<<(f*2+s)))return 1;int k=s?2:0;return f?!!(g.residence[h].windows&(1ull<<((f-1)*6+3+k))):WIN(h*3+k).angle>.75f;}
static int room_solid150(float x,float z){int f=(int)clamp((float)floor((g.playerY+.15f)/3),0,4);for(int h=0;h<NH;h++){float a=x-hx[h],b=z-hz[h];if(ab(a)>HOUSE_HALF_W||b<HOUSE_FRONT_B||b>HOUSE_BACK_B)continue;
 if(ab(b-FLAT_ENTRY_B)<.11f){int s=a>0;float d=ab(a-(s?FLAT_DOOR_A:-FLAT_DOOR_A));if(d>FLAT_DOOR_HALF||!(g.flatDoors150[h]&(1u<<(f*2+s))))return 1;}
 if(ab(a)<WALL_HIT&&b>FLAT_ENTRY_B)return 1;
 if(ab(ab(a)-BATH_A)<WALL_HIT&&b>FLAT_ENTRY_B&&b<BATH_B&&(b<BATH_DOOR_B0||b>BATH_DOOR_B1))return 1;
 if(ab(b-BATH_B)<WALL_HIT&&ab(a)>BATH_A)return 1;
 if(b>FLAT_ENTRY_B){int side=a>0;unsigned flags=g.things150[unit150(h,f,side)];
 if(ab(ab(a)-BATH_A)<WALL_HIT&&ab(b-(BATH_DOOR_B0+BATH_DOOR_B1)*.5f)<(BATH_DOOR_B1-BATH_DOOR_B0)*.5f&&!(flags&(1u<<16)))return 1;
 if(ab(b-ROOM_WALL_B)<WALL_HIT&&(ab(a)<ROOM_DOOR_A0||ab(a)>ROOM_DOOR_A1||!(flags&(1u<<17))))return 1;
 }

 }return 0;}
/* One source of truth: object positions feed drawing, hit targets and collision. */
static int object150(int h,int f,int s,int type,float*x,float*y,float*z,float*rx,float*rz){
 static const float ax[]={1.55f,.80f,.42f,3.05f,4.35f,BED_A,3.8f,.65f,.65f,2.8f,3.85f,2.4f,2.4f,3.85f,4.1f,4.67f,3.25f,ROOM_DOOR_A,4.15f,1.25f,3.95f,DRESSER_A};
 static const float bz[]={-1.42f,-1.30f,1.65f,-1.76f,1.62f,BED_B,DESK_CHAIR_B,-.65f,.27f,4.35f,3.6f,-1.94f,-1.94f,3.6f,.12f,-.28f,-.7f,ROOM_WALL_B,-1.35f,4.02f,3.55f,DRESSER_B};
 static const float yy[]={1.05f,1.18f,1.0f,1.4f,1.25f,.65f,.48f,.62f,.7f,1.5f,.88f,1.1f,1.4f,.80f,.5f,1.55f,1.1f,1.1f,1.4f,.5f,.96f,.45f};
 static const float xx[]={.40f,.25f,.28f,.10f,.24f,.53f,.26f,.40f,.30f,.7f,.15f,.6f,.25f,.55f,.25f,.02f,.03f,.70f,.48f,.6f,.15f,.28f};
 static const float zz[]={.38f,.26f,.45f,.03f,.40f,.88f,.26f,.38f,.22f,.03f,.1f,.04f,.02f,.38f,.37f,.45f,.55f,.03f,.40f,.10f,.15f,.35f};
 if(type<0||type>=22)return 0;*x=hx[h]+(s?1:-1)*(type==19?4.12f:ax[type]);*z=hz[h]+bz[type];*y=f*3+yy[type];*rx=xx[type];*rz=zz[type];return 1;
}
static int furniture_solid150(float x,float z){int f=(int)clamp((float)floor((g.playerY+.15f)/3),0,4);for(int h=0;h<NH;h++){if(ab(x-hx[h])>4.8f||ab(z-hz[h])>4.4f)continue;for(int s=0;s<2;s++)for(int t=0;t<22;t++){if(t!=0&&t!=1&&t!=2&&t!=5&&t!=6&&t!=7&&t!=8&&t!=13&&t!=14&&t!=18&&t!=21)continue;
 if(t==6&&g.chairMoved200[unit150(h,f,s)])continue;
 if(physicsActor150>=0&&physicsActor150<NN&&PERSON(physicsActor150).home==h&&physicsActor150/NH==s&&((t==5&&sleepBlend150[physicsActor150]>0)||(t==6&&g.residentHome[physicsActor150]>0)))continue;
 static const float top180[22]={1.96f,.82f,1.9f,0,0,.90f,1.15f,.97f,1.25f,0,0,0,0,.96f,1.1f,0,0,0,2.3f,0,0,.76f};if(g.playerY-f*3>top180[t])continue;float a,y,b,rx,rz;object150(h,f,s,t,&a,&y,&b,&rx,&rz);if(ab(x-a)<rx&&ab(z-b)<rz)return 1;}}
 return 0;}
/* A swept upright circle, sampled at a maximum of 3.5cm, plus exact circular dynamic bodies. */
static int body_clear150(float x,float z,float y,float radius,int people){float old=g.playerY;g.playerY=y;int ok=1;for(int k=-1;k<16;k++){float a=k<0?0:k*PI/8,xx=x+(k<0?0:(float)cos(a)*radius),zz=z+(k<0?0:(float)sin(a)*radius);if(solid(xx,zz)||(y<1.55f&&!navStatic150&&traffic_solid140(xx,zz))){ok=0;break;}}g.playerY=old;if(!ok)return 0;
 if(!people&&physicsActor150>=0&&!navStatic150&&ab(g.playerY-y)<1.7f&&dst(x,z,g.x,g.z)<radius+.27f)return 0;
 if(people&&y<1.7f)for(int i=0;i<NN;i++){if(sleepBlend150[i]>.5f)continue;float r=radius+(npc_child(i)?.21f:.28f);if(dst(x,z,PERSON(i).x,PERSON(i).z)<r)return 0;}return 1;}
/* Never leave a player center inside furniture after load, wake or a fresh world. */
static void safe_spawn160(void){int old=physicsActor150;physicsActor150=-1;
 if(body_clear150(g.x,g.z,g.playerY,.28f,0)){physicsActor150=old;return;}
 float ox=g.x,oz=g.z;int h=-1;for(int n=0;n<NH;n++)if(ab(ox-hx[n])<4.8f&&ab(oz-hz[n])<4.4f){h=n;break;}
 if(h>=0){int side=ox>hx[h];for(int j=0;j<29;j++)for(int k=0;k<21;k++){float x=hx[h]+(side?1:-1)*(.35f+k*.2f),z=hz[h]-1.55f+j*.2f;if(body_clear150(x,z,g.playerY,.28f,0)){g.x=x;g.z=z;physicsActor150=old;return;}}}
 for(float r=.25f;r<=3;r+=.25f)for(int k=0;k<16;k++){float a=k*PI/8,x=ox+cos(a)*r,z=oz+sin(a)*r;if(body_clear150(x,z,g.playerY,.28f,0)){g.x=x;g.z=z;physicsActor150=old;return;}}
 g.x=hx[0]-2.4f;g.z=hz[0]-.6f;g.playerY=0;physicsActor150=old;
}
static int npc_age150(int i){if(npc_child(i))return 9+(hash32(g.worldSeed+i*313)%7);if(npc_elder141(i))return 66+(hash32(g.worldSeed+i*47)%18);return 19+(hash32(g.worldSeed+i*137)%43);}
static const char*npc_job150(int i){int age=npc_age150(i);if(age<18)return "школа № 17";if(age>=65)return "пенсия";static const char*j[]={"слесарь ЖЭКа","продавец","почтальон","водитель","ремонтная бригада","дворник","медрегистратор","оператор котельной"};return j[hash32(g.worldSeed+i*73)%8];}
static int schedule150(int i){float h=g.hours;int age=npc_age150(i);if(h>=22.5f||h<6.5f)return 15;if((h>=7&&h<7.5f)||(h>=13&&h<13.5f)||(h>=19&&h<19.5f))return 13;if((h>=10&&h<10.16f)||(h>=16&&h<16.16f)||(h>=21&&h<21.16f))return 14;if(age>=18&&age<65&&i%3==0&&((h>=9.5f&&h<9.75f)||(h>=12&&h<12.25f)||(h>=18&&h<18.25f)))return 9;if(age<65&&g.day%7<5&&h>=8&&h<(age<18?13:17))return 2;return age>=65?12:4;}
static int bench_release150(int i,float dt){int n=npcBench141[i];if(n<0)return 0;int sched=schedule150(i),wanted=sched==12||(sched==4&&i%8==3&&g.hours>=12&&g.hours<16);if(wanted)return 0;Citizen*c=&PERSON(i);npcMoving[i]=0;npcGait[i]=0;c->activity=12;sitting141[i]=mx(0,sitting141[i]-dt*2);if(sitting141[i]>.01f)return 1;float bx,bz;bench_position141(n,&bx,&bz);float target=bz-1.2f,d=target-c->z;ignoreBench141=n;float step=clamp(d,-dt,dt);if(nav_clear(c->x,c->z+step))c->z+=step;ignoreBench141=-1;if(ab(c->z-target)<.08f){npcBench141[i]=-1;c->activity=4;citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;}else npcMoving[i]=1;return 1;}
static int sleep_tick150(int i,float dt,int night){Citizen*c=&PERSON(i);int h=c->home,s=i/NH;float sign=s?1:-1,bedx=hx[h]+sign*BED_A,bedz=hz[h]+BED_B-BED_HALF_LEN+.01f;
 if((night&&g.residentHome[i]>=8)||sleepBlend150[i]>0)g.things150[unit150(h,0,s)]|=1u<<17;
 if(night&&g.residentHome[i]>=8){sleepBlend150[i]=mx(.01f,sleepBlend150[i]);float tx=bedx,tz=bedz;if(c->z<hz[h]+1.40f){tx=hx[h]+sign*2.3f;tz=hz[h]+1.5f;}else if(ab(c->x-bedx)>.08f){tz=hz[h]+1.5f;}float dx=tx-c->x,dz=tz-c->z,d=dst(c->x,c->z,tx,tz),st=mn(d,dt*.8f);if(d>.06f){if(nav_clear(c->x+dx*st/mx(.001f,d),c->z+dz*st/mx(.001f,d))){c->x+=dx*st/mx(.001f,d);c->z+=dz*st/mx(.001f,d);}npcMoving[i]=1;c->activity=4;}else{sleepBlend150[i]=mn(1,sleepBlend150[i]+dt*2);npcMoving[i]=0;npcGait[i]=0;c->activity=15;c->facing=0;g.lifeEnergy[i]=mn(100,g.lifeEnergy[i]+dt*.08f);}return 1;}
 if(sleepBlend150[i]>0){c->activity=4;float tx=hx[h]+sign*2.3f,tz=hz[h]+.4f;
 if(c->z>hz[h]+1.58f){tx=bedx;tz=hz[h]+1.5f;}
 else if(ab(c->x-tx)>.10f){tz=hz[h]+1.5f;}
 float d=dst(c->x,c->z,tx,tz),st=mn(d,dt*.9f);
 if(nav_clear(c->x+(tx-c->x)*st/mx(.001f,d),c->z+(tz-c->z)*st/mx(.001f,d))){c->x+=(tx-c->x)*st/mx(.001f,d);c->z+=(tz-c->z)*st/mx(.001f,d);}
 if(c->z<hz[h]+.5f)sleepBlend150[i]=0;else sleepBlend150[i]=.01f;return 1;}return 0;}
static const char*use_names150[]={"Холодильник / открыть, взять продукты","Чайник / вскипятить и выпить чай","Шкаф / открыть или закрыть","Свет / выключатель","Телевизор / питание","Кровать / отдохнуть","Стул / присесть","Плита / включить конфорку","Раковина / кран","Шторы / открыть или закрыть","Книга / прочитать заметку","Дверь квартиры / открыть или закрыть","Табличка квартиры / осмотреть","Стол / осмотреть","Туалет / смыть","Зеркало / посмотреть","Дверь санузла / открыть или закрыть","Дверь комнаты / открыть или закрыть","Душ / включить воду","Батарея / осмотреть","Телефон / снять трубку","Тумба / открыть ящик"};
static int use150(int perform){
 for(int h=0;h<NH;h++){if(dst(g.x,g.z,hx[h],hz[h])>16)continue;int f=(int)clamp((float)floor((g.playerY+.15f)/3),0,4);for(int s=0;s<2;s++){
 for(int t=0;t<22;t++){if(t==4||t==10||t==20||(t==6&&g.chairMoved200[unit150(h,f,s)]))continue;float x,y,z,rx,rz;object150(h,f,s,t,&x,&y,&z,&rx,&rz);if(dst(g.x,g.z,x,z)>2.0f||!look_at(x,y,z,t==3?.96f:.94f))continue;int id=unit150(h,f,s);if(t!=11&&t!=12&&(g.z<hz[h]-1.8f||(g.x>hx[h])!=s))continue;
 if(!perform){SN(prompt,sizeof(prompt),"E — %s",use_names150[t]);return 1;}
 if(t==11){if((g.flatDoors150[h]&(1u<<(f*2+s)))&&ab(g.z-z)<.48f){say("Отойди от дверного проёма.");return 1;}g.flatDoors150[h]^=1u<<(f*2+s);nav_refresh(x,z);audio(12);}
 else if(t==0){g.things150[id]^=2;if(g.foodDay150[id]!=g.day){if(inv_add(1,1)){g.foodDay150[id]=g.day;say(h==0&&s==0?"В холодильнике нашлась порция еды.":"Взята чужая еда. Хозяин заметит пропажу.");if(h||s)g.crimeHeat=mn(100,g.crimeHeat+8);}}else say("На сегодня продуктов больше нет.");}
 else if(t==1)kettle_use200(id);
 else if(t==8){g.things150[id]^=1u<<8;audio(2);if(g.things150[id]&256){g.thirst=mn(100,g.thirst+15);say("Кран открыт. Ты попил воды и вымыл руки.");}else say("Кран закрыт.");}
 else if(t==5){if(h==0&&s==0){screen=17;say("Отдых в своей квартире.");}else say("Это чужая кровать.");}
 else if(t==6){seated=1000+id;g.x=x;g.z=z;g.rot=yaw=0;g.look=pitch=0;say("Сидишь за столом. E или движение — встать.");}
 else if(t==10)say("На полях: «В среду отключат горячую воду. Не забыть оплатить свет». ");
 else if(t==12){char a[180];SN(a,sizeof(a),"Лесная, %d / подъезд 1 / этаж %d / квартира %d",h+1,f+1,f*2+s+1);say(a);}
 else if(t==13)say("На столе кружка, хлеб и старая квитанция за отопление.");
 else if(t==14){g.bladder=mx(0,g.bladder-50);g.bowel=mx(0,g.bowel-50);audio(2);say("Вода набирается в бачок.");}
 else if(t==16||t==17){if(t==16&&ab(g.x-x)<.5f&&ab(g.z-z)<.85f||t==17&&ab(g.z-z)<.5f&&ab(g.x-x)<1.0f){say("Отойди от дверного проёма.");return 1;}g.things150[id]^=1u<<t;audio(12);nav_refresh(x,z);}
 else if(t==18){g.things150[id]^=1u<<18;audio(2);say(g.things150[id]&(1u<<18)?"Зашумел душ. Вода медленно становится тёплой.":"Душ выключен.");}
 else if(t==19)say("Чугунная батарея тёплая. Тихо стучат трубы.");
 else if(t==20){g.things150[id]^=1u<<20;say(g.things150[id]&(1u<<20)?"В трубке гудки. Никто не звонит.":"Трубка возвращена на рычаг.");}
 else if(t==21){g.things150[id]^=1u<<21;audio(0);say("В ящике старые квитанции, ключ и моток ниток.");}
 else if(t==15)say("Усталое лицо. Ещё один обычный день.");
 else{g.things150[id]^=1u<<t;audio(0);say(t==3?((g.things150[id]&(1u<<3))?"Свет выключен.":"Свет включён."):t==4?"Щёлкнул старый телевизор.":t==7?"Щёлкнула ручка конфорки.":t==9?"Шторы сдвинулись.":"Скрипнула дверца шкафа.");}return 1;}
 }}return 0;}
