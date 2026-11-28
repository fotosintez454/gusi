/* Offline local radio. Windows MCI/system codecs; no network or bundled decoder. */
static char radioNames[32][256],radioMessage[200]="WAV / MP3 из radio рядом с EXE";static int radioCount=0,radioIndex=0,radioPlaying=0;static float radioPoll=0;
static int radioLastGain=-1;
static int radio_extension(const char*s){int n=0;while(s[n])n++;if(n<4)return 0;const char*p=s+n-4;return p[0]=='.'&&(((p[1]|32)=='w'&&(p[2]|32)=='a'&&(p[3]|32)=='v')||((p[1]|32)=='m'&&(p[2]|32)=='p'&&p[3]=='3'));}
#ifndef HEADLESS
static unsigned short radioPaths[32][1300];
static HANDLE (*FindFirstFileW)(const unsigned short*,void*);static int (*FindNextFileW)(HANDLE,void*),(*FindClose)(HANDLE),(*WideCharToMultiByte)(unsigned int,unsigned int,const unsigned short*,int,char*,int,const char*,int*);static unsigned int (*GetModuleFileNameW)(HANDLE,unsigned short*,unsigned int);static unsigned int (*mciSendStringW)(const unsigned short*,unsigned short*,unsigned int,HANDLE);
static int wlen(const unsigned short*s){int n=0;while(s[n])n++;return n;}
static int wascii(unsigned short*out,int at,const char*s,int cap){while(*s&&at<cap-1)out[at++]=(unsigned char)*s++;out[at]=0;return at;}
static unsigned int mci_ascii(const char*s){unsigned short cmd[256];wascii(cmd,0,s,256);return mciSendStringW?mciSendStringW(cmd,0,0,0):1;}
static void radio_stop(void){mci_ascii("close oknoradio");radioPlaying=0;}
static void radio_scan(void){radio_stop();radioCount=radioIndex=0;void*k=imp_LoadLibraryA("kernel32.dll"),*mm=imp_LoadLibraryA("winmm.dll");LOAD(k,FindFirstFileW);LOAD(k,FindNextFileW);LOAD(k,FindClose);LOAD(k,GetModuleFileNameW);LOAD(k,WideCharToMultiByte);LOAD(mm,mciSendStringW);if(!FindFirstFileW||!FindNextFileW||!FindClose||!GetModuleFileNameW||!WideCharToMultiByte||!mciSendStringW)return;unsigned short folder[1300],pattern[1300];unsigned char data[592];unsigned int len=GetModuleFileNameW(0,folder,1000);if(!len||len>=1000)return;int end=(int)len;while(end>0&&folder[end-1]!='\\'&&folder[end-1]!='/')end--;end=wascii(folder,end,"radio\\",1300);memcpy(pattern,folder,(end+1)*2);wascii(pattern,end,"*",1300);HANDLE f=FindFirstFileW(pattern,data);if((long long)f==-1)return;do{if(*(unsigned int*)data&16)continue;unsigned short*name=(unsigned short*)(data+44);char utf[256];if(!WideCharToMultiByte(65001,0,name,-1,utf,sizeof(utf),0,0)||!radio_extension(utf))continue;int n=wlen(name);if(end+n>=1299)continue;memcpy(radioPaths[radioCount],folder,end*2);memcpy(radioPaths[radioCount]+end,name,(n+1)*2);SN(radioNames[radioCount],256,"%s",utf);radioCount++;}while(radioCount<32&&FindNextFileW(f,data));FindClose(f);SN(radioMessage,sizeof(radioMessage),"Найдено локальных треков: %d",radioCount);}
static void radio_play(void){radioLastGain=-1;if(!radioCount||!mciSendStringW)return;radio_stop();unsigned short cmd[1500];int n=wascii(cmd,0,"open \"",1500),k=0;while(radioPaths[radioIndex][k]&&n<1390)cmd[n++]=radioPaths[radioIndex][k++];n=wascii(cmd,n,"\" alias oknoradio",1500);unsigned int err=mciSendStringW(cmd,0,0,0);if(!err)err=mci_ascii("play oknoradio");radioPlaying=err==0;SN(radioMessage,sizeof(radioMessage),err?"Windows не смог открыть трек (код %u). Попробуй PCM WAV.":"Локальное воспроизведение. Следующий канал — кнопками.",err);}
static void radio_tick(void){if(!radioPlaying||realTime-radioPoll<1||!mciSendStringW)return;radioPoll=realTime;unsigned short cmd[80],status[64];wascii(cmd,0,"status oknoradio mode",80);if(mciSendStringW(cmd,status,64,0)==0&&status[0]=='s'&&status[1]=='t'&&status[2]=='o'){radioIndex=(radioIndex+1)%radioCount;radio_play();g.radioOn=radioPlaying;g.radioTrack=radioIndex;}}
#else
#include <dirent.h>
static void radio_stop(void){radioPlaying=0;}
static void radio_scan(void){radioCount=radioIndex=0;radioPlaying=0;DIR*d=opendir("radio");if(!d)d=opendir("game_data/radio");if(!d)return;struct dirent*e;while((e=readdir(d))&&radioCount<32)if(radio_extension(e->d_name))SN(radioNames[radioCount++],256,"%s",e->d_name);closedir(d);}
static void radio_play(void){radioLastGain=-1;radioPlaying=radioCount>0;SN(radioMessage,sizeof(radioMessage),"Headless: выбор файла проверяется, нативный звук не воспроизводится");}
static void radio_tick(void){}
#endif
static void radio_action(int action){if(action==2){radio_scan();return;}if(!radioCount){radio_scan();if(!radioCount)return;}if(action==0){if(radioPlaying)radio_stop();else radio_play();g.radioOn=radioPlaying;}else{radioIndex=(radioIndex+radioCount+(action<0?-1:1))%radioCount;radio_play();g.radioOn=radioPlaying;g.radioTrack=radioIndex;}}

/* Physical source only. Playback stays on at home, gain attenuates with distance.
   No radio screen, portable player or menu controls. */
static int radioGain=0;static float radioRetry=-99;
static void radio_world_tick(void){
 if(g.radioOn&&!radioPlaying&&realTime-radioRetry>2){radioRetry=realTime;radioIndex=(int)clamp(g.radioTrack,0,mx(0,radioCount-1));radio_play();}
 if(!g.radioOn&&radioPlaying)radio_stop();
 float d=dst(g.x,g.z,HOME_RADIO_X,HOME_RADIO_Z);float walls=home_annex(g.x,g.z)?1.f:insideHouse==0?.35f:.12f;
 radioGain=radioPlaying&&soundEnabled&&screen==0?(int)(1000*clamp(1-d/18,0,1)*walls*masterVolume*.01f*musicVolume*.01f):0;
#ifndef HEADLESS
 if(radioGain!=radioLastGain){char cmd[100];SN(cmd,sizeof(cmd),"setaudio oknoradio volume to %d",radioGain);mci_ascii(cmd);radioLastGain=radioGain;}
#endif
 g.radioTrack=radioIndex;
}
