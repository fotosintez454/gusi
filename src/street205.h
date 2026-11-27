/* v0.20.5: живой двор. Уличная жизнь 1990-х вокруг панелек:
   ларёк, таксофон, афишная стена, домино, колонка, бочка-костёр,
   футбольные ворота, «клад» под тополем, гитарист, объявления,
   бабки на лавке. Состояние живёт один сеанс: формат сохранения не меняется. */
#define S205_KIOK 0
#define S205_PHON 1
#define S205_WALL 2
#define S205_DOMI 3
#define S205_PUMP 4
#define S205_BARR 5
#define S205_GOAL 6
#define S205_STAS 7
#define S205_MUSI 8
#define S205_BOAR 9
#define S205_BABA 10
typedef struct{int type;float x,z,rot;} Poi205;
static const Poi205 poi205[]={
 {S205_KIOK,58,-36,PI},{S205_PHON,-56,-9.2f,0},{S205_WALL,36,-10.5f,PI},
 {S205_DOMI,-14,-35,0},{S205_PUMP,-38,-39,0},{S205_BARR,20,-38,0},
 {S205_GOAL,0,-41,0},{S205_STAS,28,-37,0},{S205_MUSI,44,20,PI},
 {S205_BABA,52,-34.5f,PI}
};
#define POI205_COUNT ((int)(sizeof(poi205)/sizeof(poi205[0])))

static struct{
 float clock,ballT,ballVX,ballVZ,strum,flick,smoke;
 int dominoDay,dominoPlayed,dominoWon,phoneUsed,postersTorn,tags,stashDay,stashFound;
 int beerBought,cigsBought,lastHint;
} st205;

static float s205_day(void){return clamp((float)sin((g.hours-6)/24*2*PI)*.8f+.35f,.12f,1);}
static int s205_night(void){return g.hours>=19||g.hours<6.5f;}
static int s205_fire_lit(void){return winterEnabled||g.hours>=17||g.hours<7;}

/* Solid footprints: bundles, crates and the kiosk corner. */
static void s205_prop_solid(Poi205*p){
 switch(p->type){
 case S205_KIOK: env_add(p->x,p->z-1.45f,1.9f,1.75f,8,900);env_add(p->x+2.55f,p->z-.4f,.55f,.55f,8,901);break;
 case S205_PHON: env_add(p->x,p->z,.55f,.55f,8,902);break;
 case S205_WALL: env_add(p->x,p->z,3.1f,.15f,8,903);break;
 case S205_DOMI: env_add(p->x,p->z,.85f,.85f,8,904);break;
 case S205_PUMP: env_add(p->x,p->z,.35f,.35f,8,905);break;
 case S205_BARR: env_add(p->x,p->z,.36f,.36f,8,906);break;
 case S205_GOAL: env_add(p->x-1.5f,p->z,.12f,.12f,8,907);env_add(p->x+1.5f,p->z,.12f,.12f,8,908);break;
 case S205_STAS: env_add(p->x,p->z,.28f,.28f,8,909);break;
 case S205_MUSI: env_add(p->x,p->z,.35f,.35f,8,910);break;
 case S205_BABA:env_add(p->x-1.1f,p->z,.28f,.28f,8,911);env_add(p->x+.9f,p->z-.2f,.28f,.28f,8,912);break;
 }
}
static void street_build205(void){
 for(int n=0;n<POI205_COUNT;n++)s205_prop_solid((Poi205*)&poi205[n]);
 for(int h=0;h<4;h++)env_add(hx[h]+4.3f,hz[h]-4.95f,.62f,.10f,8,920+h);
}

/* One-line prompts; selected index is shared by prompt and use. */
static int s205_selected=-1;
static int s205_pick(void){
 float best=4.4f;int found=-1;
 for(int n=0;n<POI205_COUNT;n++){Poi205*p=(Poi205*)&poi205[n];float d=dst(g.x,g.z,p->x,p->z);if(d>4.4f)continue;
  float eye=p->type==S205_BOAR?1.5f:p->type==S205_WALL?1.4f:p->type==S205_PUMP?1.0f:p->type==S205_STAS?.4f:1.2f;
  if(!look_at(p->x,eye,p->z,.92f))continue;if(d<best){best=d;found=n;}}
 for(int h=0;h<4;h++){float x=hx[h]+4.3f,z=hz[h]-4.95f,d=dst(g.x,g.z,x,z);if(d<best&&look_at(x,1.5f,z,.92f)){best=d;found=POI205_COUNT+h;}}
 s205_selected=found;return found;
}
#define ST205_BOARD(i) ((i)>=POI205_COUNT)

