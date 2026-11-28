#include "world.h"
#include "net.h"
#include "radio.h"
/* v0.8: one graphite UI system. No hotkey badges or assignment HUD. */
typedef struct{float x,y,w,h;int action,param;} UIButton;
static UIButton uiButtons[160];static int uiCount=0,uiBuiltScreen=-1,uiBuiltHelp=0,uiInjected=-1,uiWheel=0,uiConsumed=0;static float uiMouseX=-1000,uiMouseY=-1000,mapCX=0,mapCZ=0;
static void ui_hit(float x,float y,float w,float h,int a,int p){if(uiCount<160)uiButtons[uiCount++]=(UIButton){x,y,w,h,a,p};}
/* Paper documents, red pencil, pencil-plan map. Native GUI, no web layer. */
static void paper(int x,int y,int w,int h){rect((int)((x+3)*UI),(int)((y+4)*UI),(int)(w*UI),(int)(h*UI),0x201f1a);rect((int)(x*UI),(int)(y*UI),(int)(w*UI),(int)(h*UI),0x242c30);
 for(int yy=y+2;yy<y+h-2;yy+=3)for(int xx=x+2;xx<x+w-2;xx+=5){unsigned int v=(unsigned int)(xx*193+yy*467);if(v%17==0)ur(xx,yy,2,1,0x293238);}
 ur(x,y,w,1,0x586368);ur(x,y+h-1,w,1,0x3a474d);
}
static void inv_btn(int x,int y,int w,const char*label,int action,int param){int hov=uiMouseX>=x&&uiMouseX<x+w&&uiMouseY>=y&&uiMouseY<y+36;
 static int lastHoverAct=-9999;int hkey=action*1000+param;if(hov){if(hkey!=lastHoverAct&&uiSoundOn&&screen!=0)audio(4);lastHoverAct=hkey;}else if(lastHoverAct==hkey)lastHoverAct=-9999;
 int gx=hov?x-2:x,gy=hov?y-1:y,gw=hov?w+4:w,gh=hov?38:36;
 if(uiPaperMode){if(hov){rect((int)(gx*UI),(int)(gy*UI),(int)(gw*UI),(int)(gh*UI),0x414c53);ur(gx,gy,3,gh,0x813b2d);}ur(x+8,y+35,w-16,1,0x586368);}else{up(gx,gy,gw,gh,hov?0x414c53:0x242c30,.95f);ur(gx,gy+gh-1,gw,1,hov?0x7d8a80:0x586368);}
 float scale=hov?.445f:.43f;while(scale>.29f&&textwidth(label,scale)>gw-24)scale-=.01f;int tx=w<60?x+(w-textwidth(label,scale))/2:x+12;ut(tx,gy+9,label,scale,hov?accent:white);ui_hit(x,y,w,36,action,param);}
