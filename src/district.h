/* Shared district dispatch. Runtime selection is restored after every public query. */
static int residence_at(float x,float z){for(int h=0;h<NH;h++)if(((ab(x-hx[h])<5.05f&&(ab(z-hz[h])<4.85f||((ab(x-hx[h]-2.8f)<1.25f||ab(x-hx[h]+2.8f)<1.25f)&&z>hz[h]+4.1f&&z<hz[h]+6.3f)))||(x>hx[h]+4.85f&&x<hx[h]+8.5f&&z>hz[h]-8.6f&&z<hz[h]+3.6f)))return h;return -1;}
static int residence_floor(void){return (int)clamp((float)floor((g.playerY+.15f)/3),0,4);}
static int residence_interact_at(float x,float z){int h=residence_at(x,z);if(h>=0)return h;for(int n=0;n<NH;n++)if(x>hx[n]-7.6f&&x<hx[n]+8.35f&&dst(x,z,hx[n],hz[n]-4.42f)<2.15f)return n;return -1;}
static int residence_cabin(int h,float x,float z){return ab(x-(hx[h]+6.1f))<.83f&&ab(z-(hz[h]-6.f))<.83f;}
static int residence_aligned(int h){ResidenceSave*s=&g.residence[h];return s->wait<=0&&ab(s->y-s->target)<.01f;}
static int residence_solid(int h,float x,float z){float a=x-hx[h],b=z-hz[h];int f=residence_floor();ResidenceSave*s=&g.residence[h];
 for(int k=0;k<5;k++)if(rail_hit203(a,b,g.playerY,6.70f,-1.875f,1.875f,k*3+.09375f,k*3+1.5f)||rail_hit203(a,b,g.playerY,6.50f,-1.875f,1.875f,k*3+3,k*3+1.59375f))return 1;
 /* Enclosed service core: lobby in front, switchback stair behind. */
 if(a>5.05f){
  if(a>8.22f||b>3.30f)return 1;
  if(b< -7.98f)return g.playerY>.3f||a<6.95f;
  if(b< -4.5f&&a<5.15f)return 1;
  if(a<7.25f&&b> -7.2f&&b< -4.8f){if(a<5.0f||b< -7.06f||b> -4.94f)return 1;if(a>7.05f&&(ab(b+6)>.82f||ab(s->y-f*3)>.18f||s->wait>0))return 1;return 0;}
  return 0;
 }
 if(ab(a)>4.68f){if(h==0&&f==0&&a<0&&ab(b-1.3f)<.78f)return 0;if(a>0&&ab(b+3)<.82f&&(s->doors&(1u<<f)))return 0;return 1;}
 if(b>4.33f)return 1;

 if(ab(b+4.42f)<.45f)return 1;
 if(b< -4.12f)return 1;
 
 return 0;
}
static float residence_ground(int h,float x,float z,float old){if(residence_cabin(h,x,z))return ab(old-g.residence[h].y)<.35f?g.residence[h].y:-999;float a=x-hx[h],b=z-hz[h],best=-999,dist=1e20f;

 if(a>5.12f){for(int f=0;f<5;f++){float y=-999;if(b>2&&b<3.35f)y=f*3+1.5f;else if(b>=-2.05f&&b<=2.1f)y=f*3+(a>6.6f?clamp((b+2)/4,0,1)*1.5f:3-clamp((b+2)/4,0,1)*1.5f);if(y>=0&&ab(y-old)<dist){best=y;dist=ab(y-old);}}if(dist<.34f)return best;if(b> -2.05f&&b<3.35f)return ab(old)<.3f?0:-999;}
 for(int f=0;f<5;f++)if(ab(f*3-old)<dist){best=f*3;dist=ab(best-old);}return dist<.34f?best:-999;
}
static int panel_wing(float x,float z){int out=-1;for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;int w=panel_one_wing(x,z);if(w>=0){out=p*2+w;break;}}panelIndex=0;if(out<0&&high_at180(x,z)>=0)return 60+high_at180(x,z);if(out<0){int h=residence_at(x,z);if(h>=0)out=30+h;}return out;}
static int panel_solid(float x,float z){int out=0;for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;if(panel_one_solid(x,z)){out=1;break;}}panelIndex=0;if(out)return out;int h=residence_at(x,z);return h>=0?residence_solid(h,x,z):0;}
static int panel_step(float x,float z){
 float supported202;if(roof_at(x,z,&supported202)&&ab(g.playerY-supported202)<.25f){g.playerY=supported202;return 1;}
 int hi180=high_at180(x,z);if(hi180>=0){float yy=high_ground180(hi180,x,z,g.playerY);if(yy< -900)return 0;g.playerY=yy;return 1;}
 if(g.playerY<.6f&&(river_water(x,z)||river_water(g.x,g.z))){if(!river_water(x,z))g.playerY=terrain_height141(x,z);return 1;}
 float rh;if(roof_at(x,z,&rh)&&ab(g.playerY-rh)<=.2f){g.playerY=rh;return 1;}if(g.fallVelocity<0)return 1;float height=-999;int found=0;for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;if(panel_one_wing(x,z)>=0){height=panel_ground(x,z,g.playerY);found=1;break;}}panelIndex=0;if(!found){int h=residence_at(x,z);if(h>=0)height=residence_ground(h,x,z,g.playerY);else height=ab(g.playerY-terrain_height141(x,z))<.5f?terrain_height141(x,z):-999;}if(height< -900){if(g.playerY>2){if(g.fallVelocity==0){g.fallStart=g.playerY;g.fallVelocity=-.01f;}return 1;}return 0;}g.playerY=height;return 1;}
