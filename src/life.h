/* Persistent needs, schedules, witness responses and shared walkable navigation. */
#define NAVX 192
#define NAVZ 144
#define NAVN (NAVX*NAVZ)
#define MAXPATH 512
static unsigned char navWalk[NAVN],navLinks[NAVN];static short navParent[NAVN],navQueue[NAVN];
static short citizenPath[NN][MAXPATH];static int citizenLen[NN],citizenStep[NN],lastActivity[NN];
static float chatUntil[NN];
static float lifeThink[NN],walkPhase[NN],lifeClock=0;
static int npcDebugEnabled=0,npcPlanCursor=0,npcPathRequests=0,npcPathErrors=0;static const char*npcLogPath="OKNO_npc_debug.csv";static float npcLastRequest[NN];
static void npc_path_log(int i){if(!npcDebugEnabled)return;char row[260];Citizen*c=&PERSON(i);int n=SN(row,sizeof(row),"%d,%.3f,%d,%.3f,%.3f,%.3f,%d,%d,0\n",i,lifeClock,c->activity,c->tx,c->tz,npcLastRequest[i],citizenLen[i],citizenLen[i]?0:1);FILE*f=fopen(npcLogPath,"ab");if(f){if(n>0&&n<(int)sizeof(row))fwrite(row,1,n,f);fclose(f);}}
static float lifeAccumulator[NN];static unsigned int lifeUpdates[NN];
static const char*activity_name(int a){const char*labels[]={"дома / отдых","покупает еду","на работе","смена в магазине","отдыхает во дворе","сообщает о краже","патрулирует","идёт в гости","разговаривает во дворе","курит","отдыхает с напитком","играет во дворе"};return labels[a<0||a>11?0:a];}
static float nav_x(int n){return -191.f+(n%NAVX)*2;}
static float nav_z(int n){return -143.f+(n/NAVX)*2;}
static int nav_clear(float x,float z){if(!navStatic150&&physicsActor150>=0&&!npc_clear204(x,z,0,physicsActor150))return 0;if(!navStatic150&&!traffic_ped_clear201(x,z,0,.25f))return 0;if(!g.winter&&river_water(x,z))return 0;return body_clear150(x,z,0,.25f,0);}
static int nav_edge(int a,int b){float ax=nav_x(a),az=nav_z(a),bx=nav_x(b),bz=nav_z(b);return nav_clear((ax+bx)*.5f,(az+bz)*.5f)&&nav_clear(lerp(ax,bx,.25f),lerp(az,bz,.25f))&&nav_clear(lerp(ax,bx,.75f),lerp(az,bz,.75f));}
static void nav_build(void){navStatic150=1;for(int n=0;n<NAVN;n++){navWalk[n]=nav_clear(nav_x(n),nav_z(n));navLinks[n]=0;}
 for(int n=0;n<NAVN;n++)if(navWalk[n]){if(n%NAVX<NAVX-1&&navWalk[n+1]&&nav_edge(n,n+1)){navLinks[n]|=2;navLinks[n+1]|=1;}if(n+NAVX<NAVN&&navWalk[n+NAVX]&&nav_edge(n,n+NAVX)){navLinks[n]|=8;navLinks[n+NAVX]|=4;}}navReady=1;navStatic150=0;}