static int street_prompt205(void){
 int n=s205_pick();if(n<0)return 0;
 if(ST205_BOARD(n)){SN(prompt,sizeof(prompt),"E — прочитать объявления у подъезда");return 1;}
 Poi205*p=(Poi205*)&poi205[n];
 switch(p->type){
 case S205_KIOK:SN(prompt,sizeof(prompt),"E — ларёк «ПИВО·ВОДКА»: пиво %d ₽ · сигареты %d ₽",g.beer?35:25,g.cigarettes?30:20);break;
 case S205_PHON:SN(prompt,sizeof(prompt),"E — таксофон: позвонить (2 ₽)");break;
 case S205_WALL:SN(prompt,sizeof(prompt),st205.postersTorn<3?"E — стена афиш: сорвать афишу":"E — стена афиш: уже ободрана");break;
 case S205_DOMI:SN(prompt,sizeof(prompt),"E — сыграть в домино с мужиками (10 ₽)");break;
 case S205_PUMP:SN(prompt,sizeof(prompt),"E — колонка: набрать воды");break;
 case S205_BARR:SN(prompt,sizeof(prompt),s205_fire_lit()?"E — погреться у бочки":"E — ржавая бочка у гаражей");break;
 case S205_GOAL:SN(prompt,sizeof(prompt),"E — ударить по мячу");break;
 case S205_STAS:SN(prompt,sizeof(prompt),st205.stashDay==g.day?"E — земля под тополем: уже разрыто":"E — разгрести землю под тополем");break;
 case S205_MUSI:SN(prompt,sizeof(prompt),g.money>=5?"E — послушать гитариста (монета 5 ₽)":"E — послушать гитариста");break;
 case S205_BABA:SN(prompt,sizeof(prompt),"E — спросить у бабок про двор");break;
 }
 return 1;
}

static const char*s205_poster205(int i){
 static const char*posters[]={
  "«Кино» — последний концерт на бобине, переписан с третьей руки.",
  "Афиша: «Гражданская Оборона» в ДК, вход три рубля, милиция уже в курсе.",
  "Плакат: «Аукцыон» — полустёрт дождём, остался только бас.",
  "Самодельная афиша: «СОЮЗ ПИВО·ВОДКА», оторвана по нижнему краю."};
 return posters[i&3];
}
static const char*s205_ad205(int i){
 static const char*ads[]={
  "Куплю гараж с ямой. Звонить после восьми.",
  "Сдам комнату в коммуналке, без удобств, тихо.",
  "Меняю двухкомнатную на однокомнатную с доплатой.",
  "Пропала собака, рыжая, отзывается на Жучку.",
  "Ремонт обуви, приём макулатуры, рядом с ларьком.",
  "Уроки английского. Первое занятие бесплатно.",
  "Продам ковёр, люстру и две кассеты «Наутилуса».",
  "Ищу напарника на шабашку. Инструмент свой."};
 return ads[i&7];
}

