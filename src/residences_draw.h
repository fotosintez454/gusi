/* Compact five-storey residences: identical floor, collision and persistent IDs. */
static void residence_window_draw(int h,int f,int side,int k){
 float x=hx[h]+window_a180(side,k),y=f*3+1.02f,z=hz[h]+(side?4.48f:-4.48f);int id=(f-1)*6+side*3+k;float a=!side?((g.stairWindows200[h]&(1u<<(f*3+k)))?1.15f:0):f?residenceWindowAngle[h][id]:(side?WIN(h*3+k).angle:((g.stairWindows200[h]&(1u<<(f*3+k)))?1.15f:0)),w=WINDOW180_W;
 /* Створка не выходит за плоскость фасада: сбоку было видно
    чёрную плиту, торчащую из стены. */
 (void)a;a=0;
 V3 p=vp(x-w*.5f-.035f,y,z),q=vp(p.x+(w+.07f)*(float)cos(a),y,z+(side?-1:1)*(w+.07f)*(float)sin(a)),v=vp(q.x,y+WINDOW180_H,q.z),u=vp(p.x,y+WINDOW180_H,p.z);surfaceTex=0;glass150(p,q,v,u,h,f,k+side*3);
 /* Jambs are outside the exact opening, never over the glazing. */
 for(int e=-1;e<=1;e+=2)box(x+e*(w*.5f+.035f),y+.80f,z,.07f,1.72f,.20f,0xc4c5b8);
 box(x,y-.065f,z,w+.20f,.10f,.30f,0xc4c5b8);box(x,y+1.655f,z,w+.14f,.07f,.20f,0xc4c5b8);
 beam(p,q,.030f,.030f,0xb9bbaa);beam(q,v,.03f,.03f,0xb9bbaa);beam(v,u,.03f,.03f,0xb9bbaa);beam(u,p,.03f,.03f,0xb9bbaa);beam(blend3(p,q,.5f),blend3(u,v,.5f),.025f,.025f,0xb9bbaa);
 /* Светлые откосы проёма: сбоку окно больше не смотрится чёрной щелью. */
 {float zr=z+(side?-1:1)*.09f;
  box(x,y+WINDOW180_H+.01f,zr,w+.04f,.09f,.05f,0x9a9c92);
  box(x,y-.01f,zr,w+.04f,.09f,.05f,0x9a9c92);
  box(x-w*.5f-.01f,y+WINDOW180_H*.5f,zr,.07f,WINDOW180_H+.08f,.05f,0x9a9c92);
  box(x+w*.5f+.01f,y+WINDOW180_H*.5f,zr,.07f,WINDOW180_H+.08f,.05f,0x9a9c92);}
 V3 handle=blend3(q,v,.46f);box(handle.x-.04f,handle.y,handle.z-.04f,.06f,.14f,.06f,0xd4ceba);
}
static void facade180(int side,float x,float z,float y,U32 paint){
 float zz=z+(side?4.5f:-4.5f),edge=-4.95f;surfaceTex=&mat_concrete;
 box(x,y+.49f,zz,9.9f,.98f,.20f,paint);box(x,y+2.81f,zz,9.9f,.38f,.20f,paint);
 for(int k=0;k<3;k++){float mid=window_a180(side,k),lo=mid-WINDOW180_W*.5f,hi=mid+WINDOW180_W*.5f;
  box(x+(edge+lo)*.5f,y+1.80f,zz,lo-edge,1.64f,.20f,paint);edge=hi;
 }
 box(x+(edge+4.95f)*.5f,y+1.80f,zz,4.95f-edge,1.64f,.20f,paint);surfaceTex=0;
}
static void residence_front_door(int h){float x=hx[h],z=hz[h]-4.5f,a=residenceFrontDoorAngle[h]*1.4f;V3 p=vp(x-1.18f,.1f,z),q=vp(p.x+2.36f*(float)cos(a),.1f,z+2.36f*(float)sin(a)),v=vp(q.x,2.65f,q.z),u=vp(p.x,2.65f,p.z);surfaceTex=0;quadstrip(p,q,v,u,0x455b5c);V3 handle=blend3(q,v,.47f);box(handle.x-.12f,handle.y,handle.z-.06f,.08f,.20f,.08f,0xbeb9a8);}
static void residence_side_walls(float x,float z,float y){surfaceTex=&mat_concrete;
 if(y<.1f&&ab(x-hx[0])<.01f&&ab(z-hz[0])<.01f){box(x-4.92f,y+1.5f,z-2.05f,.18f,3,4.9f,0xb1b5b2);box(x-4.92f,y+1.5f,z+3.35f,.18f,3,2.3f,0xb1b5b2);box(x-4.92f,y+2.7f,z+1.3f,.18f,.6f,1.8f,0xb1b5b2);}
 else {box(x-4.92f,y+1.5f,z,.18f,3,9,0xb1b5b2);}

 box(x+4.92f,y+1.5f,z-4.2f,.18f,3,.6f,0xb1b5b2);box(x+4.92f,y+1.5f,z+1.2f,.18f,3,6.6f,0xb1b5b2);box(x+4.92f,y+2.7f,z-3,.18f,.6f,1.8f,0xb1b5b2);surfaceTex=0;}
