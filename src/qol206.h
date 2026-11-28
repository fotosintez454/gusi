/* v0.20.6: quality-of-life layer — window mode, persisted settings, UI motion, custom cursor.
   Settings live in OKNO_020.cfg next to the save: they survive even before the first save. */
static int fullscreenOn=0,uiSoundOn=1,uiFadeOn=1;
static char settingsPath[1200];
static float uiFadeStart=-99.f,uiFadeDur=.22f;
static void window_apply_mode(int on);
#ifdef HEADLESS
static void window_apply_mode(int on){(void)on;}
#endif

static int qol_key_int(const char*text,const char*key,int def){
 int n=0;while(key[n])n++;
 const char*p=text;
 while(*p){int j=0;while(j<n&&p[j]==key[j])j++;if(j==n){int v=0;p+=n;while(*p>='0'&&*p<='9'){v=v*10+(*p-'0');p++;}return v;}while(*p&&*p!='\n')p++;if(*p)p++;}
 return def;
}
static void qol_settings_load(void){
 if(!settingsPath[0])return;
 FILE*f=fopen(settingsPath,"r");if(!f)return;
 char buf[512];int got=(int)fread(buf,1,sizeof(buf)-1,f);fclose(f);if(got<=0)return;buf[got]=0;
 fullscreenOn=clamp(qol_key_int(buf,"fullscreen=",fullscreenOn),0,1);
 musicVolume=clamp(qol_key_int(buf,"music=",musicVolume),0,100);
 masterVolume=clamp(qol_key_int(buf,"master=",masterVolume),0,100);
 effectsVolume=clamp(qol_key_int(buf,"effects=",effectsVolume),0,100);
 uiSoundOn=clamp(qol_key_int(buf,"uisound=",uiSoundOn),0,1);
 uiFadeOn=clamp(qol_key_int(buf,"fade=",uiFadeOn),0,1);
}
static void qol_settings_save(void){
 if(!settingsPath[0])return;
 char buf[256];int n=SN(buf,sizeof(buf),"fullscreen=%d\nmusic=%d\nmaster=%d\neffects=%d\nuisound=%d\nfade=%d\n",fullscreenOn,musicVolume,masterVolume,effectsVolume,uiSoundOn,uiFadeOn);
 FILE*f=fopen(settingsPath,"w");if(!f)return;fwrite(buf,1,n,f);fclose(f);
}
static void qol_set_volume(int slot,int v){v=(int)clamp(v,0,100);if(slot==0)masterVolume=v;else if(slot==1)musicVolume=v;else effectsVolume=v;if(slot==2)audio(5);qol_settings_save();}
static void qol_toggle_fullscreen(void){fullscreenOn=!fullscreenOn;window_apply_mode(fullscreenOn);qol_settings_save();audio(5);}