static void panel_reset(void){memset(panelWindowAngle,0,sizeof(panelWindowAngle));memset(residenceWindowAngle,0,sizeof(residenceWindowAngle));for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;panel_one_reset();}panelIndex=0;}
static void lift_request(int id,int floor){if(id>=30){int h=id-30;if(h<NH){g.residence[h].target=clamp(floor,0,4)*3;g.residence[h].wait=.5f;}return;}if(id<0||id>=30)return;panelIndex=id/2;lift_one_request(id%2,floor);panelIndex=0;}
static void panel_motion(float dt){for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;panel_one_motion(dt);}panelIndex=0;for(int h=0;h<NH;h++){ResidenceSave*s=&g.residence[h];for(int k=0;k<24;k++){float target=(s->windows&(1ull<<k))?1.2f:0;residenceWindowAngle[h][k]+=clamp(target-residenceWindowAngle[h][k],-dt*2.f,dt*2.f);}for(int f=0;f<5;f++){float target=(s->doors&(1u<<f))?1.f:0;residenceDoorAngle[h][f]+=clamp(target-residenceDoorAngle[h][f],-dt*6.f,dt*6.f);}float front=(s->doors&32)?1.f:0;residenceFrontDoorAngle[h]+=clamp(front-residenceFrontDoorAngle[h],-dt*6.f,dt*6.f);}}
static int residence_use(int h,int perform){float a=g.x-hx[h],b=g.z-hz[h];int f=residence_floor();ResidenceSave*s=&g.residence[h];
 if(ab(a-4.9f)<1.8f&&ab(b+3)<1.0f&&look_at(hx[h]+4.9f,f*3+1.3f,hz[h]-3,.80f)){if(perform){int was=(s->doors>>f)&1;s->doors^=1u<<f;nav_refresh(g.x,g.z);event_log(was?"door_closed":"door_opened",1100+h*8+f);event_log("open_door",1100+h*8+f);audio(was?13:0);}else SN(prompt,sizeof(prompt),"Дверь квартиры / открыть или закрыть");return 1;}
 if(f>0)for(int side=0;side<2;side++)for(int k=0;k<3;k++){float wx=hx[h]+window_a180(side,k),wz=hz[h]+(side?4.48f:-4.48f);if(dst(g.x,g.z,wx,wz)<2.55f&&look_at(wx,f*3+1.75f,wz,.94f)){if(perform){int id=(f-1)*6+side*3+k;work_interact(1000+h*24+id);}else SN(prompt,sizeof(prompt),"%s",work_hint(1000+h*24+(f-1)*6+side*3+k));return 1;}}
 return 0;
}
static int panel_interact(void){int out=0;for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;if(panel_one_interact()){out=1;break;}}panelIndex=0;if(!out){int h=residence_interact_at(g.x,g.z);if(h>=0)out=residence_use(h,1);}return out;}
static int panel_prompt(void){int out=0;for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;if(panel_one_prompt()){out=1;break;}}panelIndex=0;if(!out){int h=residence_interact_at(g.x,g.z);if(h>=0)out=residence_use(h,0);}return out;}
static void panel_save(void){for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;DistrictSave*s=&g.district[p];s->doors=flatOpen;s->entries=(s->entries&0x3fffcu)|entryOpen;s->windows=*panel_windows();for(int w=0;w<2;w++){s->y[w]=liftY[w];s->target[w]=liftTarget[w];s->wait[w]=liftWait[w];}}panelIndex=0;g.panelSavedDoors=flatOpen;g.panelSavedEntries=entryOpen;for(int w=0;w<2;w++){g.panelSavedY[w]=liftY[w];g.panelSavedTarget[w]=liftTarget[w];g.panelSavedWait[w]=liftWait[w];}}
static float panel_valid_height(float value){return value>=0?mn(value,(PANEL_FLOORS-1)*3):0;}
static void panel_load(void){if(g.version<12){g.district[0].doors=g.panelSavedDoors;g.district[0].entries=g.panelSavedEntries;g.district[0].windows=g.panelWindowOpen;for(int w=0;w<2;w++){g.district[0].y[w]=g.panelSavedY[w];g.district[0].target[w]=g.panelSavedTarget[w];g.district[0].wait[w]=g.panelSavedWait[w];}}
 for(int p=0;p<PANEL_COUNT;p++){panelIndex=p;DistrictSave*s=&g.district[p];flatOpen=s->doors&1048575u;entryOpen=s->entries&3u;*panel_windows()=s->windows&((1ull<<40)-1);for(int k=0;k<40;k++)panelWindowAngle[p][k]=(*panel_windows()&(1ull<<k))?1.25f:0;for(int k=0;k<20;k++)flatAngles[k]=(flatOpen>>k)&1;for(int w=0;w<2;w++){liftY[w]=panel_valid_height(s->y[w]);liftTarget[w]=panel_valid_height(s->target[w]);liftWait[w]=clamp(s->wait[w],0,2);liftDoor[w]=ab(liftY[w]-liftTarget[w])<.01f;}}panelIndex=0;
 for(int h=0;h<NH;h++){ResidenceSave*s=&g.residence[h];s->y=clamp(s->y,0,12);s->target=clamp(s->target,0,12);s->wait=clamp(s->wait,0,2);s->doors&=7999;s->windows&=(1ull<<24)-1;for(int k=0;k<24;k++)residenceWindowAngle[h][k]=(s->windows&(1ull<<k))?1.2f:0;for(int f=0;f<5;f++)residenceDoorAngle[h][f]=(s->doors&(1u<<f))?1.f:0;residenceFrontDoorAngle[h]=(s->doors&32)?1.f:0;}if(panel_wing(g.x,g.z)>=0&&panel_wing(g.x,g.z)<30&&g.playerY>12)g.playerY=12;else if(high_at180(g.x,g.z)>=0){int n=high_at180(g.x,g.z);g.playerY=clamp(g.playerY,0,high_floors180(n)*3);}
 else if(!(g.playerY>=-3&&g.playerY<=15.4f))g.playerY=0;
}