static void ui_button(int x,int y,int w,const char*l,int a,int p){inv_btn(x,y,w,l,a,p);}
static float ui_fade_alpha(void){if(!uiFadeOn||uiFadeDur<=0)return 0;float t=(realTime-uiFadeStart)/uiFadeDur;if(t<0)t=0;return 1-clamp(t,0,1);}
/* v0.20.6: a small in-world arrow replaces the OS pointer in menus. */
static void ui_cursor(void){
 if(screen==0)return;
 int x=(int)(uiMouseX*UI),y=(int)(uiMouseY*UI);if(x<-40||y<-40||x>W+40||y>H+40)return;
 for(int i=0;i<14;i++){int w=13-i;if(w<1)w=1;rect(x+1+(int)(i*.52f),y+1+i,w,1,0x0a0d0c);}
 for(int i=0;i<11;i++){int w=10-i;if(w<1)w=1;rect(x+2+(int)(i*.52f),y+2+i,w,1,0xe8e3d3);}
 rect(x+2,y+13,2,5,0xd9d3c2);rect(x+3,y+14,1,3,0x1b201e);
}
/* v0.20.6: click-to-set volume slider, shared by the pause menu. */
static float volTrackX[3]={0},volTrackW[3]={0};
static void vol_slider(int x,int y,int w,const char*label,int value,int slot){
 ut(x,y-18,label,.34f,muted);
 up(x,y,w,10,0x1d2422,1);ur(x,y,w,1,0x3c4643);ur(x,y+9,w,1,0x141a19);
 int fw=(int)((w-2)*value/100.f);if(fw>0)ur(x+1,y+1,fw,8,0xbfa267);
 int kx=x+1+(int)((w-3)*value/100.f);ur(kx,y-4,3,18,0xe8e3d3);ur(kx+1,y-3,1,16,0x9a948a);
 char b[16];SN(b,sizeof(b),"%d%%",value);ut(x+w+12,y-4,b,.36f,white);
 volTrackX[slot]=x;volTrackW[slot]=w;ui_hit(x,y-6,w,22,70,slot);
}
static void separator(int y){ur(42,y,716,1,0x455258);}
static void modal(const char*kicker,const char*title){panel(0,0,W,H,0x111610,.74f);paper(24,20,752,418);ut(44,36,kicker,.35f,0x929f9e);ut(42,55,title,.96f,0xc6cbc4);ut(655,39,"ОКНО / 0.20.4",.30f,0x929f9e);separator(98);}
static void wrapped(int x,int y,const char*s){int pos=0;for(int line=0;line<5&&s[pos];line++){char b[240];int start=pos,last=pos,chars=0;while(s[pos]&&chars<70){if(s[pos]==32)last=pos;if(((unsigned char)s[pos]&192)!=128)chars++;pos++;}if(s[pos]&&last>start)pos=last;int n=pos-start;if(n>238)n=238;memcpy(b,s+start,n);b[n]=0;ut(x,y+line*22,b,.44f,white);while(s[pos]==32)pos++;}}
static int invFullInfo=0;
static short mapPath[MAXPATH];static int mapPathLen=0,mapGoal=-99,mapNeedsExit=0;static unsigned int mapSeed=0;static float mapTime=-99,mapX=0,mapZ=0;
static void nav_target(float*x,float*z){if(navService==1){*x=RECYCLE_X;*z=RECYCLE_Z;}else if(navService==2){*x=hx[0]+7.55f;*z=hz[0]-9.5f;}else if(navService==3){*x=shopX;*z=shopZ-7;}else if(navService==4){*x=ATM_X;*z=ATM_Z-1.5f;}else if(navOffice){*x=OX;*z=OZ-6.6f;}else if(job_accepted(g.activeJob)&&g.workTarget[g.activeJob]>=3000){int n=(g.workTarget[g.activeJob]-3000)/24;*x=cityBlockX[n];*z=cityBlockZ[n]-9;}else if(navHouse>=NH&&navHouse<NH+PANEL_COUNT){int p=navHouse-NH;*x=panel_origin_x(p)-7;*z=panel_origin_z(p)-9.3f;}else{int h=navHouse>=0&&navHouse<NH?navHouse:g.jobs[g.activeJob].house;*x=hx[h]+7.55f;*z=hz[h]-9.5f;}}
static void map_route(void){int goal=navService?-10-navService:navOffice?-1:navHouse>=0?navHouse:g.jobs[g.activeJob].house;if(!navReady||mapSeed!=g.worldSeed||goal!=mapGoal||realTime-mapTime>2.5f||dst(g.x,g.z,mapX,mapZ)>4){mapSeed=g.worldSeed;mapGoal=goal;mapTime=realTime;mapX=g.x;mapZ=g.z;float tx,tz;nav_target(&tx,&tz);mapNeedsExit=0;mapPathLen=nav_plan(g.x,g.z,tx,tz,mapPath);if(mapPathLen==0){int b=panel_wing(g.x,g.z);if(b>=0){float sx,sz;if(b>=60){sx=cityBlockX[b-60];sz=cityBlockZ[b-60]-9;}else if(b>=30){sx=hx[b-30]+7.35f;sz=hz[b-30]-5.6f;}else{sx=panel_origin_x(b/2)+(b%2?7:-7);sz=panel_origin_z(b/2)-9.3f;}mapPathLen=nav_plan(sx,sz,tx,tz,mapPath);mapNeedsExit=mapPathLen>0;}}}}
static int map_px(float world,float origin,float range,int x,int w){return x+(int)((world-origin+range)*w/(range*2));}
static void map_icon(int xx,int yy,int x,int y,int w,int h,U32 col,int kind){if(xx<x+4||xx>x+w-5||yy<y+4||yy>y+h-5)return;ur(xx-3,yy-3,7,7,col);if(kind){ur(xx-1,yy-1,3,3,0x152a2c);}}
static int poiCount200=0;static int poiRects200[8][4];
static void map_poi200(int x,int y,int w,int h,float ox,float oz,float rx,float rz,float wx,float wz,const char*label,U32 color,int target){int xx=map_px(wx,ox,rx,x,w),yy=map_px(wz,oz,rz,y,h);if(xx<x+8||xx>x+w-8||yy<y+8||yy>y+h-8)return;ur(xx-4,yy-4,9,9,0x172329);ur(xx-3,yy-3,7,7,color);if(w>200){int tw=(int)textwidth(label,.30f)+8,lx=(int)clamp(xx+8,x+2,x+w-tw-2),ly=(int)clamp(yy-7,y+2,y+h-17);int base=ly;for(int attempt=0;attempt<16;attempt++){int collision=0;for(int j=0;j<poiCount200;j++){int*r=poiRects200[j];if(lx<r[0]+r[2]+5&&lx+tw+5>r[0]&&ly<r[1]+r[3]+4&&ly+20>r[1])collision=1;}if(!collision)break;int offset=(attempt/2+1)*21;ly=(int)clamp(base+(attempt%2?-offset:offset),y+2,y+h-19);}if(poiCount200<8){int*r=poiRects200[poiCount200++];r[0]=lx;r[1]=ly;r[2]=tw;r[3]=16;}line2((int)(xx*UI),(int)(yy*UI),(int)(lx*UI),(int)((ly+8)*UI),color);ur(lx,ly,tw,16,0x172329);ut(lx+4,ly+3,label,.30f,color);ui_hit(lx,ly,tw,16,60,target);ui_hit(xx-5,yy-5,11,11,60,target);}}
static void map_surface(int x,int y,int w,int h,float ox,float oz,float rx,float rz){poiCount200=0;rect((int)(x*UI),(int)(y*UI),(int)(w*UI),(int)(h*UI),0x242e33);for(int yy=y+16;yy<y+h;yy+=24)ur(x,yy,w,1,0x303e44);float sx=w/(2*rx),sz=h/(2*rz);
 for(int n=0;n<5;n++){float z=n==0?0:n==1?-43:n==2?43:n==3?-106:106;int yy=map_px(z,oz,rz,y,h),half=(int)((n?4:6)*sz);int top=(int)mx(y,yy-half),bot=(int)mn(y+h,yy+half);if(bot>top)ur(x,top,w,bot-top,0x4b575c);}
 for(int n=0;n<4;n++){int xx=map_px(n==0?-94:n==1?94:n==2?-150:150,ox,rx,x,w),half=(int)(4*sx),l=(int)mx(x,xx-half),r=(int)mn(x+w,xx+half);if(r>l)ur(l,y,r-l,h,0x4b575c);}
 /* The map uses the same two courtyard lanes as the world renderer. */
 for(int n=0;n<2;n++){float z=n?72:-76;int yy=map_px(z,oz,rz,y,h),l=map_px(-86,ox,rx,x,w),r=map_px(78,ox,rx,x,w);l=(int)mx(x,l);r=(int)mn(x+w,r);if(yy>y+1&&yy<y+h-2&&r>l)ur(l,yy-1,r-l,3,0x4b575c);}
 for(int n=1;n<mapPathLen;n++){int a=mapPath[n-1],b=mapPath[n],ax=map_px(nav_x(a),ox,rx,x,w),az=map_px(nav_z(a),oz,rz,y,h),bx=map_px(nav_x(b),ox,rx,x,w),bz=map_px(nav_z(b),oz,rz,y,h);if(ax>=x&&ax<x+w&&bx>=x&&bx<x+w&&az>=y&&az<y+h&&bz>=y&&bz<y+h)line2((int)(ax*UI),(int)(az*UI),(int)(bx*UI),(int)(bz*UI),0x924736);}
 for(int p=0;p<PANEL_COUNT;p++){int xx=map_px(panel_origin_x(p),ox,rx,x,w),zz=map_px(panel_origin_z(p),oz,rz,y,h);if(xx>x+9&&xx<x+w-9&&zz>y+5&&zz<y+h-5)ur(xx-8,zz-4,16,8,0x8c8c7c);}
 for(int n=-140;n<140;n+=4){int xx=map_px(n,ox,rx,x,w),zz=map_px(river_z(n),oz,rz,y,h);if(xx>x&&xx<x+w-6&&zz>y+3&&zz<y+h-4)ur(xx,zz-2,6,4,winterEnabled?0x98afba:0x658e99);}
 map_icon(map_px(ATM_X,ox,rx,x,w),map_px(ATM_Z,oz,rz,y,h),x,y,w,h,0x2b736b,1);

 for(int hse=0;hse<NH;hse++){int xx=map_px(hx[hse],ox,rx,x,w),zz=map_px(hz[hse],oz,rz,y,h),bw=(int)mx(4,10*sx),bh=(int)mx(3,9*sz);if(xx-bw/2>=x&&xx+bw/2<x+w&&zz-bh/2>=y&&zz+bh/2<y+h){ur(xx-bw/2,zz-bh/2,bw,bh,hse==0?0x797e65:hse==g.jobs[g.activeJob].house?accent:0x979483);ur(xx-1,zz-bh/2,2,2,0xb8c3bf);}}
 map_icon(map_px(OX,ox,rx,x,w),map_px(OZ,oz,rz,y,h),x,y,w,h,0x5c6c74,1);map_icon(map_px(shopX,ox,rx,x,w),map_px(shopZ,oz,rz,y,h),x,y,w,h,0x787b55,1);map_icon(map_px(KX,ox,rx,x,w),map_px(KZ,oz,rz,y,h),x,y,w,h,0x98654b,1);
 map_icon(map_px(PANEL_X,ox,rx,x,w),map_px(PANEL_Z,oz,rz,y,h),x,y,w,h,0x9b7646,1);for(int k=0;k<LOOT_COUNT;k++){float px,pz;loot_position(k,&px,&pz);map_icon(map_px(px+.55f,ox,rx,x,w),map_px(pz,oz,rz,y,h),x,y,w,h,0x777b5d,1);}
 for(int n=0;n<(npcDebugEnabled?NN:0);n++){int xx=map_px(PERSON(n).x,ox,rx,x,w),zz=map_px(PERSON(n).z,oz,rz,y,h);if(xx>x&&xx<x+w-1&&zz>y&&zz<y+h-1)ur(xx,zz,2,2,n==15?0x7cacd3:0xa8b5a3);}
 map_poi200(x,y,w,h,ox,oz,rx,rz,hx[0]+7.55f,hz[0]-9.5f,"ДОМ",0x94c9a0,2);
 map_poi200(x,y,w,h,ox,oz,rx,rz,shopX,shopZ-7,"МАГАЗИН",0xefc37b,3);
 map_poi200(x,y,w,h,ox,oz,rx,rz,OX,OZ-6.6f,"КАССА / ЗАРПЛАТА",0x8ebee4,0);
 map_poi200(x,y,w,h,ox,oz,rx,rz,ATM_X,ATM_Z,"БАНКОМАТ",0xa6d8da,4);
 map_poi200(x,y,w,h,ox,oz,rx,rz,RECYCLE_X,RECYCLE_Z,"ПРИЁМ ВТОРСЫРЬЯ",0xd0aa8b,1);
 int px=map_px(g.x,ox,rx,x,w),pz=map_px(g.z,oz,rz,y,h);if(px>x+5&&px<x+w-5&&pz>y+5&&pz<y+h-5){float a=g.rot;int tx=px+(int)(sin(a)*6),tz=pz+(int)(cos(a)*6),lx=px+(int)(sin(a+2.5f)*4),lz=pz+(int)(cos(a+2.5f)*4),rxp=px+(int)(sin(a-2.5f)*4),rzp=pz+(int)(cos(a-2.5f)*4);line2((int)(tx*UI),(int)(tz*UI),(int)(lx*UI),(int)(lz*UI),0xb8c3bf);line2((int)(tx*UI),(int)(tz*UI),(int)(rxp*UI),(int)(rzp*UI),0xb8c3bf);ur(px-1,pz-1,3,3,0xb8c3bf);}
}

