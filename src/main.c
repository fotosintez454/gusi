#include "ui.h"
#include "discord203.h"
#include "interiors204_draw.h"
static double cpu_stage[4]={0};
static double prof_now(void){
#ifdef HEADLESS
 return (double)clock()/CLOCKS_PER_SEC;
#else
 static long long freq=0;long long t;if(!freq)QueryPerformanceFrequency(&freq);QueryPerformanceCounter(&t);return (double)t/freq;
#endif
}
static float simAccumulator=0;
static void render_frame(void){visualAlpha=clamp(simAccumulator*30,0,1);
 UI=W/800.f;update_interior();
 camx=g.x;camz=g.z;camy=g.playerY+player_eye180()+(headbob&&screen==0&&((keys['W']||keys['S']||keys['A']||keys['D']))?(float)sin(bob)*bobAmplitude:0);
 if(screen==1){camx=-84;camz=3;camy=2.4f;yaw=.70f;pitch=.13f;}
 else{yaw=g.rot;pitch=g.look;}
 float actualYaw=yaw,actualPitch=pitch;
 if(screen==0&&(g.alcohol>5||g.intox>5)){yaw+=(float)sin(realTime*.9f)*(g.alcohol*.0007f+g.intox*.00035f);pitch+=(float)sin(realTime*1.3f)*(g.alcohol*.0004f+g.intox*.0002f);}
 renderAhead202=visualAlpha/30;ride_camera202();double a=prof_now();camera();if(room204>=0&&screen!=1)room_draw204();else{world(0);doors_draw204();bodies_draw200();hands_draw();}double b=prof_now();smooth_edges();if(room204<0)weather_overlay();double c=prof_now();draw_ui();if(screen==0&&(underwater151()||swimAir141<99.5f)){ur(304,410,240,26,0x1f373b);ur(310,430,228,3,0x344d51);ur(310,430,228*swimAir141/100,3,swimAir141<25?0xc57556:0x82bec4);ut(311,414,"Воздух / Space — всплыть",.34f,0xd2e1db);}if(gpuActive)gpu_flush_world();double d=prof_now();cpu_stage[0]=(b-a)*1000;cpu_stage[1]=(c-b)*1000;cpu_stage[2]=(d-c)*1000;cpu_stage[3]=(d-a)*1000;yaw=actualYaw;pitch=actualPitch;
}
static void tick(float dt){
 realTime+=dt;noticeTime=mx(0,noticeTime-dt);
 if(screen==0&&!help&&pressed[2]&&!microActive&&!bonusActive&&opening<0){screen=22;pressed[2]=0;}
 if(screen==0&&!help)interaction();
 if(uiBuiltScreen!=screen||uiBuiltHelp!=help)draw_ui();
 ui_mouse_dispatch();if(uiConsumed)pressed[1]=0;if(!uiConsumed||uiInjected>=0)controls(dt);ui_mouse_end();
 audioWeather=g.weather;audioInterior=insideHouse>=0||panel_wing(g.x,g.z)>=0;audioPaused=screen!=0||help;radio_world_tick();
 if(screen==0&&!help){g.rot=yaw;g.look=pitch;simAccumulator+=mn(dt,.133f);int steps=0;while(simAccumulator>=1.f/30&&steps++<4){simulate(1.f/30);simAccumulator-=1.f/30;}update_interior();interaction();}else simAccumulator=0;
}
#ifdef HEADLESS
#include "tests.h"

