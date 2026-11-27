/* v0.14: persistent banking, market cycle, four seasons and fall damage.
   Native C / no runtime dependencies. All prices use the same market quote. */
static const char*season_name(void){static const char*s[]={"Зима","Весна","Лето","Осень"};return s[(int)clamp(g.season,0,3)];}
static void update140_defaults(void){
 if(g.update140!=140){g.update140=140;g.bank=0;g.deaths=0;g.economy=1;g.atmFee=2;g.economyDay=g.day;g.season=g.winter?0:2;g.seasonDay=g.day;g.fallVelocity=0;g.fallStart=0;g.drowning=0;memset(g.lootRefill,0,sizeof(g.lootRefill));}
 for(int i=0;i<NN;i++)g.residentHome[i]=(int)clamp(g.residentHome[i],0,8);
 g.bank=(int)clamp(g.bank,0,10000000);g.season=(int)clamp(g.season,0,3);g.economy=clamp(g.economy,.65f,1.6f);g.atmFee=clamp(g.atmFee,1.5f,3.5f);g.fallVelocity=0;g.drowning=0;swimAir141=100;swimBlend141=0;
}
static float river_z(float x){return 86+(float)sin(x*.028f+(g.worldSeed%97)) *2.2f;}
static int bridge_at202(float x,float z){for(int n=-1;n<=1;n++){float cx=n*90.f;if(ab(x-cx)<2.9f&&ab(z-river_z(cx))<4.75f)return 1;}return 0;}
static int river_water(float x,float z){if(ab(x)>140||ab(z-river_z(x))>3.8f)return 0;return !bridge_at202(x,z);}
static int market_price(int base){return (int)(base*clamp(g.economy,.65f,1.6f)+.5f);}
static void bank_collect(void){if(g.salary<=0){say("Нет начислений. Сначала выполни заказ.");return;}int pay=(int)mn(g.salary,10000000-g.bank);if(pay<=0){say("Достигнут лимит карты. Сними часть денег.");return;}g.bank+=pay;g.salary-=pay;say("Зарплата зачислена на карту. Наличные — в банкомате у магазина.");workNoticeUntil=realTime+5;audio(1);save_game();}
static int atm_near(void){return g.playerY<.3f&&dst(g.x,g.z,ATM_X,ATM_Z)<2.5f;}
static void bank_withdraw(int amount){
 if(!atm_near()){say("Снять деньги можно только у банкомата.");return;}
 if(g.bank<=0){say("На карте нет денег. Получи зарплату на карту в офисе.");return;}
 if(amount<=0||amount>g.bank)amount=g.bank;
 int fee=(int)(amount*g.atmFee/100+.5f),net=amount-fee;
 if(net<=0||g.money>10000000-net){say("Эту сумму снять нельзя.");return;}
 g.bank-=amount;g.money+=net;char b[220];SN(b,sizeof(b),"Списано %d / наличными %d / комиссия %d руб. (%.1f%%).",amount,net,fee,g.atmFee);say(b);workNoticeUntil=realTime+5;audio(1);save_game();
}
static int roof_at(float x,float z,float*height){if(garage_roof203(x,z,height))return 1;
 for(int h=0;h<NH;h++){float a=x-hx[h],b=z-hz[h];if(g.playerY>17.80f&&a>4.78f&&a<8.42f&&b>-4.52f&&b<3.52f){*height=18.04f;return 1;}
 if((ab(a)<5.07f&&ab(b)<4.67f)||(a>4.83f&&a<8.37f&&b>-8.42f&&b< -1.98f)){*height=15.16f;return 1;}}
 for(int p=0;p<PANEL_COUNT;p++){float a=x-panel_origin_x(p),b=z-panel_origin_z(p);if(g.playerY>14.70f&&b>.48f&&b<8.02f&&(ab(a-7)<1.82f||ab(a+7)<1.82f)){*height=14.94f;return 1;}
 if(ab(a)<14.02f&&ab(b)<8.12f){*height=12.12f;return 1;}}
 for(int n=0;n<4;n++){float a=x-cityBlockX[n],b=z-cityBlockZ[n],base=high_floors180(n)*3;if(g.playerY>base+1.85f&&ab(a)<1.12f&&b>-6.87f&&b< -4.53f){*height=base+2.10f;return 1;}
 if((ab(a)<8.02f&&ab(b)<5.32f)||(ab(a)<1.87f&&b>-7.725f&&b< -5.135f)){*height=base+.12f;return 1;}}
 return 0;
}
static void player_die(void){if(screen==24)return;g.health=0;g.deaths++;int lost=g.money/10;g.money-=lost;g.fallVelocity=0;work_cancel();bonusActive=microActive=0;if(rideIndex202>=0)g.zoneFlags150&=~(1u<<(16+rideIndex202-53));reset202();seated=-1;screen=24;event_log("player_died",g.deaths);save_game();}
static void update140_tick(float dt){
 traffic_tick140(dt);high_tick180(dt);
 /* Market follows a deterministic daily cycle with seeded positive/negative shocks. */
 int d=g.day;if(g.economyDay!=d){g.economyDay=d;float quote=1+.21f*(float)sin((d-1)*.63f)+(world_value(6000+d)-.5f)*.18f;g.economy=clamp(quote,.65f,1.6f);g.atmFee=clamp(2+(g.economy-1)*3,1.5f,3.5f);}
 if(g.day-g.seasonDay>=7){int steps=(g.day-g.seasonDay)/7;g.season=(g.season+steps)%4;g.seasonDay+=steps*7;navReady=0;}
 g.winter=g.season==0;winterEnabled=g.winter;seasonVisual140=g.season;
 for(int n=0;n<30;n++)if(g.lootMask&(1u<<n)){if(g.lootRefill[n]<=0)g.lootRefill[n]=g.absolute+4+world_value(8000+n)*6;if(g.absolute>=g.lootRefill[n]){g.lootMask&=~(1u<<n);g.lootRefill[n]=0;}}
 /* Residence lifts occupy the old service wing; never intersect the apartment. */
 for(int h=0;h<NH;h++){ResidenceSave*s=&g.residence[h];int rider=ab(g.x-(hx[h]+6.1f))<.83f&&ab(g.z-(hz[h]-6.0f))<.83f&&ab(g.playerY-s->y)<.3f;if(s->wait>0)s->wait=mx(0,s->wait-dt);else{s->y+=clamp(s->target-s->y,-dt*1.7f,dt*1.7f);if(rider)g.playerY=s->y;}}
 if(g.fallVelocity<0){g.fallVelocity-=9.81f*dt;g.playerY+=g.fallVelocity*dt;float ground=terrain_height141(g.x,g.z),rh;if(roof_at(g.x,g.z,&rh)&&rh<g.fallStart-.1f)ground=rh;
 if(g.playerY<=ground){g.playerY=ground;float drop=g.fallStart-ground;g.fallVelocity=0;if(drop>2.8f)g.health-=mx(0,(drop-2.8f)*17);if(g.health<=0){player_die();return;}if(drop>3){say("Падение. Ты получил травму.");workNoticeUntil=realTime+4;}}}
 else if(g.playerY>2){float rh;int under=roof_at(g.x,g.z,&rh);if(!under&&panel_wing(g.x,g.z)<0){g.fallStart=g.playerY;g.fallVelocity=-.01f;}}
 water_tick141(dt);
}
static int update140_use(int perform){
 if(atm_near()&&look_at(ATM_X,1.3f,ATM_Z,.80f)){if(perform)screen=23;else SN(prompt,sizeof(prompt),"E / ЛКМ — банкомат / комиссия %.1f%%",g.atmFee);return 1;}
 for(int p=0;p<PANEL_COUNT;p++)for(int w=0;w<2;w++){float x=panel_origin_x(p)+(w?7:-7),z=panel_origin_z(p);float a=g.x-x,b=g.z-z;
 if(a> -5.1f&&a< -1.65f&&b> -7.4f&&b< -3.4f){if(perform){liftUI=p*2+w;screen=14;}else SN(prompt,sizeof(prompt),"E / ЛКМ — лифт / выбрать этаж. На крышу — по лестнице");return 1;}
 }
 for(int h=0;h<NH;h++){float a=g.x-hx[h],b=g.z-hz[h];if(a>5.0f&&a<8.1f&&b> -7.3f&&b< -4.9f){if(perform){liftUI=30+h;screen=14;}else SN(prompt,sizeof(prompt),"E — лифт / этажи. На крышу — по лестнице сверху.");return 1;}}
 return 0;
}
static void decor_build140(void){
 float py=g.playerY;g.playerY=0;
 for(int n=0;n<38;n++)for(int attempt=0;attempt<24;attempt++){
 float x=-132+world_value(10000+n*37+attempt*997)*264,z=-94+world_value(11000+n*41+attempt*991)*188;
 if(ab(z)<12||ab(ab(z)-43)<8||ab(ab(x)-94)<8||ab(z-river_z(x))<7)continue;
 if(building_solid(x,z)||building_solid(x-2,z-2)||building_solid(x+2,z+2)||building_solid(x-2,z+2)||building_solid(x+2,z-2))continue;
 if(!outside141(x,z,2.4f))continue;int nearLoot=0;for(int q=0;q<LOOT_COUNT;q++){float lx,lz;loot_position(q,&lx,&lz);if(dst(x,z,lx+.55f,lz)<3.8f)nearLoot=1;}if(nearLoot)continue;
 int nearBench=0;for(int q=0;q<54;q++){float bx,bz;bench_position141(q,&bx,&bz);if(dst(x,z,bx,bz)<3)nearBench=1;}if(nearBench)continue;
 if(ab(x-shopX)<10&&ab(z-shopZ)<10)continue;
 int blocked=0;for(int k=0;k<envCount;k++)if(dst(x,z,env[k].x,env[k].z)<3.5f)blocked=1;
 if(blocked)continue;env_add(x,z,.25f,.25f,4,n);break;
 }
 for(int n=0;n<54;n++){float x,z;bench_position141(n,&x,&z);env_add(x,z,1.3f,.45f,6,n);}
 for(int n=0;n<LOOT_COUNT;n++){float x,z;loot_position(n,&x,&z);env_add(x+.55f,z,.65f,.55f,5,n);}
 g.playerY=py;
}
static int traffic_heading201(int n){return (n&1)?-1:1;}
static float traffic_speed201(int n){return (4.2f+(n%3)*1.15f)*traffic_heading201(n);}
static int traffic_ped_clear201(float x,float z,float y,float radius){if(y>=2.2f)return 1;traffic_init140();for(int n=0;n<7;n++){float xx=carX140[n],future=xx+traffic_speed201(n)*.40f;float lo=mn(xx,future)-2.30f-radius,hi=mx(xx,future)+2.30f+radius;if(x>lo&&x<hi&&ab(z-carZ140[n])<1.13f+radius)return 0;}return 1;}
static void traffic_init140(void){if(trafficSeed140==g.worldSeed)return;trafficSeed140=g.worldSeed;for(int n=0;n<7;n++){carX140[n]=-114+n*33;carZ140[n]=(n&1)?2.75f:-2.75f;}}
static int traffic_clear140(float x,float z){float py=g.playerY;g.playerY=0;int ok=1;for(int a=-1;a<=1;a++)for(int b=-1;b<=1;b++)if(building_solid(x+a*2.4f,z+b*1.12f)||env_solid(x+a*2.4f,z+b*1.12f))ok=0;g.playerY=py;return ok;}
static int traffic_solid140(float x,float z){traffic_init140();for(int n=0;n<7;n++)if(ab(x-carX140[n])<2.52f&&ab(z-carZ140[n])<1.24f)return 1;return 0;}
static void traffic_step201(float dt){traffic_init140();for(int n=0;n<7;n++){float speed=traffic_speed201(n),nx=carX140[n]+speed*dt,z=carZ140[n];if(nx>128)nx=-128;if(nx< -128)nx=128;
 int blocked=!traffic_clear140(nx,z);float front=nx+(speed>0?3.6f:-3.6f);
 if(g.playerY<1.8f&&ab(g.z-z)<1.6f&&ab(g.x-front)<3)blocked=1;
 for(int i=0;i<NN;i++)if(ab(PERSON(i).z-z)<1.6f&&ab(PERSON(i).x-front)<2)blocked=1;
 for(int i=0;i<LOCAL190;i++)if(g.revision190&&g.locals190[i].y<1.8f&&ab(g.locals190[i].z-z)<1.65f&&ab(g.locals190[i].x-front)<2.3f)blocked=1;
 for(int k=0;k<7;k++)if(k!=n&&ab(carZ140[k]-z)<1&&ab(carX140[k]-front)<3)blocked=1;
 for(int i=0;i<NN;i++)if(ab(PERSON(i).z-z)<1.42f&&ab(PERSON(i).x-nx)<2.60f)blocked=1;
 for(int i=0;i<LOCAL190;i++)if(g.revision190&&g.locals190[i].y<2.1f&&ab(g.locals190[i].z-z)<1.40f&&ab(g.locals190[i].x-nx)<2.58f)blocked=1;
 if(!blocked)carX140[n]=nx;
}}
static void traffic_tick140(float dt){int steps=(int)clamp((float)floor(dt/.025f)+1,1,64);for(int i=0;i<steps;i++)traffic_step201(dt/steps);}
/* Local doorway routing is finer than the 2m outdoor nav grid. No resident teleport. */
static int home_route140(int i,float dt,int home){Citizen*c=&PERSON(i);int h=c->home,side=i/NH;float x=hx[h],z=hz[h],fx=side?2.4f:-2.4f;int stage=g.residentHome[i];if(!stage&&home){for(int j=0;j<NN;j++)if(j!=i&&PERSON(j).home==h){int other=g.residentHome[j];/* Housemates share one narrow entrance. The lower resident ID enters first; the other waits outside instead of both converging on the same doorway. */if((other>0&&other<8)||(j<i&&other==0&&!life_at_home(j))){c->activity=0;npcMoving[i]=0;npcGait[i]=0;return 1;}}}if(stage>=8&&!home){for(int j=0;j<NN;j++)if(j!=i&&PERSON(j).home==h&&g.residentHome[j]>0&&g.residentHome[j]<8){c->activity=4;npcMoving[i]=0;npcGait[i]=0;return 1;}}
 if(!stage){if(!home)return 0;if(life_at_home(i)&&ab(c->x-x-fx)<1.3f&&c->z>z-1.7f){g.residentHome[i]=8;stage=8;}else if(dst(c->x,c->z,x+7.55f,z-9.5f)<3.2f){stage=(ab(c->x-x-7.55f)<.55f&&c->z>=z-9.55f)?2:1;g.residentHome[i]=stage;}else return 0;}
 if(home&&stage>=8){c->activity=schedule150(i)==13?13:schedule150(i)==14?14:0;npcMoving[i]=0;npcGait[i]=0;return 1;}if(!home&&stage>=8){stage=7;g.residentHome[i]=7;}
 float px[]={7.55f,7.55f,5.6f,fx,fx,fx,fx},pz[]={-9.5f,-3,-3,-3,-1.9f,-.6f,.4f};int k=(int)clamp(stage-1,0,6);g.residence[h].doors|=1;g.flatDoors150[h]|=1u<<side;g.things150[unit150(h,0,side)]|=1u<<17;float tx=x+px[k],tz=z+pz[k],d=dst(c->x,c->z,tx,tz);c->activity=home?0:4;if(d<.10f){if(home)g.residentHome[i]=stage<7?stage+1:8;else if(stage>1)g.residentHome[i]=stage-1;else{g.residentHome[i]=0;citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;}return 1;}
 float st=mn(d,dt*1.25f),dx=(tx-c->x)*st/d,dz=(tz-c->z)*st/d;if(nav_clear(c->x+dx,c->z+dz)){c->x+=dx;c->z+=dz; npcMoving[i]=1;walkPhase[i]+=st*5;npcGait[i]=1;c->facing=(float)atan2(dx,dz);}return 1;}