static void radar(int x,int y){map_route();float old=UI,oldAlpha=overlayAlpha;UI*=g.mapSize;overlayAlpha*=g.mapOpacity;x=(int)((x+160)/g.mapSize-160);y=(int)(y/g.mapSize);up(x,y,160,180,0x242c30,.95f);ut(x+9,y+7,"PDA / РАЙОН",.37f,accent);map_surface(x+8,y+28,144,130,g.x,g.z,48,44);char b[72];float tx,tz;nav_target(&tx,&tz);SN(b,sizeof(b),"Цель %d м / СКМ карта",(int)dst(g.x,g.z,tx,tz));UI=old;overlayAlpha=oldAlpha;}
static void meter(int x,int y,const char*label,float v,U32 c){ut(x,y,label,.37f,muted);ur(x,y+18,90,3,0x34433f);ur(x,y+18,(int)(90*clamp(v,0,100)/100),3,v<20?0xdb806a:c);}
static void draw_arc(float cx,float yy,float radius,float start,float end,U32 color,int thickness){if(gpuActive){gpu_arc(cx,yy,radius,start,end,color,thickness,UI);return;}float xc=cx*UI,yc=yy*UI,r0=radius*UI,r1=(radius+thickness)*UI;int x0=(int)mx(0,xc-r1-1),x1=(int)mn(W-1,xc+r1+1),y0=(int)mx(0,yc-r1-1),y1=(int)mn(H-1,yc+r1+1);int full=end-start>6.27f;
 for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){float dx=x+.5f-xc,dy=y+.5f-yc,d=dx*dx+dy*dy;if(d<r0*r0||d>r1*r1)continue;if(!full){float a=(float)atan2(dy,dx);while(a<start)a+=2*PI;if(a>end)continue;}frame[y*W+x]=color;}}
static void skill_widget(void){
 if(opening<0)return;
 up(318,204,164,153,0x242c30,.94f);uc(213,openHits==0?"ПОВОРОТ РУЧКИ":"ВЕДЕНИЕ СТВОРКИ",.36f,muted);
 ur(335,340,130,3,0x3a4c49);ur(335,340,(int)(130*openProgress),3,0x91baa5);
 if(checkActive){float phase=check_phase();draw_arc(400,278,35,-PI/2,PI*1.5f,0x4c6060,3);
  draw_arc(400,278,35,(checkCenter-checkWidth/2)*2*PI-PI/2,(checkCenter+checkWidth/2)*2*PI-PI/2,0x88be9c,4);
  draw_arc(400,278,35,(checkCenter-.045f)*2*PI-PI/2,(checkCenter+.045f)*2*PI-PI/2,0xe9e6c9,4);
  float a=phase*2*PI-PI/2;line2((int)(400*UI),(int)(278*UI),(int)((400+(float)cos(a)*31)*UI),(int)((278+(float)sin(a)*31)*UI),accent);uc(318,"Нажми в секторе",.32f,white);
 }else if(checkFeedback>0){uc(264,checkResult==2?"ОТЛИЧНО +10":checkResult==1?"ПОЛУЧИЛОСЬ":"ЕЩЁ ПОПЫТКА",.48f,checkResult<0?0xe2a07e:0xa9d6b7);uc(294,checkResult<0?"сектор станет шире":"продолжай спокойно",.30f,muted);}
 else{char b[64];SN(b,sizeof(b),"%d%%",(int)(openProgress*100));uc(258,b,.95f,white);uc(304,"Отмена",.32f,muted);}
}
static void micro_widget(void){if(!microActive)return;up(305,195,190,166,0x242c30,.95f);uc(204,micro_names[microKind],.40f,white);float phase=clamp(microClock/micro_duration(),0,1);draw_arc(400,278,34,-PI/2,PI*1.5f,0x3d515f,3);draw_arc(400,278,34,(microCenter-microWidth/2)*2*PI-PI/2,(microCenter+microWidth/2)*2*PI-PI/2,0x8dbaa9,4);float a=phase*2*PI-PI/2;line2((int)(400*UI),(int)(278*UI),(int)((400+cos(a)*31)*UI),(int)((278+sin(a)*31)*UI),accent);char b[80];SN(b,sizeof(b),"%d / %d / нажми в секторе",microHits,micro_rounds());uc(324,b,.40f,white);uc(346,"Можно отложить",.30f,muted);}

static void options_page(void){
 modal("Параметры","Настройки");
 const char*tabs[]={"ГРАФИКА","ИГРА","ЗВУК","ФОКУС / СЕЗОНЫ"};for(int i=0;i<4;i++){int x=36+i*184;ui_hit(x,103,174,31,4,i);up(x,103,174,31,i==optionsTab?0x465966:0x3b3f31,1);ut(x+10,109,tabs[i],.44f,i==optionsTab?accent:muted);}
 const char*glabels[]={"Разрешение рендера","Качество","Фильтрация текстур","Дальность видимости","Детализация","Угол обзора","Масштаб компактного HUD","Лимит кадров","Полный экран"};
 const char*plabels[]={"Щадящий режим","Чувствительность мыши","Покачивание камеры","Инверсия вертикали","Помощь в проверках","Автопрохождение проверок","Контекстные подсказки","Режим интерфейса","Метка цели","Точность прицела","Амплитуда камеры","Диагностика NPC"};
 const char*flabels[]={"Меньше визуального шума","Прозрачность HUD","Анимация UI","Всплывающие сообщения","Контрастные подсказки","Быстрый режим движения","Размер карты","Прозрачность карты","Зима"};
 const char*alabels[]={"Общая громкость","Окружение: ветер и птицы","Эффекты: шаги, окна, проверки","Звук включён","Музыка / радио"};char b[96];
 for(int n=0;n<option_count();n++){int x=34+(n/6)*370,y=142+(n%6)*41;if(n==optionsRow)up(x,y-2,360,38,0x334550,1);ut(x+8,y,optionsTab==0?glabels[n]:optionsTab==1?plabels[n]:optionsTab==2?alabels[n]:flabels[n],.40f,white);b[0]=0;
  if(optionsTab==0)switch(n){case 0:SN(b,sizeof(b),"%d × %d",W,H);break;case 1:SN(b,sizeof(b),"%s",quality<=1?"Быстро":quality==2?"Баланс":"Чётко / Full HD");break;case 2:SN(b,sizeof(b),"%s",antialias?"Вкл.":"Выкл.");break;case 3:SN(b,sizeof(b),"%d м",(int)viewdist);break;case 4:SN(b,sizeof(b),"%s",detail==0?"Низкая":detail==1?"Средняя":"Высокая");break;case 5:SN(b,sizeof(b),"%d°",(int)field);break;case 6:SN(b,sizeof(b),"%d%%",(int)(hudScale*100));break;case 7:SN(b,sizeof(b),"%d",frameLimit);break;case 8:SN(b,sizeof(b),"%s",fullscreenOn?"Вкл. (F11)":"Выкл.");break;}
  else if(optionsTab==1){int v=n==0?easy:n==2?headbob:n==3?invertY:n==4?skillAssist:n==5?autoChecks:n==11?npcDebugEnabled:showHints;if(n==1)SN(b,sizeof(b),"%.2f",sensitivity/.0028f);else if(n==7)SN(b,sizeof(b),"%s",hudMode==0?"Полный":hudMode==1?"Минимал":"Выключен");else if(n==8)SN(b,sizeof(b),"%s",showGoalMarker?"Да":"Нет");else if(n==9)SN(b,sizeof(b),"%.2f",aimMultiplier);else if(n==10)SN(b,sizeof(b),"%.3f м",bobAmplitude);else SN(b,sizeof(b),"%s",v?"Вкл.":"Выкл.");}
  else if(optionsTab==3){switch(n){case 0:SN(b,sizeof(b),"%s",g.focusMode?"Вкл.":"Выкл.");break;case 1:SN(b,sizeof(b),"%d%%",(int)(g.hudOpacity*100+.5f));break;case 2:SN(b,sizeof(b),"%s",g.uiMotion==0?"OFF":g.uiMotion==1?"LOW":"NORMAL");break;case 3:SN(b,sizeof(b),"%s",g.popupMode?"Да":"Нет");break;case 4:SN(b,sizeof(b),"%s",g.highContrast?"Да":"Нет");break;case 5:SN(b,sizeof(b),"%s",g.fastMode?"Да":"Нет");break;case 6:SN(b,sizeof(b),"%d%%",(int)(g.mapSize*100));break;case 7:SN(b,sizeof(b),"%d%%",(int)(g.mapOpacity*100));break;case 8:SN(b,sizeof(b),"%s",season_name());break;}}
  else{if(n==3)SN(b,sizeof(b),"%s",soundEnabled?"Да":"Нет");else if(n==4)SN(b,sizeof(b),"%d%%",musicVolume);else SN(b,sizeof(b),"%d%%",n==0?masterVolume:n==1?ambientVolume:effectsVolume);}
  ui_hit(x,y-2,252,38,10,n);inv_btn(x+268,y,36,"-",3,n*2);inv_btn(x+312,y,36,"+",3,n*2+1);ut(x+8,y+18,b,.38f,accent);
  if(optionsTab==2){int vv=n==0?masterVolume:n==1?ambientVolume:n==2?effectsVolume:n==4?musicVolume:-1;if(vv>=0){int fw=(int)(102*vv/100.f);up(x+150,y+18,104,9,0x1d2422,1);ur(x+150,y+18,104,1,0x3c4643);if(fw>0)ur(x+151,y+19,fw,7,0xbfa267);ur(x+151+fw,y+15,2,15,0xe8e3d3);}}
 }
 
}
/* PDA / distressed olive & amber. All coordinates in logical 800x450. */
static void inv_icon(int x,int y,int id){U32 paper=0xe1c990,green=0x769275;
 if(id==2||id==6||id==7){ur(x+11,y+5,9,23,id==7?0x769275:id==6?0x967750:0x7faaa8);ur(x+13,y+1,5,6,paper);ur(x+11,y+14,9,7,paper);}
 else if(id==4){ur(x+5,y+7,23,19,0x9a9a7b);ur(x+14,y+9,5,15,0xe7d6ab);ur(x+9,y+14,15,5,0xe7d6ab);}
 else if(id==5){ur(x+7,y+5,19,23,paper);ur(x+7,y+5,19,7,0x8d5c41);ur(x+10,y+17,13,2,0x504a36);}
 else if(id==8){for(int n=0;n<3;n++){line2((int)((x+4+n*6)*UI),(int)((y+28)*UI),(int)((x+15+n*6)*UI),(int)((y+3)*UI),0x9e8771);for(int k=0;k<5;k++)ur(x+5+n*6+k*2,y+24-k*4,6,2,0x5e6d70);}}
 else if(id==9){ur(x+15,y+8,2,21,green);for(int n=0;n<3;n++){ur(x+7+n*2,y+9+n*5,10,3,green);ur(x+17,y+8+n*5,8-n*2,3,green);}}
 else if(id){ur(x+6,y+7,22,20,id==3?green:paper);ur(x+6,y+10,22,5,0x6e6749);}}