static void s205_money(int amount){g.money=mx(0,g.money+amount);}
static int street_use205(int perform){
 int n=s205_selected;if(n<0||n!=s205_pick())return 0;
 if(!perform)return 1;
 if(ST205_BOARD(n)){int i=(int)(rnd()*(1<<20));event_log("street_board",0);
  say(s205_ad205(i));if(rnd()<.25f){g.stress-=3;if(g.stress<0)g.stress=0;}return 1;}
 Poi205*p=(Poi205*)&poi205[n];
 switch(p->type){
 case S205_KIOK:
  if(g.beer>=6){say("Продавщица: «Куда тебе ещё, милок? Иди проспись».");return 1;}
  if(g.money<25){say("Продавщица: «Нет денег — нет пива». Ставни захлопнулись.");return 1;}
  s205_money(-25);inv_add(6,1);st205.beerBought++;audio(1);
  say(rnd()<.3f?"«Свежее, вчерашнее», — кивнула продавщица и отсчитала сдачу.":"Бутылка холодная, этикетка отклеена. За углом уже свистят.");
  break;
 case S205_PHON:
  if(g.money<2){say("Таксофон жрёт двушку, а в кармане пусто.");return 1;}
  s205_money(-2);audio(5);st205.phoneUsed++;
  {int k=(int)(rnd()*5);
   if(k==0)say("На том конце долго не берут. Наконец: «Алё? Кто это?» — и короткие гудки.");
   else if(k==1){say("Мать: «Ты поел? Я тебе котлеты оставила». На душе теплее.");g.stress=mx(0,g.stress-10);}
   else if(k==2){say("Знакомый: «Есть халтура на субботу, подгребай к гаражам». Записал адрес.");g.reputation++;}
   else if(k==3){say("Позвонил в контору — там только «перезвоните позже».");}
   else {say("В трубке чужой разговор про концерт. Слушал минуту, не перебивал.");g.stress=mx(0,g.stress-4);}}
  event_log("street_phone",0);break;
 case S205_WALL:
  if(st205.postersTorn<3){st205.postersTorn++;audio(2);g.stress=mx(0,g.stress-5);
   say(s205_poster205((int)(rnd()*4)));
   if(st205.postersTorn==1)say("Афиша «Кино» оторвалась с куском штукатурки. Теперь она твоя.");
  }else if(st205.tags<3&&witnessed()){g.crimeHeat=mn(100,g.crimeHeat+6);st205.tags++;
   say("Оставил маркером поперёк стены. Бабка у подъезда уже смотрит вслед.");}
  else say("Стена ободрана до бетона. Только гвозди и старые следы скотча.");break;
 case S205_DOMI:
  if(g.money<10){say("Мужики: «Денег нет — играй на спички». Поставил спичку, проиграл.");return 1;}
  s205_money(-10);st205.dominoPlayed++;audio(3);
  if(rnd()<.5f){s205_money(25);st205.dominoWon++;g.reputation++;g.stress=mx(0,g.stress-6);
   say("«Рыба!» — положил последнюю костяшку. Мужики крякнули и отсчитали двадцать пять.");}
  else{say("Не повезло: «дупель» на руке. Мужики довольно закурили, десятка ушла.");}
  st205.dominoDay=g.day;event_log("street_domino",st205.dominoWon);break;
 case S205_PUMP:
  audio(4);
  if(g.water<3){g.water++;say("Набрал воды в бутыль. Колонка скрипнула и затихла.");}
  else{g.thirst=mn(100,g.thirst+30);say("Вода ржавая, но холодная. Напился из ладоней.");}
  g.stress=mx(0,g.stress-2);break;
 case S205_BARR:
  if(s205_fire_lit()){g.energy=mn(100,g.energy+6);g.stress=mx(0,g.stress-8);g.illness=mx(0,g.illness-2);
   audio(14);say("У бочки горят обрезки досок. Рядом мужик в телогрейке молча кивнул.");}
  else if(rnd()<.5f){if(g.money<5)s205_money(0);s205_money(4);say("На дне бочки — бутылки. Сдал две, выручил четыре рубля.");}
  else say("Ржавая бочка, дно в золе. Кто-то оставил окурок и надежду.");break;
 case S205_GOAL:
  st205.ballT=.9f;st205.ballVX=(rnd()-.5f)*.4f;st205.ballVZ=3.4f;audio(2);
  g.energy=mx(0,g.energy-2);g.stress=mx(0,g.stress-4);
  if(rnd()<.35f){g.reputation++;say("Мяч влетел в сетку, обтянутую старым капроном. Гол.");}
  else say("Ударил по мячу — он скакнул по асфальту и укатился к луже.");break;
 case S205_STAS:
  if(st205.stashDay==g.day){say("Здесь уже перекопано. Только битое стекло и мокрая земля.");return 1;}
  st205.stashDay=g.day;audio(4);
  {int k=(int)(rnd()*4);if(k==0){s205_money(18);say("Под корнями — жестяная банка с восемнадцатью рублями.");st205.stashFound++;}
   else if(k==1){s205_money(35);say("Пакет с мелочью, насквозь мокрый. Тридцать пять рублей.");st205.stashFound++;}
   else say("Только пробки, окурок и дохлый червяк. Место пахнет сыростью.");}
  event_log("street_stash",st205.stashFound);break;
 case S205_MUSI:
  if(g.money>=5){s205_money(-5);g.reputation++;g.stress=mx(0,g.stress-10);
   say("Гитарист кивнул и заиграл что-то на трёх аккордах. Прохожий остановился послушать.");}
  else{g.stress=mx(0,g.stress-5);say("Гитарист играет для всех. Ты постоял, слушая, как дребезжит шестая струна.");}
  audio(7);st205.strum=3;break;
 case S205_BABA:
  st205.lastHint=(int)(rnd()*3);audio(3);
  if(st205.lastHint==0)say("Бабка: «Под тополем вон там земля просела — там ещё до войны прятали».");
  else if(st205.lastHint==1)say("Бабка: «Афиши у стены рвут, а под ними весь район написан».");
  else say("Бабка: «Доминошники с обеда режутся. Выиграешь — не радуйся, проиграешь — не плачь».");
  g.stress=mx(0,g.stress-3);break;
 }
 return 1;
}

/* ------------------------------------------------------------------ draw */

static void s205_ground(float x,float z,float w,float d,U32 c){surfaceTex=&mat_asphalt;box(x,.028f,z,w,.02f,d,c);surfaceTex=0;}
static void s205_plank(float x,float y,float z,float w,float d,U32 c){surfaceTex=&mat_wood;box(x,y,z,w,.07f,d,c);surfaceTex=0;}
static void s205_post(float x,float z,float y0,float y1,float r,U32 c){beam(vp(x,y0,z),vp(x,y1,z),r,r,c);}

