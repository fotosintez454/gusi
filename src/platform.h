/* Minimal Win64 platform. No external runtime redistribution. */
#ifdef HEADLESS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define SN snprintf
#else
typedef unsigned long long size_t;
typedef void FILE;
extern void *(*imp_LoadLibraryA)(const char*);
extern void *(*imp_GetProcAddress)(void*,const char*);
static double (*sin)(double),(*cos)(double),(*sqrt)(double),(*tan)(double),(*atan2)(double,double),(*floor)(double),(*fmod)(double,double);
static void *(*malloc)(size_t); static void (*free)(void*);
static FILE *(*fopen)(const char*,const char*); static size_t (*fread)(void*,size_t,size_t,FILE*),(*fwrite)(const void*,size_t,size_t,FILE*); static int (*fclose)(FILE*);
static int (*SN)(char*,size_t,const char*,...); static char*(*getenv)(const char*);
void *memset(void*p,int c,size_t n){unsigned char*q=p;while(n--)*q++=c;return p;}
void *memcpy(void*d,const void*s,size_t n){unsigned char*a=d;const unsigned char*b=s;while(n--)*a++=*b++;return d;}
typedef void* HANDLE; typedef unsigned int DWORD; typedef unsigned int UINT; typedef unsigned short WORD; typedef long long LRESULT; typedef unsigned long long WPARAM; typedef long long LPARAM;
typedef struct{int x,y;} POINT;
typedef struct{int l,t,r,b;} RECT;
typedef struct{HANDLE hwnd;UINT message;WPARAM wParam;LPARAM lParam;DWORD time;POINT pt;DWORD priv;} MSG;
typedef struct{UINT size,style;LRESULT(*proc)(HANDLE,UINT,WPARAM,LPARAM);int ce,we;HANDLE instance,icon,cursor,background;const char*menu;const char*name;HANDLE smallicon;} WC;
typedef struct{DWORD size;int w,h;WORD planes,bits;DWORD compression,image;int xp,yp;DWORD colors,important;} BMI;
static HANDLE (*GetModuleHandleA)(const char*),(*LoadCursorA)(HANDLE,const char*),(*CreateWindowExA)(DWORD,const char*,const char*,DWORD,int,int,int,int,HANDLE,HANDLE,HANDLE,void*),(*GetDC)(HANDLE),(*GetForegroundWindow)(void);
static unsigned short (*RegisterClassExA)(const WC*);
static LRESULT (*DefWindowProcA)(HANDLE,UINT,WPARAM,LPARAM);
static int (*PeekMessageA)(MSG*,HANDLE,UINT,UINT,UINT),(*TranslateMessage)(const MSG*); static LRESULT(*DispatchMessageA)(const MSG*);
static int (*ShowWindow)(HANDLE,int),(*GetClientRect)(HANDLE,RECT*),(*GetCursorPos)(POINT*),(*SetCursorPos)(int,int),(*ClientToScreen)(HANDLE,POINT*),(*ScreenToClient)(HANDLE,POINT*),(*ShowCursor)(int),(*SetWindowTextA)(HANDLE,const char*);
typedef struct {unsigned short page,usage;unsigned int flags;HANDLE target;} RAWDEVICE;
static int (*DestroyWindow)(HANDLE);
static int (*ClipCursor)(const RECT*),(*RegisterRawInputDevices)(const RAWDEVICE*,UINT,UINT),(*SetProcessDPIAware)(void);
static UINT (*GetRawInputData)(HANDLE,UINT,void*,UINT*,UINT);
static short (*GetAsyncKeyState)(int);
static void (*PostQuitMessage)(int),(*Sleep)(DWORD),(*ExitProcess)(UINT);
static int (*MoveFileExA)(const char*,const char*,DWORD);
static int (*QueryPerformanceCounter)(long long*),(*QueryPerformanceFrequency)(long long*);
static int (*SetStretchBltMode)(HANDLE,int);
static int (*StretchDIBits)(HANDLE,int,int,int,int,int,int,int,int,const void*,const BMI*,UINT,DWORD);
static int (*PlaySoundA)(const char*,HANDLE,DWORD);
static HANDLE window,dc;
#define LOAD(lib,n) *(void**)(&n)=imp_GetProcAddress(lib,#n)
static void platform_init(void){
 void*k=imp_LoadLibraryA("kernel32.dll"),*u=imp_LoadLibraryA("user32.dll"),*g=imp_LoadLibraryA("gdi32.dll"),*c=imp_LoadLibraryA("msvcrt.dll");
 void*mm=imp_LoadLibraryA("winmm.dll");LOAD(mm,PlaySoundA);
 LOAD(c,sin);LOAD(c,cos);LOAD(c,sqrt);LOAD(c,tan);LOAD(c,atan2);LOAD(c,floor);LOAD(c,fmod);LOAD(c,malloc);LOAD(c,free);LOAD(c,fopen);LOAD(c,fread);LOAD(c,fwrite);LOAD(c,fclose);LOAD(c,getenv);*(void**)(&SN)=imp_GetProcAddress(c,"_snprintf");
 LOAD(k,MoveFileExA);LOAD(k,GetModuleHandleA);LOAD(k,Sleep);LOAD(k,ExitProcess);LOAD(k,QueryPerformanceCounter);LOAD(k,QueryPerformanceFrequency);
 LOAD(u,DestroyWindow);LOAD(u,ClipCursor);LOAD(u,RegisterRawInputDevices);LOAD(u,GetRawInputData);LOAD(u,SetProcessDPIAware);
 LOAD(u,LoadCursorA);LOAD(u,RegisterClassExA);LOAD(u,CreateWindowExA);LOAD(u,GetDC);LOAD(u,DefWindowProcA);LOAD(u,PeekMessageA);LOAD(u,TranslateMessage);LOAD(u,DispatchMessageA);LOAD(u,ShowWindow);LOAD(u,GetClientRect);LOAD(u,GetCursorPos);LOAD(u,SetCursorPos);LOAD(u,ClientToScreen);LOAD(u,ScreenToClient);LOAD(u,ShowCursor);LOAD(u,GetAsyncKeyState);LOAD(u,PostQuitMessage);LOAD(u,GetForegroundWindow);LOAD(u,SetWindowTextA);LOAD(g,StretchDIBits);LOAD(g,SetStretchBltMode);
}
#endif