static void inv_page(int trade){inv_sync();char b[180];modal(trade?"Магазин у дома":"Снаряжение",trade?"Покупка и обмен":"Рюкзак");SN(b,sizeof(b),"%d руб.",g.money);ut(648,58,b,.55f,accent);
 for(int n=0;n<48;n++){int x=42+n%6*36,y=112+n/6*35,id=g.invSlots[n];up(x,y,34,33,n==invSelected?0x45545f:0x2d373b,1);if(id){inv_icon(x,y,id);SN(b,sizeof(b),"%d",inv_count(id));ut(x+21,y+22,b,.24f,white);}ui_hit(x,y,34,33,20,n);}
 SN(b,sizeof(b),"%.1f кг / 25     %.1f л / 40",inv_weight(),inv_volume());ut(42,402,b,.33f,muted);
 int id=g.invSlots[invSelected];up(278,112,264,210,0x2b353a,1);ut(293,127,id?invNames[id]:inv_weight()>0?"Свободная ячейка":"Пустой рюкзак",.58f,white);
 if(id){inv_icon(295,168,id);SN(b,sizeof(b),"Количество: %d",inv_count(id));ut(342,177,b,.41f,white);SN(b,sizeof(b),"%.2f кг за предмет",invWeights[id]);ut(293,223,b,.38f,muted);ut(293,253,(id==7||id==8)?"Принимается в пункте сбора.":"Выбери действие ниже.",.38f,muted);if(invFullInfo){SN(b,sizeof(b),"Объём: %.2f л / всего %.2f кг",invVolumes[id],inv_count(id)*invWeights[id]);ut(293,284,b,.31f,accent);}inv_btn(278,334,126,id==3?"Кормить кота":"Использовать",21,0);inv_btn(414,334,128,invDropConfirm?"Подтвердить":"Выбросить",22,0);if(trade&&(id==7||id==8)){inv_btn(278,374,126,"Продать один",23,0);inv_btn(414,374,128,"Продать всё",24,0);}else inv_btn(278,374,264,"Где сдать стекло и металл?",36,1);}
 else{ut(293,178,"Здесь пока ничего нет.",.43f,muted);ut(293,210,"Корм для кота лежит дома.",.38f,muted);}
 up(558,112,200,281,0x2b353a,1);ut(570,126,trade?"Товары":"Быстрый доступ",.45f,white);
 if(trade){const char*l[]={"Еда","Вода","Корм","Аптечка","Табак","Пиво"};int prices[]={190,65,110,580,90,150};for(int n=0;n<6;n++){SN(b,sizeof(b),"%s / %d руб.",l[n],market_price(prices[n]));inv_btn(568,155+n*37,180,b,25,n);}}
 else{for(int k=0;k<2;k++){int yy=166+k*70;up(570,yy,176,58,0x354248,1);if(g.quick[k]){inv_icon(575,yy+10,g.quick[k]);ut(616,yy+12,invNames[g.quick[k]],.36f,white);}else ut(585,yy+21,"Не назначено",.39f,muted);ui_hit(570,yy,176,58,26,k);}ut(570,331,"Перетащи предмет в слот",.31f,muted);inv_btn(568,355,180,invFullInfo?"Кратко":"Полная информация",46,0);}
 inv_btn(648,397,110,"Закрыть",1,27);if(invDrag>=0&&g.invSlots[invDrag]){int xx=(int)uiMouseX+8,yy=(int)uiMouseY+8;up(xx,yy,38,38,0x45545f,1);inv_icon(xx,yy,g.invSlots[invDrag]);}}
static void bonus_widget(void){if(!bonusActive)return;int trash=bonusId>=1600;float center=bonus_center(),width=bonus_width(),phase=clamp(bonusClock/bonus_duration(),0,1);int cx=trash?400:210+(hash32(bonusId*37+workHits*917+g.worldSeed)%380),yy=trash?225:177+(hash32(bonusId*19+workHits*317+g.worldSeed)%75);up(cx-82,yy-66,164,132,0x111719,.94f);ut(cx-70,yy-54,trash?"ПОИСК":workHits==0?"РУЧКА / 1 ИЗ 3":workHits==1?"ЗАМОК / 2 ИЗ 3":"СТВОРКА / 3 ИЗ 3",.29f,muted);draw_arc(cx,yy+5,32,-PI/2,PI*1.5f,0x4a5656,3);draw_arc(cx,yy+5,32,(center-width/2)*2*PI-PI/2,(center+width/2)*2*PI-PI/2,0xe5ddba,5);float a=phase*2*PI-PI/2;line2((int)(cx*UI),(int)((yy+5)*UI),(int)((cx+cos(a)*36)*UI),(int)((yy+5+sin(a)*36)*UI),0xe26a57);ut(cx-14,yy-2,"ЛКМ",.31f,white);ut(cx-67,yy+48,trash?"Промах — шум":"Промах — закрыто",.28f,muted);}

static char*ui_find(const char*s,const char*n){for(int i=0;s[i];i++){int j=0;while(n[j]&&s[i+j]==n[j])j++;if(!n[j])return (char*)(s+i);}return 0;}
static int goal_active(void){return navService||navOffice||navHouse>=0||job_accepted(g.activeJob);}
static const char*goal_title(void){if(navService==1)return "Приём стекла и металла";if(navService==2)return "Дом / кровать";if(navService==3)return "Магазин / продукты";if(navService==4)return "Банкомат / снять наличные";if(navOffice)return "Касса / получить оплату";if(!navService&&!navOffice&&job_accepted(g.activeJob)){static char label[128];int j=g.activeJob;SN(label,sizeof(label),"Балкон / дом %d / этаж %d",g.jobs[j].house+1,work_floor(g.workTarget[j])+1);return label;}if(navHouse>=NH)return "Подъезд / вход";int h=navHouse>=0?navHouse:g.jobs[g.activeJob].house;return names[h];}
static void goal_point(float*x,float*y,float*z){nav_target(x,z);*y=1.6f;if(navService==2&&insideHouse==0){*x=hx[0]-1.1f;*z=hz[0]+3.0f;*y=.9f;}if(!navService&&!navOffice&&job_accepted(g.activeJob)){int j=g.activeJob,h=g.jobs[j].house,f=work_floor(g.workTarget[j]);if(g.workTarget[j]>=3000){int n=(g.workTarget[j]-3000)/24;if(high_at180(g.x,g.z)==n){if(ab(g.playerY-f*3)>.25f){*x=cityBlockX[n];*z=cityBlockZ[n]-5.0f;*y=f*3+1.2f;}else work_point(g.workTarget[j],x,y,z);}}else if(residence_at(g.x,g.z)==h){if(ab(g.playerY-f*3)>.25f||g.x>hx[h]+4.65f){*x=hx[h]+4.93f;*z=hz[h]-3;*y=f*3+1.3f;}else work_point(g.workTarget[j],x,y,z);}}}

