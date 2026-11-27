/* A single primitive catalog for persistent district infrastructure. Types: snowman, tire, post, sandbox, garage, car, bench, bus stop. */
typedef struct{float x,z,rx,rz,height;int type;} Prop150;
static Prop150 props150[96];static int propCount150=0;
static float playStart151[6],exerciseAt151=-100;
static int play_on151(int n){return !!(g.zoneFlags150&(1u<<(16+n-53)));}
static float play_motion151(int n){return play_on151(n)?realTime-playStart151[n-53]:0;}
static void prop_add150(float x,float z,float rx,float rz,float height,int type){if(propCount150>=96)return;props150[propCount150++]=(Prop150){x,z,rx,rz,height,type};}
static void props_init150(void){propCount150=0;prop_add150(-6,7.7f,.53f,.50f,2.2f,0);prop_add150(24,-9.6f,.53f,.50f,2.2f,0);
 for(int n=0;n<6;n++)prop_add150(-37+n*5,52,.58f,.58f,.4f,1);
 for(int n=0;n<8;n++)prop_add150(-31+(n%4)*3,-70+(n/4)*3,.07f,.07f,2.4f,2);
 prop_add150(-21,62,2,1.7f,.3f,3);
 for(int n=0;n<16;n++)prop_add150(n<8?-167:167,(float[]){-78,-65,-25,-12,12,25,65,78}[n%8],3.5f,3.0f,2.7f,4);
 for(int n=0;n<8;n++)prop_add150(-38+(n%4)*7,n<4?-51:51,2.32f,1.14f,2.10f,5);
 for(int n=0;n<4;n++)prop_add150(-18+n*12,10,1.4f,.48f,1.22f,6);
 for(int n=0;n<8;n++)prop_add150(-50+n*14.5f,n%2?-6.4f:6.4f,.09f,.09f,5.2f,7);
 for(int c=0;c<2;c++){float z=c?13:-13,dx=c?33:0;prop_add150(-22+dx,z,2,1.8f,2.5f,8);prop_add150(-12+dx,z,1.3f,1.3f,1.4f,9);prop_add150(-3+dx,z,2.1f,.6f,1.1f,10);}
 for(int side=-1;side<=1;side+=2)for(int j=0;j<4;j++)prop_add150(side*178,(float[]){-48,-35,35,48}[j],3.5f,3,2.85f,11);
}
static int props_solid150(float x,float z){if(!propCount150)props_init150();for(int n=0;n<propCount150;n++){Prop150*p=&props150[n];if(n==ignoreProp204)continue;if(g.playerY>=p->height||((p->type==4||p->type==11)&&g.playerY>=2.6f)||p->type==0&&!g.winter)continue;float a=ab(x-p->x),b=ab(z-p->z);if(p->type==3){if((a>p->rx-.15f&&a<p->rx&&b<p->rz)||(b>p->rz-.15f&&b<p->rz&&a<p->rx))return 1;}else if(p->type==4){if(a<p->rx&&b<p->rz){if(a>p->rx-.16f||b>p->rz-.16f||!(g.zoneFlags150&(1u<<(n-17))))return 1;}}else if(a<p->rx&&b<p->rz)return 1;}
 if(g.playerY<2.4f&&((ab(x+72.2f)<.08f&&ab(z-12)<.08f)||(ab(x+65.8f)<.08f&&ab(z-12)<.08f)))return 1;
 return 0;}
static int prop_use150(int perform){
 if(!propCount150)props_init150();
 for(int n=0;n<propCount150;n++){
 Prop150*p=&props150[n];if(p->type==5)continue;float range=mx(p->type>=8?3.2f:2.4f,p->rx+1.6f);
 if(g.playerY>1.8f||dst(g.x,g.z,p->x,p->z)>range||!look_at(p->x,mn(1.2f,p->height*.6f),p->z,.94f))continue;
 if(p->type==0&&!g.winter)continue;
 if(p->type==11){if(!perform){SN(prompt,sizeof(prompt),"Гараж ГСК / заперто владельцем");return 1;}say("Чужой гараж заперт навесным замком.");return 1;}
 const char*name=p->type==0?"снеговик / поправить ведро":p->type==1?"клумба из покрышки / осмотреть":p->type==2?"турник / подтянуться":p->type==3?"песочница / разгрести снег":p->type==4?"гараж / открыть или закрыть ворота":p->type==5?"машина / проверить дверь":p->type==6?"скамейка / присесть":p->type==7?"остановка / расписание":p->type==8?"качели / сесть":p->type==9?"карусель / сесть":"балансир / качнуть или остановить";
 if(!perform){SN(prompt,sizeof(prompt),"E — %s",name);return 1;}
 if(p->type==8||p->type==9){ride_start202(n);return 1;}
 if(p->type>=8){int bit=16+n-53;g.zoneFlags150^=1u<<bit;playStart151[n-53]=realTime;audio(0);say(play_on151(n)?"Старый металл скрипит. Конструкция пришла в движение.":"Ты придержал конструкцию. Движение остановилось.");}
 else if(p->type==2){if(realTime-exerciseAt151<8){say("Сначала передохни.");return 1;}if(g.stamina<15){say("Не хватает сил. Нужно передохнуть.");return 1;}exerciseAt151=realTime;g.stamina=mx(0,g.stamina-12);g.energy=mx(0,g.energy-3);g.stress=mx(0,g.stress-6);say("Несколько подтягиваний. Холодный металл обжигает ладони.");}
 else if(p->type==4){g.zoneFlags150^=1u<<(n-17);nav_refresh(p->x,p->z);audio(12);say("Заскрипели тяжёлые гаражные ворота.");}
 else if(p->type==6){seated=2000+n;g.x=p->x;g.z=p->z-.75f;g.rot=yaw=PI;g.look=pitch=0;say("Сидишь на холодной скамейке. E или движение — встать.");}
 else if(p->type==7)say("Автобус 17: 06:10–23:20. Интервал 20 минут. По выходным — 35.");
 else if(p->type==5){audio(12);say("Дверь заперта. Под дворником квитанция, крыша в снегу.");}
 else say(p->type==0?"Ведро выпрямлено. Снег уже начал слёживаться.":p->type==3?"Под снегом мокрый песок. Забытая формочка лежит у борта.":"Старая покрышка. Внутри снег и сухие прошлогодние стебли.");return 1;
 }return 0;
}
