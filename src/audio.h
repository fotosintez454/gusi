/* Procedural audio. Four queued WinMM buffers, independent event voices. */
static int soundEnabled=1,masterVolume=75,musicVolume=70,ambientVolume=24,effectsVolume=75;
static float samplewaves[20][7000],sinewave[1024],audioPhase=0,audioBird=0,audioLow=0;
static unsigned int audioSeed=1234567;
typedef struct{int sound,pos;float gain,cursor,rate;} Voice;
static Voice voices[8];static int voiceNext=0,audioWeather=0,audioPaused=1,audioInterior=0;
static float noise_audio(void){audioSeed=audioSeed*1664525u+1013904223u;return (audioSeed>>8)*(2.f/16777216.f)-1;}
static void audio(int n){if(!soundEnabled||n<0||n>=20)return;Voice*v=&voices[voiceNext++%8];v->sound=n;v->pos=0;v->cursor=0;v->rate=(n>=8&&n<=11)||(n>=16&&n<=18)?1+noise_audio()*.07f:1;v->gain=(n>=8&&n<=11)||(n>=16&&n<=18)?.85f:1;}
static int footstep_sound160(int indoor,int winter,int wet,int hardSurface){if(indoor)return 8;if(winter)return wet?18:11;if(wet)return hardSurface?16:17;return hardSurface?9:10;}
#ifndef HEADLESS
typedef struct {char*data;unsigned int len,recorded;unsigned long long user;unsigned int flags,loops;void*next;unsigned long long reserved;} WHDR;
static unsigned int (*waveOutOpen)(void**,unsigned int,const void*,unsigned long long,unsigned long long,unsigned int),(*waveOutPrepareHeader)(void*,WHDR*,unsigned int),(*waveOutUnprepareHeader)(void*,WHDR*,unsigned int),(*waveOutWrite)(void*,WHDR*,unsigned int),(*waveOutReset)(void*),(*waveOutClose)(void*);
static int prepared[4],started[4];static void*audioDevice=0;static WHDR headers[4];static short chunks[4][512];
#endif
static void mix_audio(short*out,int count){for(int i=0;i<count;i++){
 float noise=noise_audio();audioLow=audioLow*.98f+noise*.02f;audioPhase+=.000039f;if(audioPhase>=1)audioPhase-=1;audioBird+=.00031f;if(audioBird>=1)audioBird-=1;
 float ambient=audioLow*.32f;/* Rain channel deliberately removed. */
 if(!winterEnabled&&audioWeather==0&&audioPhase<.10f)ambient+=sinewave[(int)(audioBird*1024)&1023]*.025f;
 float val=ambient*ambientVolume*.01f*(audioPaused?.25f:audioInterior?.22f:1);
 for(int v=0;v<8;v++)if(voices[v].pos<7000){Voice*q=&voices[v];int at=q->pos;val+=samplewaves[q->sound][at]*q->gain*effectsVolume*.01f;q->cursor+=q->rate;q->pos=(int)q->cursor;}
 val*=soundEnabled?masterVolume*.01f:0;val=val/(1+.3f*ab(val));out[i]=(short)(clamp(val,-.98f,.98f)*32767);
}}
static void audio_init(void){
 for(int i=0;i<1024;i++)sinewave[i]=(float)sin(2*PI*i/1024);
 for(int n=0;n<20;n++)for(int i=0;i<7000;i++){float t=i/22050.f,e=1-i/7000.f,v=0;
  switch(n){case 0:v=(noise_audio()*.08f+(float)sin(2*PI*210*t)*.06f)*e*e*e;break;
  case 1:v=((float)sin(2*PI*2180*t)*.10f+(float)sin(2*PI*3130*t)*.045f+noise_audio()*.02f)*e*e*e;break;
  case 2:v=((float)sin(2*PI*(125-45*t)*t)*.21f+noise_audio()*.06f)*e;break;
  case 3:v=noise_audio()*.16f*e*e*e;break;
  case 4:v=(float)sin(2*PI*(900+t*600)*t)*.13f*e*e;break;
  case 5:v=((float)sin(2*PI*880*t)+(float)sin(2*PI*1174*t))*.075f*e;break;
  case 6:v=((float)sin(2*PI*130*t)*.2f+noise_audio()*.09f)*e*e;break;
  case 8:v=((float)sin(2*PI*95*t)*.19f+noise_audio()*.03f)*e*e*e*e;break;
  case 9:v=noise_audio()*.16f*e*e*e*e*e;break;
  case 10:v=(noise_audio()*.075f+(float)sin(t*400)*.03f)*e*e*e;break;
  case 11:v=(noise_audio()*.19f+(float)sin(t*1450)*.055f)*e*e*e;break;
  case 12:{float click=mx(0,1-t/.045f),creak=mx(0,1-ab(t-.13f)/.12f);v=noise_audio()*.12f*click*click+((float)sin(2*PI*(260+32*(float)sin(t*39))*t))*.065f*creak*e;break;}
  case 13:{float knock=mx(0,1-ab(t-.10f)/.055f);v=((float)sin(2*PI*115*t)*.16f+noise_audio()*.08f)*knock*knock;break;}
  case 14:v=((float)sin(2*PI*77*t)*.15f+noise_audio()*.08f)*e*e*e*e;break;
  case 7:{float env=(float)sin(PI*clamp(t/.317f,0,1));float f=460+330*(float)sin(t*10);v=((float)sin(2*PI*f*t)*.55f+(float)sin(4*PI*f*t)*.19f+(float)sin(6*PI*f*t)*.07f)*env*.36f;break;}
  case 15:v=((float)sin(2*PI*440*t)*.04f+(float)sin(2*PI*660*t)*.025f)*(float)sin(PI*i/7000.f);break;
  /* Additional footfalls: rain on pavement, wet soil, and melting snow. */
  case 16:{float hit=mx(0,1-t/.085f);v=(noise_audio()*.18f+(float)sin(2*PI*(95-38*t)*t)*.15f)*hit*hit;break;}
  case 17:{float hit=mx(0,1-t/.13f);v=(noise_audio()*.105f+(float)sin(2*PI*(68-24*t)*t)*.19f+(float)sin(2*PI*230*t)*.025f)*hit*hit;break;}
  case 18:{float hit=mx(0,1-t/.16f);v=(noise_audio()*.17f+(float)sin(2*PI*(105-42*t)*t)*.13f+(float)sin(2*PI*720*t)*.035f)*hit*hit;break;}
  default:v=(noise_audio()*.05f+(float)sin(t*650)*.08f)*e*e;break;}
  samplewaves[n][i]=v*mn(1,t/.004f);
 }
 for(int v=0;v<8;v++)voices[v].pos=7000;
#ifndef HEADLESS
 void*mm=imp_LoadLibraryA("winmm.dll");LOAD(mm,waveOutOpen);LOAD(mm,waveOutPrepareHeader);LOAD(mm,waveOutUnprepareHeader);LOAD(mm,waveOutWrite);LOAD(mm,waveOutReset);LOAD(mm,waveOutClose);
 unsigned char fmt[18];memset(fmt,0,18);*(unsigned short*)fmt=1;*(unsigned short*)(fmt+2)=1;*(unsigned int*)(fmt+4)=22050;*(unsigned int*)(fmt+8)=44100;*(unsigned short*)(fmt+12)=2;*(unsigned short*)(fmt+14)=16;
 if(waveOutOpen&&waveOutOpen(&audioDevice,0xffffffffu,fmt,0,0,0)==0){for(int b=0;b<4;b++){headers[b].data=(char*)chunks[b];headers[b].len=sizeof(chunks[b]);prepared[b]=waveOutPrepareHeader(audioDevice,&headers[b],sizeof(WHDR))==0;}}
 else audioDevice=0;
#endif
}
static void audio_pump(void){
#ifndef HEADLESS
 if(audioDevice)for(int b=0;b<4;b++)if(prepared[b]&&(!started[b]||((headers[b].flags&1)&&!(headers[b].flags&16)))){mix_audio(chunks[b],512);if(waveOutWrite(audioDevice,&headers[b],sizeof(WHDR))==0)started[b]=1;}
#endif
}
static void audio_shutdown(void){
#ifndef HEADLESS
 if(audioDevice){waveOutReset(audioDevice);for(int b=0;b<4;b++)if(prepared[b])waveOutUnprepareHeader(audioDevice,&headers[b],sizeof(WHDR));waveOutClose(audioDevice);}
#endif
}