static void goal_marker(void){if(!goal_active())return;float x,y,z;goal_point(&x,&y,&z);float dx=x-camx,dz=z-camz,dy=y-camy,side=dx*cy-dz*sy,forward=dx*sy+dz*cy,vertical=dy*cp-forward*sp,depth=dy*sp+forward*cp;float xx=400,yy=160;if(depth>.15f){xx=400+side*focal/depth/UI;yy=225-vertical*focal/depth/UI;}else xx=side<0?34:766;int off=depth<=.15f||xx<32||xx>768||yy<118||yy>300;xx=clamp(xx,34,766);yy=clamp(yy,124,296);int ix=(int)xx,iy=(int)yy;line2((int)(ix*UI),(int)((iy-7)*UI),(int)((ix+7)*UI),(int)(iy*UI),accent);line2((int)((ix+7)*UI),(int)(iy*UI),(int)(ix*UI),(int)((iy+7)*UI),accent);line2((int)(ix*UI),(int)((iy+7)*UI),(int)((ix-7)*UI),(int)(iy*UI),accent);line2((int)((ix-7)*UI),(int)(iy*UI),(int)(ix*UI),(int)((iy-7)*UI),accent);char b[64];SN(b,sizeof(b),"%s%d м",off?"Цель / ":"",(int)(sqrt((g.x-x)*(g.x-x)+(g.z-z)*(g.z-z)+(g.playerY+1.7f-y)*(g.playerY+1.7f-y))+.5f));int tw=(int)(textwidth(b,.34f*UI)/UI);int tx=(int)clamp(ix-tw/2,18,782-tw);up(tx-5,iy+12,tw+10,21,0x152127,.95f);ut(tx,iy+16,b,.34f,white);}
static void hud_vital(int x,int y,const char*name,float value,U32 color){
 char b[48];int w=148,h=38;
 up(x,y,w,h,0x14191b,.88f);ur(x,y,w,1,0x39423f);ur(x,y,2,h,value<20?0xe79b80:color);
 SN(b,sizeof(b),"%s",name);ut(x+11,y+6,b,.29f,value<20?0xf0ae91:0x9fa8a2);
 SN(b,sizeof(b),"%d",(int)value);int tw=(int)(textwidth(b,.40f*UI)/UI);ut(x+w-tw-10,y+4,b,.40f,value<20?0xe79b80:0xd8ddd6);
 int bx=x+11,by=y+25,bw=w-22;
 ur(bx,by,bw,4,0x2a3335);int fw=(int)(bw*clamp(value,0,100)/100);if(fw>0)ur(bx,by,fw,4,value<20?0xe79b80:color);
 for(int k=1;k<4;k++)ur(bx+k*bw/4,by,1,4,0x11171a);
}
static void hud_note(int x,int y,const char*t,U32 dot){int tw=(int)(textwidth(t,.32f*UI)/UI);up(x-10,y-5,tw+30,21,0x14191b,.80f);ur(x-10,y-5,2,21,dot);ur(x-3,y+3,4,4,dot);ut(x+8,y,t,.32f,0xb9c0b8);}
static void hud_caption(int x,int y,const char*t,float sc,U32 col){ut(x+1,y+1,t,sc,0x151a16);ut(x,y,t,sc,col);}
static void hud(void){if(hudMode!=2&&g.hudOpacity>.01f){overlayAlpha=g.hudOpacity;char b[240];
 {int cw=132,ch=48,cx=800-cw-16,cy=14;up(cx,cy,cw,ch,0x14191b,.85f);ur(cx,cy,cw,1,0x39423f);ur(cx,cy,2,ch,0xbfa267);
  SN(b,sizeof(b),"%02d:%02d",(int)g.hours,(int)(g.hours*60)%60);ut(cx+12,cy+6,b,.68f,0xe8e3d3);
  SN(b,sizeof(b),"%s / день %d",season_name(),g.day);ut(cx+12,cy+31,b,.25f,0x9aa39d);}
 hud_vital(24,404,"Здоровье",g.health,0xb66855);hud_vital(180,404,"Выносливость",g.stamina,0xb8c29e);
 SN(b,sizeof(b),"%s / карта %d / наличные %d",season_name(),g.bank,g.money);hud_note(24,26,b,0xbfa267);
 int yy=338;const char*labels[]={"Плохо себя чувствую","Надо поесть","Хочется пить","Надо поспать","Надо передохнуть"};float needs[]={g.health,g.hunger,g.thirst,g.energy,g.stamina};int shown=0;for(int i=0;i<5;i++)if(needs[i]<30&&shown<3){hud_note(24,yy,labels[i],needs[i]<12?0xe2a68e:0x8d978c);yy-=25;shown++;}
 if(goal_active()){float x,y,z;goal_point(&x,&y,&z);SN(b,sizeof(b),"%s / %d м",goal_title(),(int)(sqrt((g.x-x)*(g.x-x)+(g.z-z)*(g.z-z)+(g.playerY+1.7f-y)*(g.playerY+1.7f-y))+.5f));hud_note(24,54,b,0x9ab5ac);if(showGoalMarker&&workPending<0)goal_marker();}
 if(prompt[0]&&showHints&&!microActive){const char*start=0;SN(b,sizeof(b),"%s",start?start+5:prompt);char*end=ui_find(b,"  |  ");if(end)*end=0;float size=.42f;while(textwidth(b,size)>540&&size>.30f)size-=.01f;int tw=(int)(textwidth(b,size*UI)/UI);int px=(800-tw-46)/2,py=363;
  up(px,py,tw+46,30,0x14191b,.82f);ur(px,py,tw+46,1,0x39423f);ur(px,py,3,30,0xbfa267);
  up(px+9,py+5,20,20,0x2b3436,1);ut(px+12,py+8,"E",.26f,0xd8c79a);
  ut(px+37,py+8,b,size,0xcdd2c9);}
 if(noticeTime>0&&(g.popupMode||realTime<sleepNoticeUntil||realTime<workNoticeUntil)){up(170,78,460,32,0x242c30,.90f);float ns=.34f;while(textwidth(notice,ns)>435&&ns>.23f)ns-=.01f;uc(87,notice,ns,white);}overlayAlpha=1;}
 if(hudMode!=2&&(interactKind||targetWindow>=0||prompt[0]))rect(W/2-1,H/2-1,2,2,accent);micro_widget();skill_widget();if(itemAnim>0){ur(350,329,100,2,0x514f43);ur(350,329,(int)(100*(1-itemAnim/consume_duration())),2,accent);}}
static void dialogue_button(int y,const char*label,int action,int param){int hover=uiMouseX>=36&&uiMouseX<746&&uiMouseY>=y&&uiMouseY<y+25;if(hover){ur(36,y,710,25,0x34382e);ur(36,y,2,25,0xbfa267);}ut(48,y+5,label,.40f,hover?0xf0d59c:0xd3d0ba);ui_hit(36,y,710,25,action,param);}
static void compact_dialogue(void){
 dialog_ensure();uiPaperMode=0;panel(14*UI,14*UI,772*UI,424*UI,0x111714,.95f);ur(14,14,772,4,0x4f5549);ur(14,434,772,4,0x4f5549);ur(14,14,4,424,0x51574b);ur(782,14,4,424,0x51574b);ur(22,22,756,1,0x777a66);ur(22,430,756,1,0x353e34);
 for(int i=0;i<6;i++){int x=26+i*149;ur(x,26,3,3,0x94957e);ur(x,423,3,3,0x81836c);}ut(36,38,"РАЗГОВОР / ДВОР",.30f,0x969d87);ut(36,57,dialog_name200(dialogNPC),.59f,0xc9a267);ur(36,94,710,1,0x4f5848);ur(36,299,710,1,0x59604e);
 char profile[220];dialog_profile200(profile,sizeof(profile),dialogNPC);ut(36,80,profile,.31f,muted);
 char lines[400][220];U32 colors[400];int count=0;
 for(int e=0;e<dialogCount&&count<386;e++){
  SN(lines[count],220,"%s",dialogLog[e].player?"Ты":dialog_name200(dialogNPC));colors[count++]=dialogLog[e].player?0x9eae81:0xd4aa67;
  const char*t=dialogLog[e].text;int pos=0;while(t[pos]&&count<398){int start=pos,last=pos;char row[220];while(t[pos]&&pos-start<210){int step=((unsigned char)t[pos]<128)?1:((unsigned char)t[pos]<224?2:3);if(t[pos]==' ')last=pos;if(pos-start+step>=216)break;memcpy(row,t+start,pos-start+step);row[pos-start+step]=0;if(textwidth(row,.40f)>680)break;pos+=step;}if(t[pos]&&last>start)pos=last;if(pos==start)pos++;int len=pos-start;memcpy(lines[count],t+start,len);lines[count][len]=0;colors[count++]=0xd0d0bc;while(t[pos]==' ')pos++;}
 }
 int visible=11,maxScroll=count>visible?count-visible:0;dialogScroll=(int)clamp(dialogScroll,0,maxScroll);int first=count-visible-dialogScroll;if(first<0)first=0;for(int n=0;n<visible&&first+n<count;n++)ut(44,101+n*17,lines[first+n],.40f,colors[first+n]);
 ur(759,96,3,187,0x313c31);int thumb=(int)mx(14,187.f*visible/mx(visible,count));int ty=96+(maxScroll?(int)((187-thumb)*(1-dialogScroll/(float)maxScroll)):0);ur(759,ty,3,thumb,0x929a7d);ui_hit(750,92,22,35,11,10);ui_hit(750,253,22,35,11,11);
 dialogue_button(309,"1. Как дела?",11,0);dialogue_button(338,dialogOffer>=0?"2. Повтори адрес и этаж балкона.":"2. Нужен профессиональный открывальщик балкона?",11,1);dialogue_button(367,dialogOffer>=0?"3. Берусь. Отметь нужную створку.":"3. Что нового во дворе?",11,dialogOffer>=0?3:2);dialogue_button(396,"4. До встречи.",1,52);
}