/* Door edits touch only neighbouring navigation cells, never rebuild the whole district. */
static void nav_refresh(float x,float z){if(!navReady)return;navStatic150=1;int cx=(int)clamp((x+191)/2+.5f,0,NAVX-1),cz=(int)clamp((z+143)/2+.5f,0,NAVZ-1);for(int dz=-3;dz<=3;dz++)for(int dx=-3;dx<=3;dx++){int xx=cx+dx,zz=cz+dz;if(xx>=0&&xx<NAVX&&zz>=0&&zz<NAVZ){int n=zz*NAVX+xx;navWalk[n]=nav_clear(nav_x(n),nav_z(n));}}for(int dz=-4;dz<=4;dz++)for(int dx=-4;dx<=4;dx++){int xx=cx+dx,zz=cz+dz;if(xx<0||xx>=NAVX||zz<0||zz>=NAVZ)continue;int n=zz*NAVX+xx;navLinks[n]=0;if(!navWalk[n])continue;int next[4]={n-1,n+1,n-NAVX,n+NAVX};for(int k=0;k<4;k++){int m=next[k];if(m<0||m>=NAVN||(k==0&&xx==0)||(k==1&&xx==NAVX-1))continue;if(navWalk[m]&&nav_edge(n,m))navLinks[n]|=1<<k;}}navStatic150=0;}
static int nav_nearest(float x,float z){int xx=(int)clamp((x+191)/2+.5f,0,NAVX-1),zz=(int)clamp((z+143)/2+.5f,0,NAVZ-1),center=zz*NAVX+xx;if(navWalk[center])return center;
 for(int radius=1;radius<=5;radius++)for(int dz=-radius;dz<=radius;dz++)for(int dx=-radius;dx<=radius;dx++){int a=xx+dx,b=zz+dz;if(a>=0&&a<NAVX&&b>=0&&b<NAVZ&&navWalk[b*NAVX+a])return b*NAVX+a;}return -1;}
static int nav_plan(float x,float z,float gx,float gz,short*out){if(!navReady)nav_build();int start=nav_nearest(x,z),goal=nav_nearest(gx,gz);if(start<0||goal<0)return 0;
 for(int n=0;n<NAVN;n++)navParent[n]=-2;int head=0,tail=0;navQueue[tail++]=start;navParent[start]=-1;
 while(head<tail&&navParent[goal]==-2){int n=navQueue[head++],a[4]={n-1,n+1,n-NAVX,n+NAVX};for(int k=0;k<4;k++){int m=a[k];if(!(navLinks[n]&(1<<k))||navParent[m]!=-2)continue;navParent[m]=n;navQueue[tail++]=m;}}
 if(navParent[goal]==-2)return 0;short reverse[MAXPATH];int len=0;for(int n=goal;n!=-1&&len<MAXPATH;n=navParent[n])reverse[len++]=n;
 if(len==MAXPATH&&navParent[reverse[len-1]]!=-1)return 0;for(int n=0;n<len;n++)out[n]=reverse[len-n-1];return len;}
