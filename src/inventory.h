/* v0.6 hotfix: legacy counters remain authoritative; one movable stack/type.
   No dynamic allocations, no item duplication during UI redraw or drag. */
static const char*invNames[]={"Пусто","Бутерброд","Вода 0,5 л","Корм","Аптечка","Сигареты","Пиво","Стеклотара","Арматура / лом","Трава"};
static const char*invCodes[]={"","ЕДА","ВОДА","КОРМ","МЕД","ТАБАК","ПИВО","ТАРА","МЕТ","ТРАВА"};
static const float invWeights[]={0,.18f,.55f,.3f,.22f,.012f,.55f,.25f,.65f,.01f};
static const float invVolumes[]={0,.35f,.6f,.4f,.3f,.02f,.6f,.6f,.25f,.02f};
static const int invPrices[]={0,12,7,15,32,4,25,10,18,0};
static int invSelected=0,invDrag=-1,invDropConfirm=0;
static void inv_reset_ui(void){invSelected=0;invDrag=-1;invDropConfirm=0;}
static int*inv_counter(int id){switch(id){case 1:return &g.food;case 2:return &g.water;case 3:return &g.feed;case 4:return &g.medicine;case 5:return &g.cigarettes;case 6:return &g.beer;case 7:return &g.bottles;case 8:return &g.scrap;case 9:return &g.herb;default:return 0;}}
static int inv_count(int id){int*p=inv_counter(id);return p?*p:0;}
static float inv_weight(void){float v=0;for(int id=1;id<=9;id++)v+=inv_count(id)*invWeights[id];return v;}
static float inv_volume(void){float v=0;for(int id=1;id<=9;id++)v+=inv_count(id)*invVolumes[id];return v;}
static void inv_sync(void){
 if(!g.invReady){memset(g.invSlots,0,sizeof(g.invSlots));g.quick[0]=g.quick[1]=0;g.invReady=1;invSelected=0;invDrag=-1;invDropConfirm=0;}
 unsigned int seen=0;for(int n=0;n<48;n++){int id=g.invSlots[n];if(id<1||id>9||inv_count(id)<=0||(seen&(1u<<id)))g.invSlots[n]=0;else seen|=1u<<id;}
 for(int id=1;id<=9;id++){int*p=inv_counter(id);*p=(int)clamp(*p,0,999);if(*p>0&&!(seen&(1u<<id)))for(int n=0;n<48;n++)if(!g.invSlots[n]){g.invSlots[n]=id;break;}}
 for(int k=0;k<2;k++)if(g.quick[k]<0||(g.quick[k]>6&&g.quick[k]!=9)||inv_count(g.quick[k])<=0)g.quick[k]=0;
}
static int inv_add(int id,int amount){int*p=inv_counter(id);if(!p||amount<=0||*p+amount>999)return 0;if(inv_weight()+invWeights[id]*amount>25||inv_volume()+invVolumes[id]*amount>40){say("Рюкзак полон: максимум 25 кг / 40 л.");return 0;}*p+=amount;inv_sync();return 1;}
static int inv_move(int from,int to){if(from<0||from>=48||to<0||to>=48||!g.invSlots[from])return 0;int id=g.invSlots[from];g.invSlots[from]=g.invSlots[to];g.invSlots[to]=id;invSelected=to;return 1;}
static int inv_trade_near(void){return recycle_near();}
static void inv_sell(int id,int all){if(id!=7&&id!=8){say("Принимается только пустая тара и металлолом. Расходники не выкупаются.");return;}int*p=inv_counter(id);if(!inv_trade_near()){say("Подойди к отдельному пункту приёма у офиса.");return;}if(!p||*p<=0)return;int count=all?*p:1;*p-=count;g.money+=count*invPrices[id];eventValue=(float)(count*invPrices[id]);event_log("item_sold",id);inv_sync();audio(1);save_game();say("Сырьё сдано. Деньги получены наличными.");}
static void inv_use(int id){if(id==9){consume_begin(6);return;}if(id==7||id==8){say("Вторсырьё принимает площадка у офиса: 10 / 18 руб.");return;}int key=id==1?'1':id==2?'2':id==3?'F':id==4?'3':id==5?'4':id==6?'5':0;if(!key||!inv_count(id))return;int was=pressed[key];pressed[key]=1;use_items();pressed[key]=was;inv_sync();}
static void inv_discard(int id){discard200(id);}


static float bonus_duration(void){return workPending>=0?(1.55f-workHits*.10f):bonusId>=1600?1.4f:.8f;}
static float bonus_center(void){return workPending>=0?workCenter:bonusId>=1600?.43f+world_value(bonusId)*.32f:.56f;}
static float bonus_width(void){return workPending>=0?clamp(.17f+.04f*skill_effect(skillLevel)+(skillAssist?.03f:0),.17f,.25f):clamp(.28f+.12f*skill_effect(skillLevel),.28f,.50f);}
static int bonusCrits=0,bonusChecks=0;static float bonusTotal=0;
static void bonus_start(int id){bonusActive=1;bonusId=id;bonusClock=0;event_log("skill_check_started",id);}
static void bonus_tick(float dt){if(workPending>=0){work_check_tick(dt);return;}if(!bonusActive)return;if(bonusId>=1600){float x,z;loot_position(bonusId-1600,&x,&z);if(g.playerY>.3f||dst(g.x,g.z,x+.55f,z)>3){bonusActive=0;say("Поиск прерван: контейнер далеко.");return;}}bonusClock+=dt;float phase=bonusClock/bonus_duration();int answered=bonusClock>.05f&&(pressed[32]||(autoChecks&&ab(phase-bonus_center())<bonus_width()*.25f));if(answered||phase>1){int success=answered&&ab(phase-bonus_center())<=bonus_width()*.5f;int critical=success&&(bonusId>=1600||skillLevel>100)&&ab(phase-bonus_center())<.045f+.025f*skill_effect(skillLevel);bonusChecks++;bonusTotal+=mn(bonusClock,bonus_duration());bonusCrits+=critical;eventValue=mn(bonusClock,bonus_duration());event_log("skill_check_time",bonusId);eventValue=critical?2:success?1:0;event_log("skill_check_result",bonusId);if(bonusId>=1600){int n=bonusId-1600;if(n>=0&&n<LOOT_COUNT&&!(g.lootMask&(1u<<n))){if(success){if(inv_add(n%2?8:7,critical?2:1)){g.lootMask|=1u<<n;g.lootRefill[n]=g.absolute+4+world_value(8000+n)*6;say(critical?"Отличный поиск: две находки.":"Найдено вторсырьё. Сдай на площадке у офиса.");event_log("resource_collected",n);}}else{g.lootMask|=1u<<n;g.lootRefill[n]=g.absolute+4+world_value(8000+n)*6;g.stress=mn(100,g.stress+3);say("Шумно. Полезного не нашлось до нового наполнения.");}audio(success?5:6);}bonusActive=0;return;}if(success){if(g.districtBonusDay!=g.day){memset(g.districtBonus,0,sizeof(g.districtBonus));g.districtBonusDay=g.day;}int word=bonusId/32,bit=bonusId%32;if(word>=0&&word<50&&!(g.districtBonus[word]&(1u<<bit))){g.districtBonus[word]|=1u<<bit;g.money+=critical?15:5;skillLevel+=critical?.6f:.2f;}audio(critical?1:5);}bonusActive=0;}}