static void business_page(void){modal("От ремесла к своему делу","Бизнес / четыре направления");char b[180];SN(b,sizeof(b),"Оборот: %d / к получению: %d руб.",g.bizCapital,g.bizReceivable);ut(42,116,b,.43f,white);inv_btn(620,109,134,"Бригада",2,7);for(int n=0;n<4;n++){int y=157+n*46;up(42,y,712,41,0x303329,1);SN(b,sizeof(b),"%s / ур. %d",bizNames[n],g.bizLevel[n]);ut(53,y+7,b,.40f,white);SN(b,sizeof(b),"Расход/день: %d руб. + налог с выручки",g.bizLevel[n]*(65+n*18));ut(53,y+26,b,.25f,muted);SN(b,sizeof(b),g.bizLevel[n]<3?"Расширить / %d руб.":"Максимум",biz_upgrade_cost(n));inv_btn(532,y+2,210,b,40,n);}inv_btn(42,350,220,"В оборот / 500 руб.",41,500);SN(b,sizeof(b),"Забрать / %d руб.",g.bizReceivable);inv_btn(278,350,220,b,42,0);inv_btn(514,350,240,"Дом и улучшения",2,21);SN(b,sizeof(b),"Последний день: выручка %d / расходы и налог %d",g.bizLastGross,g.bizLastCost);ut(42,399,b,.31f,muted);if(noticeTime>0)ut(42,427,notice,.25f,accent);}
static void home_page(void){modal("Квартира и пристройка","Дом / улучшения");char b[200];SN(b,sizeof(b),"Уровень ремонта: %d / 3",g.homeLevel);ut(42,120,b,.58f,white);ut(42,168,"1. Тёплая отделка и более удобный отдых",.46f,muted);ut(42,207,"2. Кухня, санузел, утепление: коммуналка 240/сутки",.46f,muted);ut(42,246,"3. Зимний сад и окончательная отделка пристройки",.46f,muted);SN(b,sizeof(b),g.homeLevel<3?"Улучшить / %d руб.":"Дом полностью улучшен",home_upgrade_cost());inv_btn(42,318,340,b,43,0);inv_btn(414,318,340,"Маршрут домой",36,2);if(noticeTime>0)ut(42,375,notice,.30f,accent);}
static void tv_page(void){modal("Дом / телевизор","YouTube / ссылка");ut(42,119,"Видео открывается в браузере. В 3D-экране поток не декодируется.",.40f,white);ut(42,159,g.tvURL[0]?g.tvURL:"Ссылка не задана",.36f,accent);inv_btn(42,212,340,"Вставить ссылку из буфера",44,0);inv_btn(414,212,340,"Из game_data/youtube.txt",44,1);inv_btn(42,271,712,"Открыть YouTube в браузере",44,2);ut(42,330,"Нужен интернет. Поддерживаются watch, youtu.be и shorts.",.40f,muted);if(noticeTime>0)ut(42,375,notice,.32f,accent);}
static void stats_page(void){modal("Персонаж","Самочувствие");const char*l[]={"Сытость","Вода","Сон","Здоровье","Выносливость","Пузырь","Кишечник","Хрюкот / сытость"};float v[]={g.hunger,g.thirst,g.energy,g.health,g.stamina,g.bladder,g.bowel,g.petHunger};char b[80];for(int n=0;n<8;n++){int x=42+n%4*182,y=118+n/4*108;up(x,y,170,88,0x2d373b,1);ut(x+12,y+13,l[n],.40f,muted);SN(b,sizeof(b),"%d / 100",(int)v[n]);ut(x+12,y+38,b,.64f,white);ur(x+12,y+72,(int)(v[n]*1.45f),3,0x9ab5ac);}SN(b,sizeof(b),"Навык %.1f   /   Алкоголь %d%%   /   Стресс %d%%",skillLevel,(int)g.alcohol,(int)g.stress);ut(42,355,b,.44f,muted);}
static void draw_ui(void){UI=W/800.f;uiPaperMode=0;overlayAlpha=1;uiCount=0;uiBuiltScreen=screen;uiBuiltHelp=help;char b[240];
 if(screen==0){hud();bonus_widget();int f202,t202,d202,m202;if(lift_readout202(&f202,&t202,&d202,&m202)){ur(300,40,200,52,0x172d2b);SN(b,sizeof(b),"ЭТАЖ %d / %d",f202,m202);ut(316,49,b,.48f,0x9ee7b0);SN(b,sizeof(b),d202?"%s / едем на %d":"Лифт остановлен",d202>0?"Вверх":"Вниз",t202);ut(316,74,b,.31f,0xc5d0c2);}if(sprintLocked202)ut(295,99,"Нет сил / восстановись до 20",.32f,0xd7b17f);}
 else if(screen==25){modal("НОВЫЙ МИР / СИД","Создать район");ut(42,130,"Оставь поле пустым — каждый раз будет новый район.",.46f,white);ut(42,175,"Либо введи число от 0 до 4294967295. Backspace — стереть.",.39f,muted);SN(b,sizeof(b),"Сид: %s",seedLen140?seedInput140:"случайный");ut(42,227,b,.78f,accent);if(noticeTime>0)ut(42,270,notice,.34f,accent);inv_btn(42,304,340,"Создать / Enter",52,0);inv_btn(414,304,340,"Отмена",2,1);}
 else if(screen==26){modal("СЕТЕВАЯ ИГРА / RADMIN VPN","Локальное лобби");ut(42,121,"Хост: создай лобби. Друг: введи IPv4-адрес хоста из Radmin VPN.",.43f,white);SN(b,sizeof(b),"Адрес друга: %s_",netAddress[0]?netAddress:"введи цифры и точки");ut(42,164,b,.55f,accent);inv_btn(42,218,340,"Создать лобби",61,0);inv_btn(414,218,340,"Подключиться",62,0);ut(42,282,net_status_text(),.50f,white);if(netError[0])ut(42,321,netError,.37f,0xd99a7d);ut(42,365,"Порт: 27001 / TCP. Разреши игру в брандмауэре Windows.",.35f,muted);inv_btn(42,397,220,"Отключиться",63,0);inv_btn(530,397,220,"Назад",2,1);}
 else if(screen==1){panel(0,0,W,H,0x0b100f,.55f);
  up(40,32,360,386,0x12181a,.93f);ur(40,32,360,2,0xbfa267);ur(40,32,2,386,0xbfa267);
  ut(64,52,"ФЕВРАЛЬ. ПОСЛЕ СМЕНЫ.",.30f,0x9aa6a4);
  ut(60,82,"ОКНО",1.75f,0xe8e3d3);
  ur(64,182,308,1,0x4a534d);
  ut(64,194,"Утро начинается не сразу.",.34f,0x9aa6a4);
  inv_btn(64,216,308,"Продолжить",1,13);
  inv_btn(64,256,308,"Новый мир / сид",2,25);
  inv_btn(64,296,308,"Настройки",1,79);
  inv_btn(64,336,308,"Сетевая игра",2,26);
  inv_btn(64,376,308,"Закончить смену",1,27);
  up(420,96,340,258,0x12181a,.82f);ur(420,96,340,2,0x39423f);
  ut(442,114,"УПРАВЛЕНИЕ",.28f,0xbfa267);
  ut(442,144,"WASD / мышь — идти и смотреть",.33f,0xc3c9c1);
  ut(442,168,"E — взаимодействие, ЛКМ — меню",.33f,0xc3c9c1);
  ut(442,192,"Tab — записная книжка, M — карта",.33f,0xc3c9c1);
  ut(442,216,"Esc — пауза, F1 — справка",.33f,0xc3c9c1);
  ur(442,244,296,1,0x39423f);
  ut(442,258,"1996-й. Панельки, зима, чужие балконы.",.31f,0x9aa39d);
  ut(442,282,"Смена начинается в подъезде.",.31f,0x9aa39d);}
 else if(screen==2){modal("Можно немного выдохнуть","Пауза");
  inv_btn(60,118,300,"Продолжить",1,13);inv_btn(60,166,300,"Карта района",2,11);inv_btn(60,214,300,"Опции",1,79);inv_btn(60,262,300,"Сохранить",1,83);inv_btn(60,310,300,"Выйти в главное меню",1,77);
  up(404,110,348,252,0x1a211f,.92f);ur(404,110,348,1,0x39423f);ur(404,110,2,252,0xbfa267);ut(424,120,"ЗВУК И ОКНО",.34f,accent);
  vol_slider(424,172,196,"Общая громкость",masterVolume,0);vol_slider(424,218,196,"Музыка / радио",musicVolume,1);vol_slider(424,264,196,"Эффекты",effectsVolume,2);
  inv_btn(424,306,196,fullscreenOn?"Оконный режим":"Полный экран",71,0);ut(628,318,fullscreenOn?"F11":"F11",.30f,muted);}
 else if(screen==3){modal("Записная книжка / работа","Заявки соседей");for(int j=0;j<NJ;j++){Job*q=&g.jobs[j];int y=118+j*79;up(42,y,712,69,0x2d373b,1);SN(b,sizeof(b),"%s / %s",house_label190(q->house),"Балкон");ut(54,y+9,b,.43f,white);SN(b,sizeof(b),"Дом %d / этаж %d / %d руб.",q->house+1,work_floor(g.workTarget[j])+1,q->pay);ut(54,y+39,b,.38f,muted);inv_btn(585,y+18,156,job_accepted(j)?"На карте":"Принять",32,j);if(job_accepted(j))ut(426,y+39,"В работе",.38f,accent);}inv_btn(42,363,220,"Маршрут к кассе",47,0);inv_btn(278,363,220,"Оплатить счета",1,66);}
 else if(screen==4)inv_page(1);else if(screen==5)inv_page(0);else if(screen==6)options_page();else if(screen==8)stats_page();
 else if(screen==9){modal("Касса","Оплата работы");SN(b,sizeof(b),"К зачислению: %d руб.",g.salary);ut(42,132,b,.86f,white);ut(42,199,"Зарплата поступает на карту. Банкомат — у магазина.",.47f,muted);inv_btn(42,266,340,"Зачислить на карту",1,13);}
 else if(screen==10)compact_dialogue();
 else if(screen==11){modal("Окрестности","Карта района");map_route();map_surface(150,112,500,276,mapCX,mapCZ,192/g.mapZoom,144/g.mapZoom);inv_btn(42,120,90,"Ближе",6,1);inv_btn(42,163,90,"Дальше",6,-1);inv_btn(42,206,90,"Я",12,0);inv_btn(668,120,90,"Офис",12,1);inv_btn(668,163,90,"Магазин",12,2);inv_btn(668,206,90,"Дом",36,2);inv_btn(668,249,90,"Касса",47,0);inv_btn(668,292,90,"Приём",36,1);SN(b,sizeof(b),"Сид %u / %s / карта %d руб.",g.worldSeed,season_name(),g.bank);ut(150,416,b,.30f,white);if(mapNeedsExit)ut(150,395,"Сначала выйди из дома; затем по маршруту.",.34f,white);}
 else if(screen==22){panel(0,0,W,H,0x111610,.65f);paper(210,60,380,332);ut(236,82,"ЗАПИСНАЯ КНИЖКА",.56f,white);inv_btn(234,136,332,"Карта района",2,11);inv_btn(234,186,332,"Рюкзак",2,5);inv_btn(234,236,332,"Заявки",2,3);inv_btn(234,286,332,"Опции / пауза",2,2);inv_btn(234,336,332,"Вернуться",2,0);}
 else if(screen==19)business_page();else if(screen==20)tv_page();else if(screen==21)home_page();
 else if(screen==14){modal("ЛИФТ / КАБИНА И ВЫЗОВ","Выбери этаж");ut(42,121,"С площадки — вызов; внутри — поездка. Дождись кабины.",.43f,white);int floors=liftUI>=30?4:3;for(int f=0;f<=floors;f++){SN(b,sizeof(b),"Этаж %d",f+1);inv_btn(42+(f%3)*244,173+(f/3)*65,228,b,31,f);}ut(42,331,"На крышу — по лестнице с самого верхнего этажа.",.36f,muted);}
 else if(screen==23){modal("БАНКОМАТ / КАРТА","Снять наличные");SN(b,sizeof(b),"На карте %d руб. / наличные %d руб.",g.bank,g.money);ut(42,124,b,.57f,white);SN(b,sizeof(b),"Экономика %.0f%% / комиссия %.1f%%",g.economy*100,g.atmFee);ut(42,171,b,.46f,accent);ut(42,214,"Комиссия удерживается из суммы списания с карты.",.42f,muted);inv_btn(42,264,220,"Списать 100 руб.",50,100);inv_btn(286,264,220,"Списать 500 руб.",50,500);inv_btn(530,264,220,"Снять всё",50,0);SN(b,sizeof(b),"При снятии всего: на руки %d / удержано %d руб.",g.bank-(int)(g.bank*g.atmFee/100+.5f),(int)(g.bank*g.atmFee/100+.5f));ut(42,327,b,.40f,white);if(noticeTime>0)ut(42,373,notice,.31f,accent);}
 else if(screen==24){modal("ПАДЕНИЕ / ИСТОЩЕНИЕ / ВОДА","Ты погиб");ut(42,132,"Врач доставит тебя домой. Потеряно 10% наличных.",.50f,white);ut(42,179,"Деньги на карте и вещи в рюкзаке сохранены.",.43f,muted);SN(b,sizeof(b),"Смертей: %d / карта: %d руб.",g.deaths,g.bank);ut(42,226,b,.50f,accent);inv_btn(42,294,420,"Очнуться дома / Enter",1,13);}

 else if(screen==15){modal("Дом / питомец","Хрюкот");SN(b,sizeof(b),"Сытость: %d / 100   ·   Корм: %d",(int)g.petHunger,g.feed);ut(42,130,b,.58f,white);ut(42,182,g.petHunger>=99.5f?"Кот сыт. Лишний корм не расходуется.":g.feed?"Можно покормить или погладить.":g.homeFeedTaken?"Корм закончился. Ещё можно купить в магазине.":"Пакет корма лежит дома. Ещё можно купить в магазине.",.43f,muted);inv_btn(42,244,340,"Покормить",33,0);inv_btn(414,244,340,"Погладить",33,1);if(noticeTime>0)wrapped(42,306,notice);}
 else if(screen==7){modal("Работа","Бригада");SN(b,sizeof(b),"В бригаде: %d / 3",g.apprentices);ut(42,119,b,.60f,white);SN(b,sizeof(b),"Обучить / %d руб.",12000+g.apprentices*6000);inv_btn(445,114,309,b,1,82);for(int n=0;n<3;n++){int yy=172+n*59;up(42,yy,712,49,0x2d373b,1);SN(b,sizeof(b),n<g.apprentices?"Ученик %d / опыт %.0f":"Свободное место %d",n+1,g.apprenticeXP[n]);ut(56,yy+15,b,.45f,white);if(n<g.apprentices)inv_btn(560,yy+8,181,g.autoWork[n]?"В офис":"На вызовы",1,49+n);}}
 if(screen==3){if(noticeTime>0)ut(42,407,notice,.30f,accent);else if(job_accepted(g.activeJob)){SN(b,sizeof(b),"Принято: %s. Метка ведёт к подъезду и заказанному балкону.",house_label190(g.jobs[g.activeJob].house));ut(42,407,b,.30f,accent);}}
 if(screen==17){modal("Дом / отдых","Сон и новый день");ut(42,119,"Время идёт для всего района. Еда, вода и корм расходуются.",.43f,muted);inv_btn(42,171,340,"Вздремнуть / 2 часа",35,2);inv_btn(414,171,340,"Выспаться / 8 часов",35,8);inv_btn(42,232,340,"До следующего утра / 08:00",35,0);inv_btn(414,232,340,"Пропустить сутки / 24 часа",35,24);ut(42,301,"После долгого сна проснёшься голодным и захочешь пить.",.43f,white);ut(42,334,"Счета начислятся. Принятые заказы могут просрочиться.",.40f,accent);}
 if(screen==18){modal("Площадка западнее офиса","Приём стекла и металла");SN(b,sizeof(b),"Стеклотара: %d шт.  /  10 руб. за штуку",g.bottles);ut(42,130,b,.53f,white);SN(b,sizeof(b),"Металлолом: %d шт.  /  18 руб. за штуку",g.scrap);ut(42,178,b,.53f,white);inv_btn(42,232,340,"Сдать всё стекло",39,7);inv_btn(414,232,340,"Сдать весь металл",39,8);SN(b,sizeof(b),"Сдать всё / получить %d руб.",g.bottles*10+g.scrap*18);inv_btn(42,294,712,b,39,0);if(noticeTime>0)wrapped(42,345,notice);}
 if(screen==16){modal("Чужая квартира","Кошелёк на столе");ut(42,131,"Можно взять деньги, но это кража.",.60f,white);ut(42,184,"Если заметят — испортишь репутацию. Пропажу могут найти позже.",.39f,muted);inv_btn(42,260,340,"Взять деньги",34,0);inv_btn(414,260,340,"Не трогать",34,1);}
 if(screen==2&&noticeTime>0)ut(42,387,notice,.28f,muted);
 if((screen==4||screen==5||screen==7||screen==9)&&noticeTime>0)ut(42,101,notice,.29f,accent);
 float fa206=ui_fade_alpha();if(fa206>.004f){overlayAlpha=fa206;up(0,0,W,H,0x05070a,fa206);overlayAlpha=1;}
 ui_cursor();
 if(screen>2&&screen!=4&&screen!=5&&screen!=10&&screen!=22){inv_btn(648,397,110,"Назад",1,27);}
 if(help){uiCount=0;modal("Справка","Как играть");const char*l[]={"Наводись на ручки окон, двери и предметы.","Взаимодействуй мышью. Меню не требует горячих клавиш.","В квартире есть кот, корм и стационарное радио.","Вещи покупаются в ларьке или выдаются в точках сбора.","Подработки необязательны: жители живут своей жизнью.","Подробное управление записано в README_RU.md."};for(int i=0;i<6;i++)ut(42,124+i*32,l[i],.46f,white);inv_btn(648,397,110,"Закрыть",1,112);}
}
static void ui_mouse_dispatch(void){uiConsumed=0;uiInjected=-1;
 if(invDrag>=0){if(screen!=4&&screen!=5){invDrag=-1;}else if(!keys[1]&&!pressed[1]){int from=invDrag;invDrag=-1;if(uiBuiltScreen==screen){for(int n=0;n<uiCount;n++){UIButton*b=&uiButtons[n];if(uiMouseX>=b->x&&uiMouseX<b->x+b->w&&uiMouseY>=b->y&&uiMouseY<b->y+b->h){if(b->action==20)inv_move(from,b->param);else if(b->action==26&&g.invSlots[from]>=1&&(g.invSlots[from]<=6||g.invSlots[from]==9))g.quick[b->param]=g.invSlots[from];break;}}}uiConsumed=1;}}
if(uiBuiltScreen!=screen||uiBuiltHelp!=help)return;if(screen==0&&!help&&mouseLook&&!keys[18])return;
 if(uiWheel&&screen==10){dialogScroll=(int)clamp(dialogScroll+uiWheel*3,0,380);}else if(uiWheel&&screen==11)g.mapZoom=clamp(g.mapZoom+uiWheel*.2f,1,3);else if(uiWheel&&screen==6){change_option(uiWheel>0?1:-1);}
 if(pressed[2]&&(screen==4||screen==5)){for(int n=0;n<uiCount;n++){UIButton*b=&uiButtons[n];if(b->action==20&&uiMouseX>=b->x&&uiMouseX<b->x+b->w&&uiMouseY>=b->y&&uiMouseY<b->y+b->h){invSelected=b->param;invDrag=-1;invDropConfirm=0;uiConsumed=1;return;}}}
 if(!pressed[1])return;
 if(uiSoundOn)audio(5);
 for(int n=uiCount-1;n>=0;n--){UIButton*b=&uiButtons[n];if(uiMouseX<b->x||uiMouseX>b->x+b->w||uiMouseY<b->y||uiMouseY>b->y+b->h)continue;uiConsumed=1;switch(b->action){case 61:net_host();break;case 62:net_join();break;case 63:net_disconnect();break;case 60:navService=b->param;navOffice=b->param==0;navHouse=-1;mapGoal=-99;say("Маршрут выбран. Esc — вернуться в игру.");break;case 47:navOffice=1;navService=0;navHouse=-1;mapGoal=-99;screen=0;break;case 45:inv_use(g.quick[b->param]);screen=0;break;case 46:invFullInfo=!invFullInfo;break;case 40:biz_upgrade(b->param);break;
 case 41:if(!biz_invest(b->param))say("Нужны свободные 500 рублей.");break;
 case 42:if(!biz_collect())say("Пока нет денег к получению.");break;
 case 43:home_upgrade();break;
 case 44:{int ok=b->param==0?tv_paste():b->param==1?tv_import_file():tv_open();say(ok?(b->param==2?"Видео открыто в браузере.":"Ссылка сохранена."):"Не удалось. Нужна корректная HTTPS-ссылка YouTube.");break;}
 case 32:if(job_accepted(b->param)){g.activeJob=b->param;navService=0;navHouse=g.jobs[b->param].house;navOffice=0;mapGoal=-99;screen=11;}else{accept_job(b->param);mapGoal=-99;}break;
 case 35:{float hours=b->param?b->param:8-g.hours;if(hours<=0)hours+=24;if(player_sleep(hours))screen=0;break;}
 case 36:navService=b->param;navOffice=0;navHouse=-1;mapGoal=-99;screen=0;say(b->param==1?"Приёмщик отмечен на экране и карте.":"Маршрут домой отмечен.");break;
 case 39:recycle_sell(b->param);break;
 case 34:if(!b->param)steal_cash(interactIndex);screen=0;break;
 case 33:if(b->param)pet_cat();else feed_cat();break;
 case 30:mapCX=b->param?PANEL_X:hx[0];mapCZ=b->param?PANEL_Z:hz[0];break;
 case 52:pressed[13]=1;uiInjected=13;break;
 case 50:bank_withdraw(b->param);break;
 case 31:lift_request(liftUI,b->param);screen=0;break;
 case 20:invSelected=b->param;invDrag=b->param;invDropConfirm=0;break;
 case 21:inv_use(g.invSlots[invSelected]);break;
 case 22:if(invDropConfirm)inv_discard(g.invSlots[invSelected]);else invDropConfirm=1;break;
 case 23:inv_sell(g.invSlots[invSelected],0);break;
 case 24:inv_sell(g.invSlots[invSelected],1);break;
 case 25:shopbuy(b->param);break;
 case 26:inv_use(g.quick[b->param]);break;
 case 27:navService=0;navHouse=dialogNPC<NN?PERSON(dialogNPC).home:NH+g.locals190[dialogNPC-NN].home;navOffice=0;mapGoal=-99;say("Дом отмечен на схеме.");break;
 case 70:qol_set_volume(b->param,(int)clamp((uiMouseX-volTrackX[b->param])*100.f/volTrackW[b->param],0,100));break;case 71:qol_toggle_fullscreen();break;case 1:pressed[b->param]=1;uiInjected=b->param;break;case 2:screen=b->param;break;case 3:optionsRow=b->param/2;change_option(b->param%2?1:-1);break;case 4:optionsTab=b->param;optionsRow=0;break;case 6:g.mapZoom=clamp(g.mapZoom+b->param*.25f,1,3);break;case 10:optionsRow=b->param;if(optionsTab==2){int bx=34+(b->param/6)*370+150;int v=(int)clamp((uiMouseX-bx)*100.f/104.f,0,100);if(b->param==4)qol_set_volume(1,v);else if(b->param<3)qol_set_volume(b->param,v);}break;case 11:dialog_choose(b->param);break;case 12:mapCX=b->param==0?g.x:b->param==1?OX:shopX;mapCZ=b->param==0?g.z:b->param==1?OZ:shopZ;break;}return;}
 if(screen==11&&uiMouseX>=150&&uiMouseX<=650&&uiMouseY>=112&&uiMouseY<=388){float x=mapCX+(uiMouseX-400)*384/(500*g.mapZoom),z=mapCZ+(uiMouseY-250)*288/(276*g.mapZoom),best=1e20f;for(int h=0;h<NH;h++){float d=dst(x,z,hx[h],hz[h]);if(d<best){best=d;navHouse=h;}}for(int p=0;p<PANEL_COUNT;p++){float d=dst(x,z,panel_origin_x(p),panel_origin_z(p));if(d<best){best=d;navHouse=NH+p;}}navService=0;navOffice=0;mapGoal=-99;uiConsumed=1;}
}
static void ui_mouse_end(void){if(uiInjected>=0)pressed[uiInjected]=0;uiWheel=0;}