static void residence_upper_draw(int h){float x=hx[h],z=hz[h];if(!building_visible203(x+1.6f,z-1.9f,13.8f,18.1f,13.6f))return;int close=dst(camx,camz,x,z)<23,cf=(int)(g.playerY/3+.1f);ResidenceSave*s=&g.residence[h];U32 concrete151=(U32[]){0x777d80,0x80827b,0x77715e,0x7a8583}[hash32(g.worldSeed+h*17)%4];
 for(int f=0;f<5;f++){float y=f*3;if(f==0)floor203(x+6.6f,0,z-2.5f,3.3f,11.8f);int interior=close&&ab(f-cf)<=1;
 residence_side_walls(x,z,y);surfaceTex=&mat_concrete;
 facade180(0,x,z,y,concrete151);facade180(1,x,z,y,concrete151);
 for(int side=0;side<2;side++)for(int k=0;k<3;k++)residence_window_draw(h,f,side,k);
 floor_record203(x,y,z,9.8f,8.9f);surfaceTex=&mat_floor;box(x,y-.06f,z,9.8f,.12f,8.9f,0x827b75);surfaceTex=0;if(interior)room_walls_draw(h,y);
 /* Continuous enclosed core: same walls enclose lift and stairs on every floor. */
 surfaceTex=&mat_concrete;box(x+8.35f,y+1.5f,z-2.5f,.22f,3,12.0f,0xa6afad);box(x+6.65f,y+1.5f,z+3.4f,3.5f,3,.22f,0xa6afad);box(x+4.92f,y+1.5f,z-6.45f,.18f,3,4.0f,0xa6afad);
 if(f>0){box(x+6.65f,y+.55f,z-8.4f,3.5f,1.1f,.22f,0xa6afad);box(x+6.65f,y+2.8f,z-8.4f,3.5f,.4f,.22f,0xa6afad);box(x+5.75f,y+1.8f,z-8.4f,1.7f,1.4f,.22f,0xa6afad);box(x+8.1f,y+1.8f,z-8.4f,.6f,1.4f,.22f,0xa6afad);surfaceTex=0;public_window200(x+7.2f,y+1.12f,z-8.42f,1.20f,1.35f,!!(g.stairWindows200[h]&(1u<<(16+f))));}else{box(x+5.93f,1.5f,z-8.4f,2.05f,3,.22f,0xa6afad);box(x+8.28f,1.5f,z-8.4f,.22f,3,.22f,0xa6afad);box(x+7.55f,2.8f,z-8.4f,1.5f,.4f,.22f,0xa6afad);box(x+8.12f,1.16f,z-7.72f,.10f,2.3f,1.3f,0x4f675c);box(x+7.5f,2.95f,z-8.85f,2.5f,.13f,1.2f,0x5f726b);}surfaceTex=0;
 /* Lobby floor leaves a real shaft hole at x+6.1,z-6.0. */
 panel_slab(x+7.8f,y,z-6.2f,1.1f,4.1f);panel_slab(x+6.6f,y,z-7.75f,3.3f,1.1f);panel_slab(x+6.6f,y,z-4.45f,3.3f,.8f);
 if(close){panel_door(x+4.93f,y,z-3,residenceDoorAngle[h][f],1);
 /* Each public landing connects the apartment door to the switchback stairs. */
 panel_slab(x+6.6f,y,z-3.15f,3.1f,2.2f);
 if(f<5&&interior){for(int n=0;n<16;n++){float b=-2+(n+.5f)*.25f;surfaceTex=&mat_concrete;floor_record203(x+7.35f,y+(n+1)*1.5f/16,z+b,1.35f,.25f);box(x+7.35f,y+(n+1)*1.5f/16-.08f,z+b,1.35f,.16f,.25f,0xa7ada7);floor_record203(x+5.85f,y+3-n*1.5f/16,z+b,1.35f,.25f);box(x+5.85f,y+3-n*1.5f/16-.08f,z+b,1.35f,.16f,.25f,0xa7ada7);surfaceTex=0;}panel_slab(x+6.6f,y+1.5f,z+2.65f,3.1f,1.3f);stair_rail203(x+6.70f,z-1.875f,z+1.875f,y+.09375f,y+1.5f);stair_rail203(x+6.50f,z-1.875f,z+1.875f,y+3,y+1.59375f);}
 box(x-4.5f,y+1.25f,z-2.7f,.12f,.24f,.17f,0xc8ba8f);
 }
 surfaceTex=0; /* Facade joints are drawn once by residence_skin170. */
 }
 panel_slab(x+6.6f,15.16f,z-2.3f,3.45f,.6f);residence_lift_draw140(h);surfaceTex=&mat_roof;box(x,15.06f,z,10.1f,.20f,9.3f,0x7a8889);box(x+6.6f,15.06f,z-5.5f,3.5f,.2f,5.8f,0x7a8889);surfaceTex=0;
 /* Roof access is at the head of the actual fifth-floor stair flight. */
 box(x+6.65f,16.5f,z-4.5f,3.5f,2.8f,.22f,0xa6afad);box(x+8.35f,16.5f,z-.5f,.22f,2.8f,7.8f,0xa6afad);box(x+6.65f,16.5f,z+3.4f,3.5f,2.8f,.22f,0xa6afad);box(x+4.92f,16.5f,z+.7f,.18f,2.8f,5.5f,0xa6afad);box(x+4.92f,17.7f,z-3,.18f,.4f,1.8f,0xa6afad);box(x+4.92f,16.5f,z-4.1f,.18f,2.8f,.6f,0xa6afad);surfaceTex=&mat_roof;box(x+6.6f,17.95f,z-.5f,3.6f,.18f,8,0x7a8889);surfaceTex=0;
 /* Thin service details stay against existing solid walls, never across doorways. */
 surfaceTex=0;
 beam(vp(x-4.72f,.1f,z-4.67f),vp(x-4.72f,15.15f,z-4.67f),.055f,.055f,0x555e61);
 beam(vp(x-4.72f,.1f,z-4.67f),vp(x-4.45f,.1f,z-4.8f),.055f,.055f,0x555e61);
 box(x+6.55f,2.2f,z-8.55f,.7f,.36f,.045f,0x414c53);
 for(int t=0;t<(h%4)+1;t++)box(x+6.32f+t*.11f,2.2f,z-8.58f,.045f,.19f,.02f,0xb8a77d);
 box(x+7.4f,2.65f,z-8.58f,.4f,.12f,.12f,0x7e7863);
 if(g.hours>=16||g.hours<8)glow150(vp(x+7.23f,2.61f,z-8.65f),vp(x+7.57f,2.61f,z-8.65f),vp(x+7.57f,2.70f,z-8.65f),vp(x+7.23f,2.70f,z-8.65f),0xb8a77d);
 if(h%3==1){box(x-3.95f,7.7f,z-4.72f,.8f,.45f,.45f,0x8a8d83);for(int t=0;t<5;t++)box(x-4.24f+t*.13f,7.7f,z-4.96f,.025f,.3f,.02f,0x555e61);}
 if(h%2){beam(vp(x+1,15.15f,z),vp(x+1,17.6f,z),.025f,.025f,0x414c53);for(int t=0;t<4;t++)beam(vp(x+.4f,16.3f+t*.3f,z),vp(x+1.6f,16.3f+t*.3f,z),.015f,.015f,0x414c53);}

}