#else
static int creatingWindow=1;
static int mouseCaptured=0,rawMouse=0,rawSkip=0;static long long rawDX=0,rawDY=0;
static int cursorHiddenByMenu=0,windowRectSaved=0,lastFadeScreen206=-99;static RECT windowRect;
/* v0.20.6: borderless fullscreen on the current monitor, restored to the old window rectangle. */
static void window_apply_mode(int on){
 if(!window||!SetWindowPos)return;
 long long style=GetWindowLongPtrA?GetWindowLongPtrA(window,-16):0;
 if(on){
  if(!windowRectSaved&&GetWindowRect){GetWindowRect(window,&windowRect);windowRectSaved=1;}
  MONITORINFO mi;memset(&mi,0,sizeof(mi));mi.size=sizeof(mi);
  RECT area={0,0,1280,720};
  if(MonitorFromWindow&&GetMonitorInfoA){HANDLE mon=MonitorFromWindow(window,2);if(mon&&GetMonitorInfoA(mon,&mi))area=mi.monitor;}
  if(SetWindowLongPtrA)SetWindowLongPtrA(window,-16,(style&~0x00C00000LL&~0x00040000LL&~0x00080000LL&~0x00020000LL)|0x80000000LL);
  SetWindowPos(window,0,area.l,area.t,area.r-area.l,area.b-area.t,0x0040|0x0020);
 }else{
  if(SetWindowLongPtrA)SetWindowLongPtrA(window,-16,(style&~0x80000000LL)|0x00C00000LL|0x00040000LL|0x00080000LL);
  RECT r=windowRect;if(!windowRectSaved){r.l=100;r.t=80;r.r=1380;r.b=840;}
  SetWindowPos(window,0,r.l,r.t,r.r-r.l,r.b-r.t,0x0040|0x0020);
 }
}
static void release_mouse(void){if(ClipCursor)ClipCursor(0);rawDX=rawDY=0;if(mouseCaptured){while(ShowCursor(1)<0){}mouseCaptured=0;}if(cursorHiddenByMenu){while(ShowCursor(1)<0){}cursorHiddenByMenu=0;}}
static LRESULT wndproc(HANDLE h,UINT msg,WPARAM wp,LPARAM lp){
 if(msg==0x20a){uiWheel+=(short)((wp>>16)&65535)/120;return 0;}
 if(msg==0xff&&GetRawInputData){unsigned long long data[32];UINT bytes=sizeof(data);UINT got=GetRawInputData((HANDLE)lp,0x10000003,data,&bytes,24);
  if(got!=(UINT)-1&&got>=48&&got<=sizeof(data)&&*(UINT*)data==0){unsigned char*p=(unsigned char*)data;unsigned short flags=*(unsigned short*)(p+24);
   if(flags&1){rawMouse=0;rawDX=rawDY=0;}else if(mouseCaptured&&rawMouse&&GetForegroundWindow()==window){rawDX+=*(int*)(p+36);rawDY+=*(int*)(p+40);}}
  return DefWindowProcA(h,msg,wp,lp);
 }
 if(msg==2){release_mouse();if(!creatingWindow)PostQuitMessage(0);return 0;}
 if(msg==16){release_mouse();save_game();qol_settings_save();running=0;PostQuitMessage(0);return 0;}
 if(msg==8||(msg==0x1c&&!wp)){release_mouse();if(screen==0)screen=2;memset(keys,0,sizeof(keys));memset(oldkeys,0,sizeof(oldkeys));return DefWindowProcA(h,msg,wp,lp);}
 return DefWindowProcA(h,msg,wp,lp);
}
void start(void){
 platform_init();net_load();radio_scan();if(SetProcessDPIAware)SetProcessDPIAware();audio_init();
 char storage[1024],eventStorage[1100],npcStorage[1100];char*app=getenv("LOCALAPPDATA");if(app){SN(storage,sizeof(storage),"%s\\OKNO_020.save",app);savepath=storage;SN(settingsPath,sizeof(settingsPath),"%s\\OKNO_020.cfg",app);SN(eventStorage,sizeof(eventStorage),"%s\\OKNO_events.csv",app);eventpath=eventStorage;SN(npcStorage,sizeof(npcStorage),"%s\\OKNO_npc_debug.csv",app);npcLogPath=npcStorage;}
 HANDLE inst=GetModuleHandleA(0);WC wc;memset(&wc,0,sizeof(wc));wc.size=sizeof(wc);wc.style=3|0x20;wc.proc=wndproc;wc.instance=inst;wc.cursor=LoadCursorA(0,(const char*)32512);wc.name="OknoNative3D";RegisterClassExA(&wc);
 window=CreateWindowExA(0,wc.name,"OKNO | Professional Window Opener | Update 0.20.4",0x10cf0000,100,80,1280,760,0,0,inst,0);if(!window)ExitProcess(1);dc=GetDC(window);if(SetStretchBltMode)SetStretchBltMode(dc,4);ShowWindow(window,5);
#ifndef FORCE_SOFTWARE
 if(!gpu_init()){gpu_shutdown();if(DestroyWindow){DestroyWindow(window);window=CreateWindowExA(0,wc.name,"OKNO 0.20.4 | CPU fallback",0x10cf0000,100,80,1280,760,0,0,inst,0);if(!window)ExitProcess(2);dc=GetDC(window);ShowWindow(window,5);}}

 qol_settings_load();if(fullscreenOn)window_apply_mode(1);
 district_material_prepare();season_prepare();
 if(gpuActive){const Texture*warm[]={&mat_brick,&mat_redbrick,&mat_plaster,&mat_paint,&mat_roof,&mat_wood,&mat_floor,&mat_asphalt,&mat_grass,&mat_pavement,&mat_concrete,&mat_metal};for(int n=0;n<12;n++)gpu_texture_index(warm[n]);}
#endif

 creatingWindow=0;
 RAWDEVICE device={1,2,0,window};rawMouse=RegisterRawInputDevices&&GetRawInputData&&RegisterRawInputDevices(&device,1,sizeof(device));
 long long initseed;QueryPerformanceCounter(&initseed);nextWorldSeed=hash32((unsigned int)initseed);new_game();if(!gpuActive){set_resolution(0);antialias=0;}screen=1;discord_init203();
 FILE*perfFile=0;char profilePath[1200];
 long long frequency,last,now;QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&last);MSG msg;
 while(running){
  while(PeekMessageA(&msg,0,0,0,1)){if(msg.message==18)running=0;TranslateMessage(&msg);DispatchMessageA(&msg);}if(!running)break;
  QueryPerformanceCounter(&now);double frameMilliseconds=(double)(now-last)*1000/frequency;float dt=clamp((float)(frameMilliseconds*.001),.001f,.08f);last=now;fps=lerp(fps,1000.f/(float)mx(.1f,(float)frameMilliseconds),.06f);
  int focus=GetForegroundWindow()==window;
  for(int k=0;k<256;k++){keys[k]=focus&&((GetAsyncKeyState(k)&0x8000)!=0);pressed[k]=keys[k]&&!oldkeys[k];oldkeys[k]=keys[k];}
  if(focus&&(pressed[122]||(keys[18]&&pressed[13])))qol_toggle_fullscreen();
  if(screen!=lastFadeScreen206){lastFadeScreen206=screen;uiFadeStart=realTime;}
  RECT client;GetClientRect(window,&client);int cw=client.r-client.l,ch=client.b-client.t;
  POINT cursor;GetCursorPos(&cursor);ScreenToClient(window,&cursor);uiMouseX=cw>0?cursor.x*800.f/cw:-1000;uiMouseY=ch>0?cursor.y*450.f/ch:-1000;
  static POINT oldCursor={0,0};if(screen==11&&keys[2]){mapCX=clamp(mapCX-(cursor.x-oldCursor.x)*330.f/(mn(cw,1920)*g.mapZoom),-192,192);mapCZ=clamp(mapCZ-(cursor.y-oldCursor.y)*184.f/(mn(ch,1080)*g.mapZoom),-144,144);}oldCursor=cursor;
  int capture=focus&&screen==0&&!help&&mouseLook&&!keys[18];
  POINT mid={cw/2,ch/2};ClientToScreen(window,&mid);
  if(capture&&!mouseCaptured){while(ShowCursor(0)>=0){}mouseCaptured=1;rawSkip=rawMouse?0:1;rawDX=rawDY=0;yaw=g.rot;pitch=g.look;SetCursorPos(mid.x,mid.y);}
  if(!capture&&mouseCaptured)release_mouse();
  if(!capture&&mouseCaptured)release_mouse();
  if(capture&&cw>0&&ch>0){
   RECT clip;if(rawMouse){clip.l=mid.x;clip.t=mid.y;clip.r=mid.x+1;clip.b=mid.y+1;}else{POINT origin={0,0};ClientToScreen(window,&origin);clip.l=origin.x;clip.t=origin.y;clip.r=origin.x+cw;clip.b=origin.y+ch;}if(ClipCursor)ClipCursor(&clip);
   float dx=0,dy=0;if(rawMouse){dx=(float)rawDX;dy=(float)rawDY;}else{POINT p;GetCursorPos(&p);dx=p.x-mid.x;dy=p.y-mid.y;}
   rawDX=rawDY=0;if(rawSkip){dx=dy=0;rawSkip=0;}
   dx=clamp(dx,-2000,2000);dy=clamp(dy,-2000,2000);float aim=keys[2]?aimMultiplier:1;yaw+=dx*sensitivity*aim;pitch=clamp(pitch-dy*sensitivity*aim*(invertY?-1:1),-1.2f,1.2f);
   if(ab(yaw)>PI*100)yaw=(float)fmod(yaw,PI*2);g.rot=yaw;g.look=pitch;SetCursorPos(mid.x,mid.y);
  }else{rawDX=rawDY=0;}
  if(pressed[120]){if(perfFile){fclose(perfFile);perfFile=0;say("Профиль сохранён в LOCALAPPDATA.");}else{SN(profilePath,sizeof(profilePath),"%s\\OKNO_profile_%.0f.csv",app?app:".",(double)now);perfFile=fopen(profilePath,"w");if(perfFile)SN(notice,sizeof(notice),"F9: профилирование включено. F9 — остановить.");noticeTime=4;if(perfFile){const char*header="frame_dt_ms,world_cpu_ms,post_cpu_ms,ui_cpu_ms,render_cpu_ms,draw_batches,triangles,estimated_gl_texture_bytes,backend\n";int len=0;while(header[len])len++;fwrite(header,1,len,perfFile);}}}
  if(focus){net_key_input();tick(dt);}net_tick();
  if(mouseCaptured&&(screen!=0||help||!mouseLook))release_mouse();
  if(focus&&screen!=0&&!cursorHiddenByMenu&&!mouseCaptured){while(ShowCursor(0)>=0){}cursorHiddenByMenu=1;}
  if((!focus||screen==0)&&cursorHiddenByMenu&&!mouseCaptured){while(ShowCursor(1)<0){}cursorHiddenByMenu=0;}
  radio_tick();audio_pump();discord_tick203(dt);
  gpuOutputW=cw;gpuOutputH=ch;
  if(cw>0&&ch>0)render_frame();
  if(cw>0&&ch>0&&gpuActive)gpu_present();
  if(cw>0&&ch>0&&!gpuActive){BMI bmi;memset(&bmi,0,sizeof(bmi));bmi.size=40;bmi.w=W;bmi.h=-H;bmi.planes=1;bmi.bits=32;StretchDIBits(dc,0,0,cw,ch,0,0,W,H,frame,&bmi,0,0x00cc0020);}
  if(perfFile){char row[320];int len=SN(row,sizeof(row),"%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d,%u,%s\n",frameMilliseconds,cpu_stage[0],cpu_stage[1],cpu_stage[2],cpu_stage[3],gpuActive?gpuDrawCalls:-1,draws,(unsigned int)gpuTextureBytes,gpuActive?"OpenGL":"CPU");if(len>0&&len<(int)sizeof(row))fwrite(row,1,len,perfFile);}
  QueryPerformanceCounter(&now);float used=(float)((double)(now-last)/frequency);float budget=1.f/frameLimit;if(used<budget)Sleep((DWORD)((budget-used)*1000));if(!focus||cw<=0||ch<=0)Sleep(40);
 }
 release_mouse();net_disconnect();if(netWSACleanup&&netWsaReady)netWSACleanup();discord_shutdown203();if(perfFile)fclose(perfFile);save_game();radio_stop();gpu_shutdown();audio_shutdown();ExitProcess(0);
}
#endif