static float npcGait[NN],npcBlocked[NN],npcIdle[NN],npcPrevX[NN],npcPrevZ[NN];static int npcMoving[NN],npcReplans[NN];
static int npc_elder141(int n){return n>=0&&n<NN&&n%3==1&&(n/3)%3==0;}
static int npc_child(int n){return n>=0&&n<NN&&n%12==0;}
static int npc_woman(int n){return n>=0&&n<NN&&!npc_child(n)&&n%3==1;}
static void life_init(int fresh){navReady=0;g.worldRevision=6;lifeClock=g.absolute*60.f;npcPlanCursor=0;npcPathRequests=npcPathErrors=0;nav_build();
 for(int i=0;i<NN;i++){Citizen*c=&PERSON(i);if(fresh){g.lifeHunger[i]=70;g.lifeEnergy[i]=85;g.lifeWallet[i]=80+(i%9)*17;}
 if(c->home<0||c->home>=NH)c->home=i%NH;
 if(!nav_clear(c->x,c->z)){int node=nav_nearest(c->x,c->z);if(node>=0){c->x=nav_x(node);c->z=nav_z(node);}}
 g.lifeHunger[i]=clamp(g.lifeHunger[i],0,100);g.lifeEnergy[i]=clamp(g.lifeEnergy[i],0,100);g.lifeWallet[i]=clamp(g.lifeWallet[i],0,100000);c->mood=clamp(c->mood,0,100);
 npcPrevX[i]=c->x;npcPrevZ[i]=c->z;c->tx=c->x;c->tz=c->z;c->activity=4;c->wait=0;citizenLen[i]=citizenStep[i]=0;lastActivity[i]=-1;lifeThink[i]=i*.03f;lifeAccumulator[i]=0;lifeUpdates[i]=0;npcGait[i]=npcBlocked[i]=npcIdle[i]=0;npcMoving[i]=npcReplans[i]=0;walkPhase[i]=0;npcBench141[i]=-1;sitting141[i]=0;}
 spawn_repair204();
}
static void crowd_separate(void){int old=physicsActor150;for(int pass=0;pass<2;pass++)for(int i=0;i<NN;i++)for(int j=i+1;j<NN;j++){Citizen*a=&PERSON(i),*b=&PERSON(j);float d=dst(a->x,a->z,b->x,b->z),radius=.66f;if(d>=radius)continue;float dx=d>.001f?(a->x-b->x)/d:(i%2?1:-1),dz=d>.001f?(a->z-b->z)/d:0,push=mn((radius-d)*.51f,.045f);if(a->home==b->home&&a->z<hz[a->home]-7.7f&&b->z<hz[b->home]-7.7f){dx=0;dz=i<j?1:-1;}physicsActor150=i;if(g.residentHome[i]!=8&&sitting141[i]<.5f&&nav_clear(a->x+dx*push,a->z+dz*push)){a->x+=dx*push;a->z+=dz*push;}physicsActor150=j;if(g.residentHome[j]!=8&&sitting141[j]<.5f&&nav_clear(b->x-dx*push,b->z-dz*push)){b->x-=dx*push;b->z-=dz*push;}}physicsActor150=old;}
static int life_at_home(int i){Citizen*c=&PERSON(i);return ab(c->x-hx[c->home])<4.5f&&ab(c->z-hz[c->home])<4.0f;}
static int life_buy_meal(int i){if(i<0||i>=NN||npc_child(i))return 0;Citizen*c=&PERSON(i);if(c->activity!=1||dst(c->x,c->z,shopX,shopZ-7)>2.2f||g.lifeHunger[i]>=55||g.lifeWallet[i]<20)return 0;g.lifeWallet[i]-=20;g.lifeHunger[i]=mn(100,g.lifeHunger[i]+38);c->mood=mn(100,c->mood+3);return 1;}
static void life_local_needs(int i,float dt){Citizen*c=&PERSON(i);int home=life_at_home(i);
 if(c->activity==0&&home){g.lifeEnergy[i]=mn(100,g.lifeEnergy[i]+dt*.22f);/* Inexpensive food at home still costs money; children are supported at home. */if(g.lifeHunger[i]<35&&(npc_child(i)||g.lifeWallet[i]>=10)){if(!npc_child(i))g.lifeWallet[i]-=10;g.lifeHunger[i]=mn(100,g.lifeHunger[i]+30);}}
 if(home&&c->activity==13){g.lifeHunger[i]=mn(100,g.lifeHunger[i]+dt*.45f);g.lifeEnergy[i]=mn(100,g.lifeEnergy[i]+dt*.08f);}if(home&&c->activity==14)c->mood=mn(100,c->mood+dt*.03f);life_buy_meal(i);
 if(!npc_child(i)&&c->activity==2&&dst(c->x,c->z,OX-10+(i%4)*3,OZ-7)<2.4f){g.lifeWallet[i]=mn(100000,g.lifeWallet[i]+dt*.15f);g.lifeEnergy[i]=mx(0,g.lifeEnergy[i]-dt*.012f);}
 c->mood=clamp(c->mood+dt*(g.lifeHunger[i]<20||g.lifeEnergy[i]<15?-.035f:.003f),0,100);
}
static void life_tick(float frameDt){lifeClock+=frameDt;int plans=0;
 for(int order=0;order<NN;order++){int i=(order+npcPlanCursor)%NN;Citizen*c=&PERSON(i);physicsActor150=i;float distance=dst(g.x,g.z,c->x,c->z),interval=distance<60?0:.2f;lifeAccumulator[i]+=frameDt;lifeThink[i]-=frameDt;float dt=lifeAccumulator[i];if(dt+.00001f<interval)continue;lifeAccumulator[i]=0;dt=mn(dt,.25f);lifeUpdates[i]++;npcPrevX[i]=c->x;npcPrevZ[i]=c->z;npcMoving[i]=0;
 if(bench_release150(i,dt))continue;
 if(chatUntil[i]>lifeClock){float goal=(float)atan2(g.x-c->x,g.z-c->z);c->facing+=(float)atan2(sin(goal-c->facing),cos(goal-c->facing))*mn(1,dt*7);c->activity=8;continue;}
 g.lifeHunger[i]=mx(0,g.lifeHunger[i]-dt*.016f);g.lifeEnergy[i]=mx(0,g.lifeEnergy[i]-dt*.009f);c->wait=mx(0,c->wait-dt);npcIdle[i]+=dt;
 int sched=schedule150(i),night=sched==15;int mustHome=night||sched==13||sched==14||g.lifeEnergy[i]<20||(g.lifeHunger[i]<30&&(npc_child(i)||g.lifeWallet[i]<20));int mustEat=!mustHome&&!npc_child(i)&&g.lifeHunger[i]<30;
 if(sleep_tick150(i,dt,night))continue;
 if(home_route140(i,dt,mustHome)){npcBench141[i]=-1;sitting141[i]=mx(0,sitting141[i]-dt*3);life_local_needs(i,dt);continue;}
 if(!mustHome&&!mustEat&&bench_route141(i,dt)){life_local_needs(i,dt);continue;}sitting141[i]=mx(0,sitting141[i]-dt*3);
 if((mustHome&&c->activity!=0)||(mustEat&&c->activity!=1)){citizenLen[i]=citizenStep[i]=0;c->wait=0;lifeThink[i]=0;}
 if(plans<2&&lifeThink[i]<=0&&(citizenStep[i]>=citizenLen[i]&&c->wait<=0||npcBlocked[i]>.8f)){
 float tx,tz;int cycle=(int)(lifeClock/40+i)%5,mode=4;
 if(mustHome){int h=c->home;if(!(g.residence[h].doors&1)){g.residence[h].doors|=1;nav_refresh(hx[h]+4.9f,hz[h]-3);}tx=hx[h]+7.55f;tz=hz[h]-9.5f;mode=0;}
 else if(mustEat){tx=shopX;tz=shopZ-7;mode=1;}
 else if(sched==9||sched==10){tx=hx[c->home]+2;tz=hz[c->home]-10;mode=sched;}
 else if(sched==2){tx=OX-9+(i%5)*2;tz=OZ-7;mode=2;}
 else if(npc_child(i)){mode=11;tx=-24+((i/12)%4)*12;tz=-11+(float)sin(lifeClock*.02f+i)*2;}
 else if(i%4==0||i%7==0){int group=(i/4)%4;float angle=(i%7)*.8976f;tx=(-34+group*22)+(float)cos(angle)*2.4f;tz=8.0f+(float)sin(angle)*1.5f;mode=cycle==0?9:cycle==1?10:8;}
 else if(g.hours>=8&&g.hours<17&&i%3==0){tx=OX-10+(i%4)*3;tz=OZ-7;mode=2;}
 else{unsigned int h=hash32(i*733u+(unsigned int)(lifeClock/22)+g.worldSeed);tx=-155+(h%310);tz=(h&1)?-9:9;mode=4;}
 c->tx=tx;c->tz=tz;c->activity=mode;lastActivity[i]=mode;citizenLen[i]=nav_plan(c->x,c->z,tx,tz,citizenPath[i]);citizenStep[i]=0;npcLastRequest[i]=lifeClock;npcPathRequests++;npcPathErrors+=citizenLen[i]==0;npc_path_log(i);c->wait=cycle==4?14:7;lifeThink[i]=1+(i%4)*.1f;npcBlocked[i]=0;npcReplans[i]++;plans++;
 }
 float remaining=dt*(npc_child(i)?1.50f:1.12f+(i%5)*.07f);int visited=0;float oldx=c->x,oldz=c->z;
 while(remaining>.00001f&&citizenStep[i]<citizenLen[i]&&visited++<8){int n=citizenPath[i][citizenStep[i]];float tx=nav_x(n),tz=nav_z(n),d=dst(c->x,c->z,tx,tz);if(d<.04f){citizenStep[i]++;continue;}float step=mn(d,remaining),dx=(tx-c->x)*step/d,dz=(tz-c->z)*step/d;int moved=0;
 for(int other=0;other<NN;other++)if(other!=i){Citizen*q=&PERSON(other);float dd=dst(c->x+dx,c->z+dz,q->x,q->z);if(dd<.92f){float ox=c->x-q->x,oz=c->z-q->z,od=mx(.001f,dst(c->x,c->z,q->x,q->z));float tangent=(i<other?1:-1)*step*.65f;dx=ox/od*step*.25f-oz/od*tangent;dz=oz/od*step*.25f+ox/od*tangent;}}

 float proposed=(float)sqrt(dx*dx+dz*dz);if(proposed>step&&proposed>.00001f){dx*=step/proposed;dz*=step/proposed;}
 if(nav_clear(c->x+dx,c->z+dz)){c->x+=dx;c->z+=dz;moved=1;}else{if(nav_clear(c->x+dx,c->z)){c->x+=dx;moved=1;}if(nav_clear(c->x,c->z+dz)){c->z+=dz;moved=1;}}
 if(!moved){npcBlocked[i]+=dt;if(npcBlocked[i]>.6f){citizenLen[i]=citizenStep[i]=0;c->wait=0;lifeThink[i]=0;}break;}remaining-=step;if(dst(c->x,c->z,tx,tz)<.08f)citizenStep[i]++;
 }
 if(citizenLen[i]>0&&citizenStep[i]>=citizenLen[i]&&remaining>0){float d=dst(c->x,c->z,c->tx,c->tz);if(d>.04f&&d<3){float st=mn(d,remaining),xx=c->x+(c->tx-c->x)*st/d,zz=c->z+(c->tz-c->z)*st/d;if(nav_clear(xx,zz)){c->x=xx;c->z=zz;}else npcBlocked[i]+=dt;}}
 float moved=dst(oldx,oldz,c->x,c->z);if(moved>.0001f){npcMoving[i]=1;npcBlocked[i]=0;walkPhase[i]+=moved*5;float goal=(float)atan2(c->x-oldx,c->z-oldz);c->facing+=(float)atan2(sin(goal-c->facing),cos(goal-c->facing))*mn(1,dt*9);}
 if(!npcMoving[i]&&(c->activity==8||c->activity==9||c->activity==10)){float ga=(float)atan2((i%2?18:-18)-c->x,7.6f-c->z);c->facing+=(float)atan2(sin(ga-c->facing),cos(ga-c->facing))*mn(1,dt*4);}
 if(!npcMoving[i]&&distance<3.2f&&distance>.6f){float target=(float)atan2(g.x-c->x,g.z-c->z);c->facing+=(float)atan2(sin(target-c->facing),cos(target-c->facing))*mn(1,dt*2.5f);}
 npcGait[i]=lerp(npcGait[i],npcMoving[i]?1:0,mn(1,dt*12));
 life_local_needs(i,dt);
 }
 physicsActor150=-1;npcPlanCursor=(npcPlanCursor+2)%NN;crowd_separate();physicsActor150=-1;
}
/* Sleep advances the same residents on the same ground navigation grid. No despawn, teleport or money reset. */
static void life_skip_hours(float hours){if(!(hours>0&&hours<=24))return;float start=g.absolute,seconds=hours*60;int steps=(int)(seconds*5+.5f);for(int step=0;step<steps;step++){g.absolute=start+(step+1)*(.2f/60);g.hours=(float)fmod(g.absolute,24);g.day=1+(int)(g.absolute/24);life_tick(.2f);}g.absolute=start+hours;g.hours=(float)fmod(g.absolute,24);g.day=1+(int)(g.absolute/24);}

static void collect_salary(void){if(dst(g.x,g.z,OX,OZ+1.8f)>2.4f){say("Начисление на карту — в кассе офиса.");return;}bank_collect();}
