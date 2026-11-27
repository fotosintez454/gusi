/* v180: explorable 9/12 storey houses replace the four sealed skyline boxes.
   All plan rectangles below are shared by collision and the drawing module. */
static int high_at180(float x,float z){for(int n=0;n<4;n++)if((ab(x-cityBlockX[n])<8.10f&&ab(z-cityBlockZ[n])<5.4f)||((ab(ab(x-cityBlockX[n])-4.8f)<1.65f)&&z>=cityBlockZ[n]+5.4f&&z<cityBlockZ[n]+7.1f)||(ab(x-cityBlockX[n])<1.95f&&z>cityBlockZ[n]-7.8f&&z<=cityBlockZ[n]-5.4f))return n;return -1;}
static int high_floor180(int n){return (int)clamp((float)floor((g.playerY+.13f)/3),0,high_floors180(n)-1);}
static int high_lit180(int n,int f,int side){unsigned bit=1u<<(f*2+side);if(g.highLightTouched180[n]&bit)return !(g.highLightOff180[n]&bit);return flat_lit180(NH+15+n,f,side);}
static U32 high_color180(int n,int f,int side){return flat_color180(NH+15+n,f,side);}
static int high_part180(int k,float*a,float*b,float*rx,float*rz){
 static const float rect[5][4]={{5.85f,-4.65f,.10f,.50f},{5.85f,-2.75f,.10f,.40f},{6.825f,-2.35f,.975f,.10f},{2.875f,.65f,1.075f,.10f},{6.425f,.65f,1.375f,.10f}};
 if(k<0||k>=5)return 0;*a=rect[k][0];*b=rect[k][1];*rx=rect[k][2];*rz=rect[k][3];return 1;
}
static int high_part190(int n,int k,float*a,float*b,float*rx,float*rz){if(!high_part180(k,a,b,rx,rz))return 0;if(n>=2){if(k==3){*a=2.475f;*rx=.675f;}if(k==4){*a=6.025f;*rx=1.775f;}}return 1;}
static int high_prop180(int k,float*a,float*b,float*rx,float*rz,float*height){
 static const float p[9][5]={{3.0f,3.3f,.55f,.95f,.65f},{7.15f,2.5f,.38f,.65f,2.05f},{2.70f,-4.4f,.36f,.38f,1.85f},{6.6f,-1.1f,.65f,.45f,.80f},{6.45f,-4.5f,.3f,.35f,.85f},{7.25f,-3.65f,.4f,.85f,.58f},{6.40f,-2.90f,.25f,.38f,.91f},{4.1f,-4.6f,.60f,.35f,.90f},{5.12f,-4.6f,.32f,.35f,.90f}};
 if(k<0||k>=9)return 0;*a=p[k][0];*b=p[k][1];*rx=p[k][2];*rz=p[k][3];*height=p[k][4];return 1;
}
static int high_solid180(float x,float z){int n=high_at180(x,z);if(n<0)return 0;float a=x-cityBlockX[n],b=z-cityBlockZ[n],aa=ab(a);int f=high_floor180(n),side=a>0;float yy=g.playerY-f*3;
 for(int k=0;k<high_floors180(n)-1;k++)if(rail_hit203(a,b,g.playerY,-.21f,-1.875f,1.875f,k*3+.09375f,k*3+1.5f)||rail_hit203(a,b,g.playerY,.21f,-1.875f,1.875f,k*3+3,k*3+1.59375f))return 1;
 if(g.playerY>=high_floors180(n)*3-.15f)return aa>7.65f||b>5.08f||b< -6.9f;
 if(b< -5.2f){if(aa>1.8f)return 1;if(aa>1.68f)return 1;if(b< -7.50f)return f>0||aa>.66f||!(g.highEntry180&(1u<<n));}
 else {if(aa>7.68f)return 1;if(b>5.05f){if(ab(aa-4.8f)>1.48f||b>6.88f)return 1;if(b<5.40f&&(ab(aa-4.8f)>.73f||!(g.highWindows180[n]&(1u<<(f*2+side)))))return 1;return 0;}if(ab(b+5.2f)<.12f&&aa>1.8f)return 1;}
 if(aa<.72f&&b> -6.45f&&b< -4.95f){if(aa>.61f||b< -6.32f)return 1;int moving=g.highWait180[n]>0||ab(g.highY180[n]-g.highTarget180[n])>.02f;if(b> -5.08f&&(moving||ab(g.highY180[n]-g.playerY)>.18f))return 1;}
 if(b>=-5.2f&&ab(aa-1.8f)<.10f){if(ab(b+3.25f)>.55f||!(g.highDoors180[n]&(1u<<(f*2+side))))return 1;}
 if(aa>1.9f&&b>=-5.1f){float ax,bz,rx,rz,hh;for(int k=0;high_part190(n,k,&ax,&bz,&rx,&rz);k++)if(ab(aa-ax)<rx&&ab(b-bz)<rz)return 1;
  for(int k=0;high_prop180(k,&ax,&bz,&rx,&rz,&hh);k++)if(yy<hh&&ab(aa-ax)<rx&&ab(b-bz)<rz)return 1;
 }
 return 0;
}
static float high_ground180(int n,float x,float z,float old){float a=x-cityBlockX[n],b=z-cityBlockZ[n],best=-999,dist=1000;
 if(ab(a)<.61f&&b> -6.32f&&b< -5.08f)return ab(old-g.highY180[n])<.35f?g.highY180[n]:-999;
 for(int f=0;f<high_floors180(n);f++){float y=f*3;
  if(ab(a)<1.72f&&b> -2&&f==high_floors180(n)-1)continue;
  if(ab(a)<1.72f&&b> -2&&b<2)y+=a<0?clamp((b+2)/4,0,1)*1.5f:3-clamp((b+2)/4,0,1)*1.5f;
  else if(ab(a)<1.72f&&b>=2)y+=1.5f;
  if(ab(y-old)<dist){best=y;dist=ab(y-old);}
 }
 return dist<.34f?best:-999;
}
static void high_tick180(float dt){for(int n=0;n<4;n++){float x=cityBlockX[n],z=cityBlockZ[n],old=g.highY180[n];int rider=ab(g.x-x)<.61f&&g.z>z-6.32f&&g.z<z-5.08f&&ab(g.playerY-old)<.3f;
 if(g.highWait180[n]>0)g.highWait180[n]=mx(0,g.highWait180[n]-dt);else g.highY180[n]+=clamp(g.highTarget180[n]-old,-dt*1.6f,dt*1.6f);if(rider)g.playerY=g.highY180[n];}}
