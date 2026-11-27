/* Original 0.9.5 economy, interactions and bounded URL handling. */
static const char*bizNames[]={"Оконная бригада","Пункт вторсырья","Доставка","Ремонтная мастерская"};
static void expansion_defaults(void){if(g.featureRevision!=1){g.featureRevision=1;memset(g.bizLevel,0,sizeof(g.bizLevel));g.bizCapital=g.bizReceivable=g.bizRevenue=g.bizCosts=g.bizLastGross=g.bizLastCost=0;g.bizDay=g.day;g.homeLevel=g.herb=0;g.herbDay=-1;g.intox=0;g.tvURL[0]=0;}if(g.bizDay>g.day||g.bizDay<1)g.bizDay=g.day;}
static int biz_upgrade_cost(int n){return n>=0&&n<4&&g.bizLevel[n]<3?(25000+10000*n)*(g.bizLevel[n]+1)*(g.bizLevel[n]+1):0;}
static int biz_upgrade(int n){int cost=biz_upgrade_cost(n);if(!cost||g.money<cost){say("Не хватает денег или достигнут третий уровень.");return 0;}g.money-=cost;g.bizLevel[n]++;save_game();say("Направление открыто/расширено. Пополни оборотный бюджет для работы.");return 1;}
static int biz_invest(int amount){if(amount<=0||amount>g.money||g.bizCapital>10000000-amount)return 0;g.money-=amount;g.bizCapital+=amount;save_game();return 1;}
static int biz_collect(void){int value=g.bizReceivable;if(value<=0||g.money>10000000-value)return 0;g.bizReceivable=0;g.money+=value;save_game();return value;}
static void economy_tick(void){if(g.bizDay>=g.day)return;int first=g.bizDay+1;if(g.day-first>30)first=g.day-30;for(int day=first;day<=g.day;day++){g.bizLastGross=g.bizLastCost=0;for(int n=0;n<4;n++){int level=g.bizLevel[n];if(!level)continue;int cost=level*(65+n*18);if(g.bizCapital<cost)continue;float demand=.85f+((hash32(g.worldSeed+day*71+n*913)>>8)%350)/1000.f;int gross=(int)((cost+level*(150+n*25)+mn(g.reputation,100))*demand),tax=gross*13/100;g.bizCapital-=cost;g.bizReceivable=(int)mn(10000000,g.bizReceivable+gross-tax);g.bizRevenue=(int)mn(10000000,g.bizRevenue+gross);g.bizCosts=(int)mn(10000000,g.bizCosts+cost+tax);g.bizLastGross+=gross;g.bizLastCost+=cost+tax;}eventValue=(float)(g.bizLastGross-g.bizLastCost);event_log("business_day_closed",day);}g.bizDay=g.day;}
static int home_upgrade_cost(void){int prices[]={18000,48000,95000};return g.homeLevel<3?prices[g.homeLevel]:0;}
static int home_upgrade(void){int cost=home_upgrade_cost();if(insideHouse!=0||g.playerY>.3f){say("Улучшения квартиры оформляются дома.");return 0;}if(!cost||g.money<cost){say("Не хватает денег или все три улучшения куплены.");return 0;}g.money-=cost;g.homeLevel++;save_game();say("Дом улучшен: отделка, отдых и оснащение обновлены.");return 1;}
static int herb_pick(void){if(g.playerY>.3f||g.homeLevel<3||dst(g.x,g.z,hx[0]-11.7f,hz[0]-3.5f)>2||g.herbDay==g.day)return 0;if(!inv_add(9,1))return 0;g.herbDay=g.day;save_game();say("Трава добавлена в рюкзак. Употребление вызывает временное опьянение.");return 1;}
static int bench_sit(int n){float x,z;if(n<0||n>=54||g.playerY>.3f)return 0;bench_position141(n,&x,&z);if(dst(g.x,g.z,x,z)>2.8f)return 0;float sx=x+.58f;for(int i=0;i<NN;i++)if(dst(PERSON(i).x,PERSON(i).z,sx,z)<.7f)sx=x-.58f;for(int i=0;i<NN;i++)if(dst(PERSON(i).x,PERSON(i).z,sx,z)<.65f){say("Лавочка занята.");return 0;}seated=n;g.x=sx;g.z=z-.07f;g.rot=yaw=PI;g.look=pitch=0;return 1;}
static void bench_stand(void){if(rideIndex202>=0){ride_exit202();return;}if(seated>=2000){/* v151 outdoor benches: keep current safe approach position. */g.playerY=0;}else if(seated>=1000){int id=seated-1000,h=id/10,f=id%10/2,s=id%2;g.x=hx[h]+(s?2.4f:-2.4f);g.z=hz[h]+2.0f;g.playerY=f*3;}else if(seated>=0){float x,z;bench_position141(seated,&x,&z);g.x=x;g.z=z-1.15f;g.playerY=0;}seated=-1;}