static void s205_man(float x,float z,float yaw,int coat,int pants,int hat,int sit,float phase){
 float c=(float)cos(yaw),s=(float)sin(yaw);
 float sh=phase*.32f;
 if(sit){
  soft_mesh(x,1.02f,z,.20f,.24f,.19f,yaw,coat);
  soft_mesh(x,1.30f,z,.11f,.11f,.10f,yaw,(hat>=0)?hat:0x9c8a78);
  box(x-.20f,.70f,z,.10f,.34f,.42f,pants);box(x+.20f,.70f,z,.10f,.34f,.42f,pants);
  box(x-.20f,.32f,z+s*.52f,.10f,.10f,.46f,pants);box(x+.20f,.32f,z+s*.52f,.10f,.10f,.46f,pants);
  box(x-.30f,1.10f,z-s*.22f,.10f,.10f,.34f,coat);box(x+.30f,1.10f,z-s*.22f,.10f,.10f,.34f,coat);
 }else{
  box(x-.11f,.42f,z,.11f,.84f,.13f,pants);box(x+.11f,.42f,z,.11f,.84f,.13f,pants);
  soft_mesh(x,1.10f,z,.21f,.28f,.17f,yaw,coat);
  soft_mesh(x,1.47f,z,.11f,.12f,.10f,yaw,(hat>=0)?hat:0x9c8a78);
  box(x-.29f,.95f,z-s*.10f,.10f,.10f,.52f,coat);box(x+.29f,.95f,z-s*.10f,.10f,.10f,.52f,coat);
  box(x-.19f,.09f,z+s*.18f,.13f,.18f,.26f,0x2c2b28);box(x+.19f,.09f,z+s*.18f,.13f,.18f,.26f,0x2c2b28);
 }
 (void)c;
}

static void s205_kiosk(Poi205*p,float day){
 float x=p->x,z=p->z;int lit=s205_night();
 s205_ground(x,z,6.4f,5.2f,0x6f6f68);
 surfaceTex=&mat_metal;box(x,1.5f,z,4.5f,3.0f,3.2f,0x767b6e);surfaceTex=0;
 box(x,1.5f,z-1.62f,4.3f,.9f,.06f,0x4c5549);
 /* витрина ларька: тёплый свет и прилавок */
 box(x,1.75f,z-1.66f,2.4f,1.15f,.06f,lit?0xe8c179:0x566a63);
 box(x,1.13f,z-1.70f,2.6f,.14f,.3f,0x3a3f38);
 box(x,2.36f,z-1.70f,2.6f,.12f,.26f,0x3a3f38);
 box(x,1.75f,z-1.70f,.10f,1.15f,.05f,0x333b33);
 if(lit)box(x,1.75f,z-1.74f,2.2f,1.0f,.02f,0xb98f4e);
 box(x,2.92f,z,4.9f,.16f,3.6f,0x3f463d);
 box(x,3.28f,z-1.78f,3.8f,.52f,.07f,0x7d2b23);
 for(int k=0;k<4;k++)box(x-1.25f+k*.82f,3.28f,z-1.83f,.34f,.24f,.03f,0xd8c9a4);
 /* полосатый навес над прилавком: снизу тоже виден */
 for(int k=0;k<6;k++){float a=x-2.3f+k*.77f;U32 c=k%2?0xb7a488:0x6b3f34;
  quad(vp(a,2.9f,z-1.72f),vp(a+.77f,2.9f,z-1.72f),vp(a+.77f,2.42f,z-2.95f),vp(a,2.42f,z-2.95f),c,0,1.0f);
  quad(vp(a,2.42f,z-2.95f),vp(a+.77f,2.42f,z-2.95f),vp(a+.77f,2.9f,z-1.72f),vp(a,2.9f,z-1.72f),mix(c,0x1c1c18,.35f),0,.8f);}
 box(x,2.40f,z-2.96f,4.7f,.10f,.08f,0x4a3428);
 /* ящики и бутылки */
 surfaceTex=&mat_wood;box(x+2.45f,.24f,z-.4f,1.0f,.48f,1.0f,0x7b5b3d);box(x+2.45f,.72f,z-.4f,1.0f,.46f,1.0f,0x6c4f36);surfaceTex=0;
 for(int k=0;k<4;k++)s205_post(x+2.2f+k*.15f,z-1.0f,.5f,1.05f,.035f,(k%2)?0x3c5a3a:0x594c31);
 box(x-2.55f,.42f,z+.9f,.55f,.8f,.55f,0x4a5148);
 if(lit)box(x,3.05f,z-2.0f,1.5f,.06f,.3f,0xffd58c);
 box(x+2.0f,1.3f,z+1.7f,.7f,1.9f,.7f,0x3d443d); /* урна-будка */
}

