/* Local Discord Rich Presence. No token, OAuth secret, network socket or SDK DLL.
   Protocol: https://docs.discord.com/developers/topics/rpc
   Configuration is optional; a real application/client ID is required. */
static int discord_len203(const char*s){int n=0;while(s[n])n++;return n;}
static int discord_has203(const char*s,const char*needle){for(int i=0;s[i];i++){int k=0;while(needle[k]&&s[i+k]&&s[i+k]==needle[k])k++;if(!needle[k])return 1;}return 0;}
static int discord_id203(const char*text,char*out){int i=0,n=0;while(text[i]==' '||text[i]=='\r'||text[i]=='\n'||text[i]=='\t')i++;while(text[i]>='0'&&text[i]<='9'&&n<21){if(n<20)out[n]=text[i];n++;i++;}if(n<17||n>20){out[0]=0;return 0;}out[n]=0;while(text[i]==' '||text[i]=='\r'||text[i]=='\n'||text[i]=='\t')i++;if(text[i]){out[0]=0;return 0;}return 1;}
static void discord_escape203(char*out,int cap,const char*in){int j=0;for(int i=0;in[i]&&j<cap-2;i++){unsigned char c=(unsigned char)in[i];if(c<32)continue;if(c=='"'||c=='\\')out[j++]='\\';out[j++]=(char)c;}out[j]=0;}
static int discord_payload203(char*out,int cap,unsigned pid,unsigned nonce,double start,const char*details,const char*state,int clear){char d[330],s[330];discord_escape203(d,sizeof(d),details);discord_escape203(s,sizeof(s),state);int n=clear?SN(out,cap,"{\"cmd\":\"SET_ACTIVITY\",\"args\":{\"pid\":%u,\"activity\":null},\"nonce\":\"%u\"}",pid,nonce):SN(out,cap,"{\"cmd\":\"SET_ACTIVITY\",\"args\":{\"pid\":%u,\"activity\":{\"type\":0,\"details\":\"%s\",\"state\":\"%s\",\"timestamps\":{\"start\":%.0f}}},\"nonce\":\"%u\"}",pid,d,s,start,nonce);if(n<0||n>=cap){out[0]=0;return 0;}return n;}
static void discord_gameinfo203(char*details,int ds,char*state,int ss){
 const char*action=screen==1?"В главном меню":screen==24?"Возвращается в район":screen==11?"Изучает карту района":screen==2||help?"Пауза":room204==0?"Осматривает подвал":room204==1?"Исследует чердак":rideIndex202>=0?"Катается во дворе":g.playerY>11.9f?"На верхних этажах или крыше":opening>=0||workPending>=0?"Работает с окнами":panel_wing(g.x,g.z)>=0?"В доме":ab(g.x)>160?"В гаражном кооперативе":"Гуляет по району";
 SN(details,ds,"%s",action);if(screen==1)SN(state,ss,"ОКНО 0.20.4 / Симулятор жизни");else SN(state,ss,"День %d / %s / Ранг %d",g.day,season_name(),g.rank);
}
#ifndef HEADLESS
typedef union{long long align;unsigned char storage[40];} DiscordLock203;
static DiscordLock203 discordLock203;
static HANDLE (*d203CreateFileA)(const char*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE),(*d203CreateThread)(void*,size_t,DWORD(*)(void*),void*,DWORD,DWORD*);
static int (*d203ReadFile)(HANDLE,void*,DWORD,DWORD*,void*),(*d203WriteFile)(HANDLE,const void*,DWORD,DWORD*,void*),(*d203PeekNamedPipe)(HANDLE,void*,DWORD,DWORD*,DWORD*,DWORD*),(*d203CloseHandle)(HANDLE);
static DWORD (*d203GetCurrentProcessId)(void),(*d203WaitForSingleObject)(HANDLE,DWORD);
static void (*d203InitializeCriticalSection)(void*),(*d203EnterCriticalSection)(void*),(*d203LeaveCriticalSection)(void*),(*d203GetSystemTimeAsFileTime)(void*);
static HANDLE discordThread203=0;static int discordStop203=0;static char discordApp203[24],discordDetails203[160],discordState203[160];static float discordClock203=0;static double discordStart203=0;
static int discord_frame203(HANDLE pipe,unsigned opcode,const char*payload,int len){unsigned char frame[2048];if(len<0||len>2040)return 0;for(int k=0;k<4;k++){frame[k]=(unsigned char)(opcode>>(8*k));frame[k+4]=(unsigned char)((unsigned)len>>(8*k));}memcpy(frame+8,payload,len);int sent=0;for(int n=0;n<4&&sent<len+8;n++){DWORD wrote=0;if(!d203WriteFile(pipe,frame+sent,(DWORD)(len+8-sent),&wrote,0)||!wrote)return 0;sent+=(int)wrote;}return sent==len+8;}
static unsigned discord_word203(const unsigned char*p){return (unsigned)p[0]|((unsigned)p[1]<<8)|((unsigned)p[2]<<16)|((unsigned)p[3]<<24);}
static DWORD discord_worker203(void*unused){(void)unused;HANDLE pipe=0;int ready=0,used=0,ticks=0,retry=0;unsigned nonce=0;unsigned char incoming[8193];char details[160],state[160],json[1400];unsigned pid=d203GetCurrentProcessId();
 for(;;){d203EnterCriticalSection(&discordLock203);int stop=discordStop203;memcpy(details,discordDetails203,sizeof(details));memcpy(state,discordState203,sizeof(state));d203LeaveCriticalSection(&discordLock203);if(stop)break;
 if(!pipe){if(retry--<=0){for(int n=0;n<10;n++){char path[96];SN(path,sizeof(path),"\\\\?\\pipe\\discord-ipc-%d",n);HANDLE p=d203CreateFileA(path,0xc0000000u,0,0,3,0,0);if(p&&p!=(HANDLE)(long long)-1){pipe=p;break;}}
 if(pipe){int len=SN(json,sizeof(json),"{\"v\":1,\"client_id\":\"%s\"}",discordApp203);if(!discord_frame203(pipe,0,json,len)){d203CloseHandle(pipe);pipe=0;}ready=0;used=0;ticks=0;}retry=50;}}
 else{int broken=0;DWORD available=0;if(!d203PeekNamedPipe(pipe,0,0,0,&available,0))broken=1;
 if(!broken&&available){DWORD got=0,want=(DWORD)mn((float)available,(float)(8192-used));if(!want||!d203ReadFile(pipe,incoming+used,want,&got,0)||!got)broken=1;else used+=(int)got;}
 while(!broken&&used>=8){unsigned opcode=discord_word203(incoming),len=discord_word203(incoming+4);if(len>4096){broken=1;break;}if((unsigned)used<len+8)break;unsigned char end=incoming[8+len];incoming[8+len]=0;
 if(opcode==2)broken=1;else if(opcode==3){if(!discord_frame203(pipe,4,(char*)incoming+8,(int)len))broken=1;}else if(opcode==1){if(discord_has203((char*)incoming+8,"\"READY\"")){ready=1;ticks=150;}if(discord_has203((char*)incoming+8,"\"ERROR\""))broken=1;}
 incoming[8+len]=end;int consumed=(int)len+8;for(int i=consumed;i<used;i++)incoming[i-consumed]=incoming[i];used-=consumed;
 }
 if(!broken&&ready&&++ticks>=150){int len=discord_payload203(json,sizeof(json),pid,++nonce,discordStart203,details,state,0);if(!len||!discord_frame203(pipe,1,json,len))broken=1;ticks=0;}
 if(!ready&&++ticks>100)broken=1;
 if(broken){d203CloseHandle(pipe);pipe=0;ready=0;used=0;retry=50;}
 }
 Sleep(100);
 }
 if(pipe){if(ready){int len=discord_payload203(json,sizeof(json),pid,++nonce,discordStart203,"","",1);if(len)discord_frame203(pipe,1,json,len);}d203CloseHandle(pipe);}return 0;
}
#define D203_LOAD(p,name) *(void**)(&p)=imp_GetProcAddress(k,name)
static void discord_init203(void){char text[96];FILE*f=fopen("discord_app_id.txt","r");if(!f)return;int count=(int)fread(text,1,sizeof(text)-1,f);fclose(f);text[count]=0;if(!discord_id203(text,discordApp203))return;void*k=imp_LoadLibraryA("kernel32.dll");
 D203_LOAD(d203CreateFileA,"CreateFileA");D203_LOAD(d203CreateThread,"CreateThread");D203_LOAD(d203ReadFile,"ReadFile");D203_LOAD(d203WriteFile,"WriteFile");D203_LOAD(d203PeekNamedPipe,"PeekNamedPipe");D203_LOAD(d203CloseHandle,"CloseHandle");D203_LOAD(d203GetCurrentProcessId,"GetCurrentProcessId");D203_LOAD(d203WaitForSingleObject,"WaitForSingleObject");D203_LOAD(d203InitializeCriticalSection,"InitializeCriticalSection");D203_LOAD(d203EnterCriticalSection,"EnterCriticalSection");D203_LOAD(d203LeaveCriticalSection,"LeaveCriticalSection");D203_LOAD(d203GetSystemTimeAsFileTime,"GetSystemTimeAsFileTime");
 if(!d203CreateFileA||!d203CreateThread||!d203ReadFile||!d203WriteFile||!d203PeekNamedPipe||!d203CloseHandle||!d203GetCurrentProcessId||!d203WaitForSingleObject||!d203InitializeCriticalSection||!d203EnterCriticalSection||!d203LeaveCriticalSection||!d203GetSystemTimeAsFileTime)return;
 DWORD ft[2];d203GetSystemTimeAsFileTime(ft);discordStart203=(double)(((unsigned long long)ft[1]<<32)|ft[0])/10000000.0-11644473600.0;
 discord_gameinfo203(discordDetails203,sizeof(discordDetails203),discordState203,sizeof(discordState203));d203InitializeCriticalSection(&discordLock203);discordStop203=0;discordThread203=d203CreateThread(0,0,discord_worker203,0,0,0);
}
#undef D203_LOAD
static void discord_tick203(float dt){if(!discordThread203)return;discordClock203+=dt;if(discordClock203<5)return;discordClock203=0;char d[160]={0},s[160]={0};discord_gameinfo203(d,sizeof(d),s,sizeof(s));d203EnterCriticalSection(&discordLock203);memcpy(discordDetails203,d,sizeof(d));memcpy(discordState203,s,sizeof(s));d203LeaveCriticalSection(&discordLock203);}
static void discord_shutdown203(void){if(!discordThread203)return;d203EnterCriticalSection(&discordLock203);discordStop203=1;d203LeaveCriticalSection(&discordLock203);d203WaitForSingleObject(discordThread203,300);d203CloseHandle(discordThread203);discordThread203=0;}
#endif
