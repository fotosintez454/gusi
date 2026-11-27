/* v0.20.2: transient ride dynamics and seasonal footprints. Save layout unchanged. */
static void reset202(void){rideIndex202=-1;renderAhead202=0;sprintLocked202=0;staminaDelay202=0;memset(ridePhase202,0,sizeof(ridePhase202));memset(rideAmp202,0,sizeof(rideAmp202));memset(rideOmega202,0,sizeof(rideOmega202));}
static void tracks_tick202(float dt){
 if(g.season!=0||!g.winter){g.trackHead190=0;g.clean190[0]=g.clean190[1]=0;return;}
 int count=(int)clamp(g.trackHead190,0,TRACK190);float age=dt*(g.weather==1?4:1);
 for(int n=0;n<count;n++)g.tracks190[n].age=mn(241,g.tracks190[n].age+age);
}
static void ride_position202(int n,float ahead,float*x,float*y,float*z){Prop150*p=&props150[n];int k=n-53;float phase=ridePhase202[k]+(p->type==8?2:rideOmega202[k])*ahead;
 if(p->type==8){float a=(float)sin(phase)*rideAmp202[k];*x=p->x;*y=2.4f-(float)cos(a)*1.7f+.045f;*z=p->z+(float)sin(a)*1.7f;}
 else{*x=p->x+(float)cos(phase+PI/4)*.62f;*y=.665f;*z=p->z+(float)sin(phase+PI/4)*.62f;}
}
static int ride_start202(int n){if(n<53||n>58||!(props150[n].type==8||props150[n].type==9))return 0;if(held200>=0){say("Сначала отпусти предмет клавишей G.");return 0;}rideIndex202=n;seated=3000+n;yaw=props150[n].type==8?0:-ridePhase202[n-53]-PI*.75f;pitch=0;g.rot=yaw;g.look=pitch;g.zoneFlags150|=1u<<(16+n-53);g.fallVelocity=0;jump180=jumpV180=0;moveVelocityX=moveVelocityZ=0;ride_position202(n,0,&g.x,&g.playerY,&g.z);say("Ты сел. E — встать, Пробел — раскачать или остановить.");return 1;}
static int ride_exit202(void){if(rideIndex202<0)return 0;Prop150*p=&props150[rideIndex202];float ox=g.x,oy=g.playerY,oz=g.z;int seat=seated;seated=-1;g.playerY=0;
 for(int k=0;k<24;k++){float a=k*PI/12,r=mx(p->rx,p->rz)+.65f;float x=p->x+(float)cos(a)*r,z=p->z+(float)sin(a)*r;
 if(terrain_height141(x,z)<-.1f||!body_clear150(x,z,0,.23f,1))continue;
 g.x=x;g.z=z;g.playerY=terrain_height141(x,z);g.fallVelocity=0;moveVelocityX=moveVelocityZ=0;g.zoneFlags150&=~(1u<<(16+rideIndex202-53));rideIndex202=-1;say("Ты встал. Аттракцион плавно останавливается.");return 1;}
 g.x=ox;g.z=oz;g.playerY=oy;seated=seat;say("Выход занят. Подожди, пока проход освободится.");return 0;
}
static int ride_controls202(float dt){if(rideIndex202<0)return 0;if(pressed['E']){ride_exit202();pressed['E']=0;return 1;}if(pressed[32]){g.zoneFlags150^=1u<<(16+rideIndex202-53);pressed[32]=0;}stamina_tick202(dt,0,0);return 1;}
static void ride_tick202(float dt){props_init150();for(int k=0;k<6;k++){int n=53+k;float on=play_on151(n)?1:0;ridePhase202[k]+=(props150[n].type==9?rideOmega202[k]:2)*dt;
 if(ridePhase202[k]>2*PI)ridePhase202[k]-=2*PI;
 rideAmp202[k]+=clamp(on*.55f-rideAmp202[k],-dt*.16f,dt*.22f);rideOmega202[k]+=clamp(on*.8f-rideOmega202[k],-dt*.24f,dt*.30f);}
 if(rideIndex202>=0){ride_position202(rideIndex202,0,&g.x,&g.playerY,&g.z);if(props150[rideIndex202].type==9){yaw-=rideOmega202[rideIndex202-53]*dt;g.rot=yaw;}}
}
static void ride_camera202(void){if(rideIndex202<0)return;float x,y,z;ride_position202(rideIndex202,renderAhead202,&x,&y,&z);camx=x;camy=y+player_eye180();camz=z;
 if(props150[rideIndex202].type==9)yaw-=rideOmega202[rideIndex202-53]*renderAhead202;
 else pitch+=(float)sin(ridePhase202[rideIndex202-53]+2*renderAhead202)*rideAmp202[rideIndex202-53]*.10f;
}
/* One-based floor indicator shared by HUD and the real cabin displays. */
static int lift_readout202(int*current,int*target,int*moving,int*maxFloor){float y=0,t=0;int found=0,top=5;
 for(int h=0;h<NH&&!found;h++)if(residence_cabin(h,g.x,g.z)&&ab(g.playerY-g.residence[h].y)<.5f){y=g.residence[h].y;t=g.residence[h].target;found=1;}
 for(int p=0;p<PANEL_COUNT&&!found;p++)for(int w=0;w<2&&!found;w++)if(ab(g.x-(panel_origin_x(p)+(w?7:-7)-3.5f))<1.05f&&ab(g.z-(panel_origin_z(p)-6))<1&&ab(g.playerY-panelRuntime[p].y[w])<.5f){y=panelRuntime[p].y[w];t=panelRuntime[p].target[w];top=4;found=1;}
 for(int n=0;n<4&&!found;n++)if(ab(g.x-cityBlockX[n])<.6f&&ab(g.z-(cityBlockZ[n]-5.7f))<.6f&&ab(g.playerY-g.highY180[n])<.5f){y=g.highY180[n];t=g.highTarget180[n];top=high_floors180(n);found=1;}
 *current=(int)clamp((float)floor((y+1.5f)/3)+1,1,top);*target=(int)(t/3+.5f)+1;*moving=ab(t-y)<.02f?0:t>y?1:-1;*maxFloor=top;return found;
}