static void s205_phone(Poi205*p,float day){
 float x=p->x,z=p->z;
 s205_ground(x,z,2.4f,2.4f,0x777a70);
 box(x,1.25f,z,.16f,.05f,1.35f,0x303d3a);
 for(int s=-1;s<=1;s+=2){box(x+s*.62f,1.22f,z,.07f,2.35f,.07f,0x3f4d49);
  box(x+s*.62f,1.35f,z-1.28f,.07f,2.05f,.07f,0x3f4d49);
  box(x+s*.62f,2.36f,z-.64f,.07f,.07f,1.32f,0x3f4d49);}
 box(x,1.15f,z-1.28f,1.3f,1.9f,.03f,0x5d7a7c);
 box(x,1.15f,z-.02f,1.3f,1.9f,.03f,0x5d7a7c);
 box(x,2.42f,z-.65f,1.45f,.14f,1.55f,0x45514c);
 box(x,2.58f,z-.65f,.22f,.16f,.22f,s205_night()?0xffe1a2:0x8b8f7d);
 box(x+.34f,1.45f,z-.55f,.26f,.62f,.18f,0x333b34);
 box(x+.34f,1.55f,z-.46f,.20f,.30f,.05f,0x8a928a);
 s205_post(x,1.45f,z-.55f,.3f,.02f,0x22282a);
}

static void s205_wall(Poi205*p,float day){
 float x=p->x,z=p->z;
 surfaceTex=&mat_brick;box(x,1.15f,z,6.2f,2.3f,.28f,0x8a7d68);surfaceTex=0;
 box(x,2.38f,z,6.36f,.14f,.42f,0x6d6354);
 for(int k=0;k<3;k++)box(x-2.4f+k*2.4f,1.15f,z-.16f,.12f,2.3f,.05f,0x746a5a);
 /* афиши: рваные края, разный цвет и наклон бумаги */
 for(int k=0;k<5;k++){int torn=(st205.postersTorn>k);
  float ax=x-2.35f+k*1.18f,ay=1.28f+(k%3)*.22f;
  U32 c=(U32[]){0xd8d2bb,0x8f3a33,0xc7b98d,0x3d5570,0xcfc0a3}[k%5];
  if(torn){plane(ax,ay+.28f,z-.18f,.60f,.28f,0xb9b2a0,0);box(ax-.18f,ay+.06f,z-.20f,.3f,.02f,.02f,0xa49c88);continue;}
  plane(ax,ay,z-.18f,.60f,.78f,c,0);
  for(int s=0;s<3;s++)box(ax-.18f+s*.18f,ay-.16f,z-.20f,.12f,.03f,.02f,0x1e1c18);
  box(ax,ay+.22f,z-.20f,.5f,.04f,.02f,0x1e1c18);}
 /* граффити: постоянные теги и те, что добавил игрок */
 for(int k=0;k<3;k++){float gx=x-1.9f+k*1.7f;
  box(gx,1.86f,z-.19f,1.0f,.07f,.02f,(U32[]){0x9c3f3a,0x3e5a72,0x6f7a3f}[k%3]);
  box(gx+.25f,2.00f,z-.19f,.72f,.05f,.02f,(U32[]){0x2f3d57,0x7d2b33,0x3c4d3a}[k%3]);
  box(gx-.35f,1.72f,z-.19f,.45f,.05f,.02f,(U32[]){0x3c4d3a,0x9c3f3a,0x2f3d57}[k%3]);}
 for(int k=3;k<3+st205.tags&&k<6;k++){float gx=x-1.2f+(k-3)*1.2f;
  box(gx,1.45f,z-.185f,.9f,.07f,.02f,(U32[]){0xb4a24a,0x7d2b33,0x3e5a72}[k%3]);
  box(gx+.15f,1.30f,z-.185f,.6f,.05f,.02f,0x2b2b26);}
 box(x-2.2f,.22f,z-.4f,1.2f,.45f,.5f,0x5d4a3a);
}

static void s205_domino(Poi205*p,float day){
 float x=p->x,z=p->z;
 s205_ground(x,z,4.6f,4.2f,0x767a6f);
 surfaceTex=&mat_wood;box(x,.80f,z,1.25f,.10f,1.25f,0x6e5334);surfaceTex=0;
 for(int a=-1;a<=1;a+=2)for(int b=-1;b<=1;b+=2)box(x+a*.48f,.40f,z+b*.48f,.10f,.78f,.10f,0x57412c);
 for(int a=-1;a<=1;a+=2)for(int b=-1;b<=1;b+=2)box(x+a*.48f,.44f,z+b*.48f,.10f,.78f,.10f,0x57412c);
 /* кости и стаканы */
 for(int k=0;k<6;k++)box(x-.35f+(k%3)*.30f,.87f,z-.25f+(k/3)*.34f,.11f,.03f,.22f,0xd9d3bd);
 box(x+.38f,.90f,z-.35f,.09f,.18f,.09f,0x8f9a86);
 /* игроки: трое стариков и один в кепке */
 s205_man(x-.95f,z-.2f,PI/2,0x4f5a4a,0x3a3f3b,0x6a6a58,1,sinf(st205.clock*.9f));
 s205_man(x+.95f,z+.2f,-PI/2,0x5b4d3e,0x424640,0x2f3430,1,sinf(st205.clock*1.1f+1));
 s205_man(x-.2f,z-1.0f,0,0x6a5c48,0x3d4038,0x7b6d52,1,sinf(st205.clock*.8f+2));
 s205_man(x+.2f,z+1.0f,PI,0x47525a,0x353a3c,-1,1,sinf(st205.clock*1.3f+3));
 if(g.hours>=17)box(x,.62f,z+.0f,.22f,.10f,.22f,0xf0c179);
}