static int high_use180(int perform){for(int n=0;n<4;n++){float x=cityBlockX[n],z=cityBlockZ[n];int f=high_floor180(n);
 if(g.playerY<.3f&&dst(g.x,g.z,x,z-7.65f)<2.2f&&look_at(x,1.2f,z-7.65f,.84f)){if(perform){g.highEntry180^=1u<<n;nav_refresh(x,z-7.65f);audio(12);}else SN(prompt,sizeof(prompt),"E — дверь подъезда / %d этажей",high_floors180(n));return 1;}
 if(high_at180(g.x,g.z)!=n)continue;
 if(dst(g.x,g.z,x,z-5.05f)<2.0f&&look_at(x,g.playerY+1.2f,z-5.05f,.85f)){
  int rider=ab(g.x-x)<.61f&&g.z>z-6.32f&&g.z<z-5.08f;
  if(perform){int dest=rider?(int)(g.highY180[n]/3+.1f)+(keys[16]?-1:1):f;dest=(int)clamp(dest,0,high_floors180(n)-1);g.highTarget180[n]=dest*3;g.highWait180[n]=.65f;audio(12);}
  else SN(prompt,sizeof(prompt),rider?"Лифт: E — этаж выше / Shift+E — ниже":"E — вызвать лифт на этаж %d",f+1);return 1;
 }
 for(int side=0;side<2;side++){float sg=side?1:-1;unsigned bit=1u<<(f*2+side);
  if(dst(g.x,g.z,x+sg*1.8f,z-3.25f)<1.8f&&look_at(x+sg*1.8f,f*3+1.25f,z-3.25f,.93f)){if(perform){g.highDoors180[n]^=bit;nav_refresh(g.x,g.z);audio(12);}else SN(prompt,sizeof(prompt),"E — дверь квартиры %d",f*2+side+1);return 1;}
  if(dst(g.x,g.z,x+sg*4.8f,z+4.95f)<2.1f&&look_at(x+sg*4.8f,f*3+1.8f,z+5.17f,.94f)){if(perform){work_interact(3000+n*24+f*2+side);}else SN(prompt,sizeof(prompt),"%s",work_hint(3000+n*24+f*2+side));return 1;}
  if(dst(g.x,g.z,x+sg*2.05f,z-2.45f)<1.8f&&look_at(x+sg*2.05f,f*3+1.4f,z-2.45f,.97f)){if(perform){int old=high_lit180(n,f,side);g.highLightTouched180[n]|=bit;if(old)g.highLightOff180[n]|=bit;else g.highLightOff180[n]&=~bit;audio(0);}else SN(prompt,sizeof(prompt),"E — свет квартиры");return 1;}
 }
 }return 0;
}
