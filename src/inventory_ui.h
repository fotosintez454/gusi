/* PDA / distressed olive & amber. All coordinates in logical 800x450. */
static void inv_icon(int x,int y,int id){U32 paper=0xe1c990,green=0x769275;
 if(id==2||id==6||id==7){ur(x+11,y+5,9,23,id==7?0x769275:id==6?0x967750:0x7faaa8);ur(x+13,y+1,5,6,paper);ur(x+11,y+14,9,7,paper);}
 else if(id==4){ur(x+5,y+7,23,19,0x9a9a7b);ur(x+14,y+9,5,15,0xe7d6ab);ur(x+9,y+14,15,5,0xe7d6ab);}
 else if(id==5){ur(x+7,y+5,19,23,paper);ur(x+7,y+5,19,7,0x8d5c41);ur(x+10,y+17,13,2,0x504a36);}
 else if(id==8){ur(x+5,y+13,24,7,0x8f9891);ur(x+8,y+5,6,22,0xb1aa91);}
 else if(id){ur(x+6,y+7,22,20,id==3?green:paper);ur(x+6,y+10,22,5,0x6e6749);}}
static void inv_panel(int x,int y,int w,int h){up(x,y,w,h,0x171c16,.98f);ur(x,y,w,1,0x7e744e);ur(x,y+h-1,w,1,0x4e4c34);ur(x,y,1,h,0x7e744e);ur(x+w-1,y,1,h,0x4e4c34);for(int i=0;i<4;i++)ur(x+(i%2?w-5:3),y+(i/2?h-5:3),2,2,0xab9565);}
static void inv_btn(int x,int y,int w,const char*label,int action,int param){int hover=uiMouseX>=x&&uiMouseX<x+w&&uiMouseY>=y&&uiMouseY<y+31;up(x,y,w,31,hover?0x55583b:0x303728,1);ur(x,y,w,1,hover?0xe6cf95:0x82764e);ut(x+9,y+9,label,.40f,hover?0xffedbc:0xd8c896);ui_hit(x,y,w,31,action,param);}
static void inv_page(int trade){
 inv_sync();char b[200];up(0,0,800,450,0x0b100c,.91f);inv_panel(18,16,764,419);ur(34,33,4,45,accent);ut(48,32,trade?"ТОРГОВЛЯ / НАЛИЧНЫЙ РАСЧЁТ":"ЛИЧНОЕ ДЕЛО / СНАРЯЖЕНИЕ",.34f,accent);ut(46,49,trade?"ЛАРЁК «У ДОМА»":"ВЕЩМЕШОК",.94f,white);SN(b,sizeof(b),"%d руб.",g.money);ut(619,49,b,.64f,accent);ur(34,91,732,1,0x756b48);
 ut(35,102,"РЮКЗАК / 6 × 8",.34f,muted);
 for(int n=0;n<48;n++){int x=34+n%6*36,y=123+n/6*34,id=g.invSlots[n],sel=invSelected==n;up(x,y,34,32,sel?0x484a30:0x232a20,1);ur(x,y,34,1,sel?accent:0x51573d);if(id){inv_icon(x,y,id);SN(b,sizeof(b),"%d",inv_count(id));ut(x+20,y+22,b,.24f,white);}ui_hit(x,y,34,32,20,n);}
 SN(b,sizeof(b),"%.1f / 25 кг   %.1f / 40 л",inv_weight(),inv_volume());ut(34,401,b,.33f,inv_weight()>20?0xe7a16a:accent);
 inv_panel(265,123,260,190);int id=g.invSlots[invSelected];ut(280,137,id?invNames[id]:"Свободная ячейка",.56f,white);if(id){inv_icon(281,170,id);SN(b,sizeof(b),"В стопке: %d",inv_count(id));ut(329,176,b,.4f,accent);SN(b,sizeof(b),"Вес %.2f кг / объём %.2f л",invWeights[id],invVolumes[id]);ut(280,215,b,.34f,muted);SN(b,sizeof(b),"Выкуп: %d руб. за шт.",invPrices[id]);ut(280,239,b,.37f,accent);ut(280,276,id>=7?"Вторсырьё. Сдаётся в ларьке.":id==5?"Снижает стресс. Вредит здоровью.":"Расходник. Используй по необходимости.",.29f,muted);}else{ut(280,190,"Перетащи сюда предмет.",.40f,muted);ut(280,221,"ЛКМ: выбрать / удерживать: перенос",.31f,muted);}
 if(trade){inv_panel(538,123,226,272);ut(551,137,"ПРИЛАВОК",.45f,accent);int ids[]={1,2,4,5,6,3};const char*labels[]={"Еда  /  35 руб.","Вода  /  20 руб.","Аптечка  /  95 руб.","Сигареты ×5 / 60 руб.","Пиво  /  75 руб.","Корм  /  45 руб."};for(int k=0;k<6;k++)inv_btn(551,163+k*34,199,labels[k],25,ids[k]-1);ut(551,373,"Тара 10 руб. / металл 18 руб.",.29f,muted);}
 else{inv_panel(538,123,226,272);ut(551,138,"БЫСТРЫЙ ДОСТУП",.43f,accent);for(int k=0;k<2;k++){int yy=166+k*57;up(551,yy,199,49,0x2c3427,1);inv_icon(556,yy+7,g.quick[k]);ut(597,yy+8,invNames[g.quick[k]],.38f,white);SN(b,sizeof(b),"В запасе: %d",inv_count(g.quick[k]));ut(597,yy+29,b,.3f,accent);ui_hit(551,yy,199,49,26,k);}ut(551,290,"Перетащи расходник в быстрый слот.",.29f,muted);ut(551,315,"ЛКМ по слоту — использовать.",.30f,muted);ut(551,351,"От 20 кг движение медленнее.",.30f,muted);ut(551,373,"Лимит: 25 кг / 40 литров.",.31f,accent);}
 inv_btn(265,325,125,"Использовать",21,0);inv_btn(400,325,125,invDropConfirm?"Точно? −1":"Выбросить −1",22,0);inv_btn(265,367,125,"Продать −1",23,0);inv_btn(400,367,125,"Всю стопку",24,0);
 inv_btn(653,399,111,"Назад",1,27);
 if(invDrag>=0&&g.invSlots[invDrag]){int x=(int)uiMouseX+10,y=(int)uiMouseY+10;up(x,y,42,40,0x4b5035,.95f);inv_icon(x+3,y+3,g.invSlots[invDrag]);}
}
static void bonus_widget(void){if(!bonusActive)return;up(254,286,292,81,0x171c16,.98f);ut(267,297,bonusId>=72?"ДВЕРЬ / БЫСТРАЯ ПРОВЕРКА":"ОКНО / БЫСТРАЯ ПРОВЕРКА",.41f,accent);ur(267,322,266,10,0x394331);ur(267+(int)(266*(.56f-bonus_width()/2)),322,(int)(266*bonus_width()),10,0x9fba76);ur(267+(int)(266*clamp(bonusClock/bonus_duration(),0,1)),318,3,18,white);ut(267,344,"ЛКМ в светлом секторе. Промах без штрафа.",.30f,white);}