static int trash_start(int n){if(n<0||n>=LOOT_COUNT||bonusActive||g.playerY>.3f||(g.lootMask&(1u<<n)))return 0;float x,z;loot_position(n,&x,&z);if(dst(g.x,g.z,x+.55f,z)>2.7f)return 0;bonus_start(1600+n);say("Поиск: пробел в светлом секторе. Узкая зона — две находки.");return 1;}
static int tv_prefix(const char*s,const char*p){while(*p)if(*s++!=*p++)return 0;return 1;}
static int tv_set_url(const char*url){if(!url)return 0;const char*p=0;const char*prefixes[]={"https://www.youtube.com/watch?v=","https://youtube.com/watch?v=","https://youtu.be/","https://www.youtube.com/shorts/","https://youtube.com/shorts/"};for(int n=0;n<5;n++)if(tv_prefix(url,prefixes[n])){p=url;for(const char*q=prefixes[n];*q;q++)p++;break;}if(!p)return 0;char id[12];for(int n=0;n<11;n++){char c=p[n];if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||c=='-'))return 0;id[n]=c;}id[11]=0;if(p[11]&&p[11]!='?'&&p[11]!='&'&&p[11]!='#'&&p[11]!='/')return 0;SN(g.tvURL,sizeof(g.tvURL),"https://www.youtube.com/watch?v=%s",id);save_game();return 1;}
static int tv_import_file(void){char line[256];FILE*f=fopen("game_data/youtube.txt","rb");if(!f)return 0;int n=(int)fread(line,1,255,f);fclose(f);line[n]=0;for(int k=0;k<n;k++)if(line[k]=='\r'||line[k]=='\n'){line[k]=0;break;}return tv_set_url(line);}
static int tv_paste(void){
#ifdef HEADLESS
 return 0;
#else
 void*u=imp_LoadLibraryA("user32.dll"),*k=imp_LoadLibraryA("kernel32.dll");int(*open)(HANDLE)=imp_GetProcAddress(u,"OpenClipboard");int(*close)(void)=imp_GetProcAddress(u,"CloseClipboard");HANDLE(*get)(UINT)=imp_GetProcAddress(u,"GetClipboardData");void*(*lock)(HANDLE)=imp_GetProcAddress(k,"GlobalLock");int(*unlock)(HANDLE)=imp_GetProcAddress(k,"GlobalUnlock");size_t(*size)(HANDLE)=imp_GetProcAddress(k,"GlobalSize");if(!open||!close||!get||!lock||!unlock||!size||!open(window))return 0;HANDLE h=get(1);char text[256];int length=0;if(h){size_t cap=size(h);const char*s=lock(h);if(s){while(length<255&&(size_t)length<cap&&s[length]){text[length]=s[length];length++;}unlock(h);}}close();text[length]=0;return tv_set_url(text);
#endif
}
static int tv_open(void){if(!g.tvURL[0]||!tv_set_url(g.tvURL))return 0;
#ifdef HEADLESS
 return 0;
#else
 long long(*open)(HANDLE,const char*,const char*,const char*,const char*,int)=imp_GetProcAddress(imp_LoadLibraryA("shell32.dll"),"ShellExecuteA");if(!open)return 0;screen=2;return open(window,"open",g.tvURL,0,0,1)>32;
#endif
}
