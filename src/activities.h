/* Opt-in local microtasks, separate from the window-opening state machine. */
static int qteCount=0,qteSuccess=0;static float qteTimeTotal=0;
static int microOffer=0,microActive=0,microKind=0,microHouse=1,microHits=0,microMisses=0,microCompleted=0;
static float microNext=30,microClock=0,microCenter=.6f,microWidth=.35f,microFeedback=0;
static const char*micro_names[]={"Картошка для жильца","Проверить запасную защёлку","Посылка для соседа"};
static int micro_rounds(void){return microKind==0?3:2;}
static float micro_duration(void){return clamp(.78f+.05f*skill_effect(skillLevel),.4f,1.f);}
static void micro_position(float*x,float*z){*x=hx[microHouse];*z=hz[microHouse];if(microKind==0){*x-=3;*z-=1.7f;}else if(microKind==1){*x-=2.8f;*z+=3.1f;}else *z-=5.8f;}
static int micro_near(void){float x,z;micro_position(&x,&z);return dst(g.x,g.z,x,z)<2.5f&&visible(g.x,g.z,x,z);}
static void micro_new_round(void){microClock=0;microCenter=.5f+(rnd()-.5f)*.2f;microWidth=.48f+.09f*skill_effect(skillLevel)+mn(.04f*microMisses,.20f);}
static void micro_finish(void){if(!microActive)return;microActive=microOffer=0;microNext=25+rnd()*10;microCompleted++;int pay=65+microKind*15+(int)(skill_effect(skillLevel)*15);g.salary+=pay;skillLevel+=2;if(microKind==0)g.food++;char b[180];SN(b,sizeof(b),"%s: готово. %d руб. начислено в офисе. Навык +2.",micro_names[microKind],pay);say(b);audio(1);}
static void micro_answer(int success){if(!microActive)return;qteCount++;qteSuccess+=success;qteTimeTotal+=mn(microClock,micro_duration());eventValue=microClock;eventValue=success?(skillLevel>100&&ab(microClock/micro_duration()-microCenter)<.08f?2:1):0;event_log("skill_check_result",100+microHouse);if(success){microHits++;skillLevel+=.2f;audio(5);}else{microMisses++;audio(6);if(skillAssist&&microMisses%3==0)microHits++;}if(microHits>=micro_rounds())micro_finish();else{micro_new_round();microFeedback=.04f;}}
static void micro_play(float dt){if(!microActive)return;if(pressed['T']){microActive=0;say("Помощь отложена. Нажми T у места задания, чтобы вернуться.");return;}if(microFeedback>0){microFeedback=mx(0,microFeedback-dt);return;}microClock+=dt;float phase=microClock/micro_duration(),distance=ab(phase-microCenter);
 if(pressed[32]||pressed[1]||(autoChecks&&distance<microWidth*.35f))micro_answer(distance<microWidth*.5f);else if(phase>1)micro_answer(0);}
static void micro_update(float dt){if(microActive)return;if(!microOffer){microNext-=dt;if(microNext<=0&&opening<0){float best=1e20f;for(int h=1;h<NH;h++){float d=dst(g.x,g.z,hx[h],hz[h]);if(d<best){best=d;microHouse=h;}}microKind=microCompleted%3;microOffer=1;char b[180];SN(b,sizeof(b),"Рядом: %s — %s. T у места задания; подробности в телефоне.",names[microHouse],micro_names[microKind]);say(b);}}
 if(microOffer&&pressed['T']&&opening<0&&micro_near()){microActive=1;microHits=microMisses=0;microFeedback=0;micro_new_round();audio(7);}}

static void reset_activities(void){microOffer=microActive=microKind=microHits=microMisses=microCompleted=0;microHouse=1;microNext=30;microClock=microFeedback=0;}