static void s205_pump(Poi205*p,float day){
 float x=p->x,z=p->z;
 s205_ground(x,z,2.0f,1.5f,0x6e7168);
 box(x,.22f,z,.9f,.44f,.9f,0x747a70);
 s205_post(x,z,.2f,2.15f,.14f,0x5a6159);
 s205_post(x,z,2.15f,2.32f,.20f,0x47504b);
 beam(vp(x,1.62f,z),vp(x+.52f,1.5f,z),.05f,.05f,0x3d4642);
 beam(vp(x,1.86f,z-1.0f),vp(x,1.86f,z),.04f,.04f,0x3d4642);
 beam(vp(x,1.62f,z),vp(x,1.0f,z+.02f),.03f,.03f,0x39423e);
 box(x-.3f,.08f,z+.4f,.6f,.12f,.06f,0x4b4f48);
 box(x+.05f,.5f,z+.42f,.36f,.2f,.06f,0x4f574d);
 surfaceTex=&mat_asphalt;box(x+.1f,.035f,z+.75f,1.1f,.02f,.7f,0x4a4f46);surfaceTex=0;
}

static void s205_barrel(Poi205*p,float day){
 float x=p->x,z=p->z;int lit=s205_fire_lit();
 s205_ground(x,z,3.2f,3.2f,0x615f56);
 beam(vp(x,0,z),vp(x,.95f,z),.30f,.34f,0x6b4a35);
 beam(vp(x,.52f,z),vp(x,.98f,z),.35f,.35f,0x51402f);
 if(lit){
  for(int k=0;k<5;k++){float a=(k*.7f+st205.clock*2.2f);float r=.13f*sinf(st205.clock*3+k);
   soft_mesh(x+r,.98f+(k*.09f),z+cosf(a)*.10f,.13f,.16f,.12f,a,0xd07a2c);}
  soft_mesh(x,.92f,z,.24f,.2f,.24f,0,0xe8b25a);
  if(s205_night())box(x,.9f,z,.7f,.5f,.7f,0x7a3d1c);
  for(int k=0;k<4;k++){float t=fmodf(st205.clock*.35f+k*.25f,1.f);soft_mesh(x+.05f*sinf(k),(1.5f+t*2.0f),z+cosf(k)*.06f,.22f+t*.3f,.16f,.2f,0,0x6a6a60);}
 }
 /* бревно и ящик для сидения */
 s205_plank(x+1.15f,.46f,z+.1f,.35f,2.1f,0x6a4f36);
 box(x+1.15f,.22f,z-.7f,.3f,.44f,.3f,0x4c3b2a);box(x+1.15f,.22f,z+.9f,.3f,.44f,.3f,0x4c3b2a);
 s205_man(x-1.5f,z+.3f,PI/2,0x4a4a3f,0x33352f,0x5e5a48,0,0);
}

static void s205_goal(Poi205*p,float day){
 float x=p->x,z=p->z;
 s205_ground(x,z,5.2f,3.0f,0x6c7167);
 for(int s=-1;s<=1;s+=2){s205_post(x+s*1.5f,z,.0f,1.9f,.05f,0xc0b9a3);box(x+s*1.5f,1.9f,z,.1f,.06f,.1f,0xa39c88);}
 s205_post(x,z-.6f,1.9f,1.9f,.05f,0xc0b9a3);
 beam(vp(x-1.5f,1.9f,z),vp(x+1.5f,1.9f,z),.04f,.04f,0xb6af99);
 for(int k=0;k<8;k++){float a=x-1.5f+k*.42f;beam(vp(a,1.92f,z),vp(a,1.55f,z-.55f),.008f,.008f,0x8e8b7c);beam(vp(a,1.55f,z-.55f),vp(a,1.9f,z),.008f,.008f,0x8e8b7c);}
 for(int k=0;k<5;k++)beam(vp(x-1.5f+k*.75f,1.9f,z),vp(x-1.5f+k*.75f,1.0f,z-.55f),.008f,.008f,0x8e8b7c);
 float bx=x,by=.16f,bz=z+.9f;
 if(st205.ballT>0){st205.ballT-=1.f/60.f;float t=.9f-st205.ballT;bx=x+st205.ballVX*t*3;bz=z+.9f+st205.ballVZ*t;by=.16f+sinf(t*3.1f)*.35f;if(by<.14f)by=.14f;}
 soft_mesh(bx,by,bz,.11f,.11f,.11f,0,0xd8d5c8);
}

