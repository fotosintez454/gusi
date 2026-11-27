/* Spatial and gameplay contracts for the v141 architectural rebuild. */
static int outside141(float x,float z,float m){
 for(int h=0;h<NH;h++){float a=x-hx[h],b=z-hz[h];if(ab(a)<5.2f+m&&b> -4.7f-m&&b<6.6f+m)return 0;if(a>4.7f-m&&a<8.6f+m&&b> -8.7f-m&&b<3.7f+m)return 0;}
 if(x>hx[0]-14.5f-m&&x<hx[0]-4.8f+m&&ab(z-hz[0])<4.6f+m)return 0;
 for(int p=0;p<PANEL_COUNT;p++)if(ab(x-panel_origin_x(p))<14.3f+m&&ab(z-panel_origin_z(p))<9.2f+m)return 0;
 for(int n=0;n<4;n++)if(ab(x-cityBlockX[n])<8+m&&ab(z-cityBlockZ[n])<5.5f+m)return 0;
 if(ab(x-OX)<8.3f+m&&ab(z-OZ)<6+m)return 0;if(ab(x-shopX)<5.6f+m&&ab(z-shopZ)<6.4f+m)return 0;
 return 1;
}
static void bench_position141(int n,float*x,float*z){if(n<24){*x=hx[n]+2.65f;*z=hz[n]-6.6f;}else{int p=(n-24)/2,w=(n-24)%2;*x=panel_origin_x(p)+(w?7:-7)+3.9f;*z=panel_origin_z(p)-10.25f;}}
static float terrain_height141(float x,float z){if(ab(x)<143&&ab(z-river_z(x))<5){if(bridge_at202(x,z))return .36f;if(river_water(x,z))return g.winter?-.18f:-2.55f;}return 0;}
/* v151: explicit dive intent + submerged standing eye. No crouch/stamina coupling. */
static int underwater151(void){return !g.winter&&river_water(g.x,g.z)&&g.playerY+1.64f<-.42f;}
static void water_tick141(float dt){
 if(!river_water(g.x,g.z)||g.playerY>.6f||g.winter){
  if(river_water(g.x,g.z)&&g.winter&&g.playerY<.6f)g.playerY=-.18f;
  else if(!river_water(g.x,g.z)&&g.playerY<0)g.playerY=terrain_height141(g.x,g.z);
  swimAir141=mn(100,swimAir141+dt*35);g.drowning=0;swimBlend141=mx(0,swimBlend141-dt*3);return;
 }
 swimBlend141=mn(1,swimBlend141+dt*2);
 int dive=keys[17]&&!keys[32];
 float target=dive?-2.28f:keys[32]?-1.20f:-1.48f;
 target+=(float)sin(realTime*2.3f)*.025f;
 g.playerY=lerp(g.playerY,target,mn(1,dt*4));g.fallVelocity=0;g.x+=dt*.08f;
 g.stamina=mx(0,g.stamina-dt*(dive?2.0f:.25f));
 if(dive&&underwater151()){swimAir141=mx(0,swimAir141-dt*10);g.drowning+=dt;}
 else{swimAir141=mn(100,swimAir141+dt*35);g.drowning=0;}
 if(swimAir141<=0&&dive&&underwater151())g.health=mx(0,g.health-dt*12);
 if(g.health<=0)player_die();
}
static int bench_route141(int i,float dt){
 int elder=npc_elder141(i),sched=schedule150(i),wanted=(sched==12&&g.hours>=9&&g.hours<20)||(sched==4&&i%8==3&&g.hours>=12&&g.hours<16);if(!wanted){npcBench141[i]=-1;return 0;}
 Citizen*c=&PERSON(i);int n=c->home;float bx,bz;bench_position141(n,&bx,&bz);float tx=bx+(i/NH?.58f:-.58f),tz=bz-.22f;
 int fresh=c->activity!=12;if(fresh){citizenLen[i]=citizenStep[i]=0;lifeThink[i]=0;}c->activity=12;c->tx=tx;c->tz=tz;float d=dst(c->x,c->z,tx,tz);
 if(d<3.6f){ignoreBench141=n;float st=mn(d,dt*1.1f),xx=c->x+(tx-c->x)*st/mx(.001f,d),zz=c->z+(tz-c->z)*st/mx(.001f,d);if(nav_clear(xx,zz)){c->x=xx;c->z=zz;}else{if(nav_clear(xx,c->z))c->x=xx;if(nav_clear(c->x,zz))c->z=zz;}
 ignoreBench141=-1;npcBench141[i]=n;if(d<.10f){sitting141[i]=mn(1,sitting141[i]+dt*2.5f);npcMoving[i]=0;npcGait[i]=0;c->facing=PI;c->wait=8;g.lifeEnergy[i]=mn(100,g.lifeEnergy[i]+dt*.07f);}else{sitting141[i]=mx(0,sitting141[i]-dt*3);npcMoving[i]=1;npcGait[i]=1;walkPhase[i]+=st*5;}return 1;}
 npcBench141[i]=-1;sitting141[i]=0;
 if(citizenLen[i]<=0||citizenStep[i]>=citizenLen[i]){if(lifeThink[i]<=0){citizenLen[i]=nav_plan(c->x,c->z,bx,bz-1.15f,citizenPath[i]);citizenStep[i]=0;lifeThink[i]=1.5f;}}
 if(citizenStep[i]<citizenLen[i]){int node=citizenPath[i][citizenStep[i]];float x=nav_x(node),z=nav_z(node),dd=dst(c->x,c->z,x,z),st=mn(dd,dt*1.2f);float xx=c->x+(x-c->x)*st/mx(.001f,dd),zz=c->z+(z-c->z)*st/mx(.001f,dd);if(nav_clear(xx,zz)){c->x=xx;c->z=zz;npcMoving[i]=1;npcGait[i]=1;walkPhase[i]+=st*5;float a=(float)atan2(x-c->x,z-c->z);c->facing=a;}if(dd<.15f)citizenStep[i]++;}
 return 1;
}
static int furniture141(float x,float z){return furniture_solid150(x,z);}
static int roof_solid141(float x,float z){for(int n=0;n<4;n++)if(g.playerY>=high_floors180(n)*3-.1f&&g.playerY<high_floors180(n)*3+2.05f&&ab(x-cityBlockX[n])<1.16f&&z-cityBlockZ[n]>-6.91f&&z-cityBlockZ[n]<-4.49f)return 1;
 for(int h=0;h<NH;h++)if(g.playerY>=14.95f&&g.playerY<17.9f){float a=x-hx[h],b=z-hz[h];if(a>4.7f&&a<8.6f&&b> -4.7f&&b<3.6f){if(a>8.20f||b>3.25f||b< -4.25f||(a<5.06f&&(b< -3.84f||b> -2.16f)))return 1;}}
 for(int p=0;p<PANEL_COUNT;p++)if(g.playerY>=11.95f&&g.playerY<14.80f)for(int w=0;w<2;w++){float a=x-(panel_origin_x(p)+(w?7:-7)),b=z-panel_origin_z(p);if(ab(a)<1.9f&&b>.35f&&b<8.1f){if(ab(a)>1.55f||b>7.7f||(ab(b-.6f)<.2f&&ab(a)>.64f))return 1;}}
 return 0;
}