static void s205_stash(Poi205*p,float day){
 float x=p->x,z=p->z;
 surfaceTex=0;beam(vp(x,.0f,z),vp(x+.15f,3.4f,z-.1f),.16f,.10f,0x4a3a2b);
 beam(vp(x+.15f,2.1f,z-.1f),vp(x-1.2f,3.0f,z-.5f),.06f,.02f,0x46362a);
 beam(vp(x+.15f,2.4f,z-.1f),vp(x+1.1f,3.3f,z+.4f),.05f,.02f,0x46362a);
 soft_mesh(x,.08f,z,.75f,.16f,.62f,0,0x4b4033);
 if(st205.stashDay==g.day){soft_mesh(x,.05f,z,.42f,.10f,.34f,0,0x2c2820);box(x+.35f,.08f,z-.2f,.22f,.08f,.22f,0x5d5545);}
 else box(x-.6f,.20f,z+.5f,.5f,.4f,.06f,0x7c6a4c);
}

static void s205_music(Poi205*p,float day){
 float x=p->x,z=p->z;
 s205_ground(x,z,2.6f,2.6f,0x74776c);
 s205_man(x,z,PI,0x3f4750,0x2c3033,0x6d5d48,0,0);
 /* гитара и колонка-ящик */
 soft_mesh(x-.18f,1.02f,z-.28f,.10f,.26f,.16f,.4f,0x8a5f34);
 s205_post(x+.28f,z-.12f,.0f,.5f,.09f,0x5a4430);
 box(x-.45f,1.0f,z-.34f,.5f,.06f,.06f,0x2c1f15);
 box(x+.75f,.35f,z,.55f,.7f,.4f,0x2f3330);
 box(x+.75f,.62f,z-.21f,.34f,.26f,.03f,0x1e2422);
 box(x,0.02f,z+.7f,.7f,.06f,.5f,0x5c4c39);
 if(st205.strum>0)box(x+.3f,1.45f,z-.2f,.02f,.9f,.02f,0x9c8a78);
}

static void s205_board(float x,float z,int h){
 plane(x,1.45f,z-.02f,.95f,1.05f,0x8a7b5f,0);
 for(int k=0;k<5;k++){float a=x-.3f+(k%2)*.34f,b=1.78f-(k/2)*.34f;plane(a,b,z-.06f,.28f,.28f,(k%3)?0xd9d2ba:0xc3b48c,0);}
 plane(x,2.06f,z-.05f,.9f,.14f,0x3f4a44,0);
}

/* Провода, столбы, чёрные ливнёвки: инфраструктура 90-х. */
static void s205_pole(float x,float z,float h){
 s205_post(x,z,.0f,h,.13f,0x6b6252);
 box(x,h,z,.5f,.10f,.10f,0x4c463a);
 box(x,h-.55f,z,.14f,.14f,.14f,0x8b8574);
}
static void s205_wire_row(float z,int dir){
 float lit=s205_night()?1.f:.35f;
 for(int k=-5;k<=5;k++){float x=k*24.f;
  if(dst(camx,camz,x,z)>viewdist+26)continue;
  s205_pole(x,z,6.1f);
  if(k<5){float x2=x+24.f;if(dst(camx,camz,x2,z)>viewdist+40)continue;
   for(int w=0;w<2;w++){float y=5.9f-w*.28f;beam(vp(x,y,z),vp(x+12,y-.55f,z),.012f,.012f,0x2b2f2d);
    beam(vp(x+12,y-.55f,z),vp(x2,y,z),.012f,.012f,0x2b2f2d);}
   if(winterEnabled)beam(vp(x,5.6f,z),vp(x2,5.6f,z),.006f,.006f,0x3a3f3c);
  }
  for(int b=0;b<2;b++){if(world_value(9100+(int)(x*3)+b*7+(int)z)<.45f)continue;
   float bx=x+(world_value(9200+(int)x+b)-.5f)*10,by=5.55f;
   if(dst(camx,camz,bx,z)>viewdist)continue;
   soft_mesh(bx,by,z+(world_value(9300+b+(int)x)-.5f)*.2f,.06f,.055f,.05f,0,0x3a3a34);}
 }
}
static const float s205_cars[][2]={{-30,-5.3f},{14,5.4f},{-80,5.3f},{78,-5.3f},{105,4.9f},{-108,-5.0f},{116,-36},{66,-36},{-72,-36}};
static const float s205_trash[][2]={{56,-33.5f},{40,-34},{30,-39},{20,-9.5f},{-30,-9.4f},{-44,-9.6f},{10,-33},{-14,-9.6f},{62,-9.4f},{-58,-36},{-24,-36},{108,-9.5f},{-106,9.5f},{90,9.4f},{-96,-36.5f},{48,9.6f}};
static const float s205_pud[][3]={{-20,4.2f,1.7f},{32,-4.0f,1.5f},{-62,4.4f,1.8f},{72,4.0f,1.4f},{-88,-4.2f,1.9f},{4,-34.5f,1.3f},{46,-38.5f,1.6f},{-34,-9.0f,1.2f},{104,-4.4f,1.7f},{-116,4.3f,1.5f},{24,39.5f,1.6f},{-40,39.2f,1.4f}};

static void s205_ambient(void){
 float day=s205_day();
 for(int n=0;n<9;n++){float x=s205_cars[n][0],z=s205_cars[n][1];
  if(dst(camx,camz,x,z)<viewdist+6)vehicle190(x,z,n+2);}
 for(int n=0;n<16;n++){float x=s205_trash[n][0],z=s205_trash[n][1];
  if(dst(camx,camz,x,z)>viewdist+4)continue;
  int v=(int)(world_value(7000+n)*1000)%4;
  if(v==0){surfaceTex=&mat_wood;box(x,.24f,z,.7f,.48f,.55f,0x6f5438);surfaceTex=0;box(x-.2f,.7f,z+.1f,.3f,.3f,.3f,0x5b4a36);}
  else if(v==1){box(x,.18f,z,.55f,.36f,.42f,0x5c5144);box(x+.3f,.12f,z-.2f,.3f,.24f,.3f,0x4d453a);}
  else if(v==2){for(int k=0;k<3;k++)s205_post(x-.3f+k*.3f,z,.0f,.28f+k*.03f,.045f,(k%2)?0x3f5a3f:0x5a4a2c);}
  else{soft_mesh(x,.2f,z,.5f,.2f,.4f,0,0x4a4640);soft_mesh(x+.4f,.12f,z+.2f,.2f,.12f,.18f,0,0x39352f);}
 }
 for(int n=0;n<12;n++){float x=s205_pud[n][0],z=s205_pud[n][1],r=s205_pud[n][2];
  if(dst(camx,camz,x,z)>viewdist+4)continue;
  surfaceTex=0;quadstrip(vp(x-r,.035f,z-r*.5f),vp(x+r,.035f,z-r*.6f),vp(x+r*.9f,.035f,z+r*.5f),vp(x-r*.8f,.035f,z+r*.6f),0x49595a);}
 s205_wire_row(-7.9f,1);s205_wire_row(7.9f,-1);s205_wire_row(-38.6f,1);s205_wire_row(38.6f,-1);
 /* бельё во дворе и «ЦОЙ ЖИВ» на торце дома */
 for(int n=0;n<4;n++){float x=hx[n]+8.4f,z=hz[n]-2.2f;
  if(dst(camx,camz,x,z)>viewdist)continue;
  s205_post(x,z-.9f,.0f,2.3f,.03f,0x4c463c);s205_post(x,z+2.4f,.0f,2.3f,.03f,0x4c463c);
  for(int w=0;w<3;w++)beam(vp(x,2.2f,z-.9f),vp(x,2.05f,z+2.4f),.006f,.006f,0x5b564a);
  for(int c=0;c<4;c++){float cz=z-.3f+c*.65f,sw=sinf(st205.clock*1.3f+c)*.03f;
   quad(vp(x+.03f,2.16f,cz),vp(x+.03f,2.16f,cz+.4f),vp(x+.03f+sw,1.55f,cz+.4f),vp(x+.03f+sw,1.55f,cz),(c%2)?0xc9c0aa:0x8f6a5a,0,.9f);}
 }
 (void)day;
}

static void street_draw205(void){
 float day=s205_day();
 s205_ambient();
 for(int n=0;n<POI205_COUNT;n++){Poi205*p=(Poi205*)&poi205[n];
  if(dst(camx,camz,p->x,p->z)>viewdist+10)continue;
  switch(p->type){
  case S205_KIOK:s205_kiosk(p,day);break;case S205_PHON:s205_phone(p,day);break;
  case S205_WALL:s205_wall(p,day);break;case S205_DOMI:s205_domino(p,day);break;
  case S205_PUMP:s205_pump(p,day);break;case S205_BARR:s205_barrel(p,day);break;
  case S205_GOAL:s205_goal(p,day);break;case S205_STAS:s205_stash(p,day);break;
  case S205_MUSI:s205_music(p,day);break;case S205_BABA:s205_man(p->x-1.1f,p->z,PI*.5f,0x6a5548,0x3e3a33,0x9a9080,0,0),s205_man(p->x+.9f,p->z-.2f,PI*.5f,0x4d4f44,0x35382f,0x7d7462,0,0);break;}
 }
 for(int h=0;h<4;h++){float x=hx[h]+4.3f,z=hz[h]-4.95f;if(dst(camx,camz,x,z)<viewdist)s205_board(x,z,h);}
}

static void street_tick205(float dt){
 st205.clock+=dt;
 if(st205.strum>0)st205.strum-=dt;
 if(st205.ballT>0&&st205.ballT<dt)st205.ballT=0;
 if(st205.dominoDay!=g.day&&st205.dominoPlayed){st205.dominoPlayed=0;}
}
