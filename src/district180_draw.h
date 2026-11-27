/* Original measured highrise meshes. Exact same part and prop tables feed collision. */
static void high_window180(int n,int f,int side,int front){float x=cityBlockX[n]+(side?1:-1)*(front?6.8f:4.8f),z=cityBlockZ[n]+(front?-5.23f:5.23f),y=f*3+(front?1.03f:.13f),wh=front?1.5f:2.4f,w=1.6f;unsigned bit=1u<<(f*2+side);float angle=!front&&(g.highWindows180[n]&bit)?1.15f:0;
 V3 p=vp(x-w*.5f,y,z),q=vp(p.x+w*(float)cos(angle),y,z-(float)sin(angle)*w),v=vp(q.x,y+wh,q.z),u=vp(p.x,y+wh,p.z);surfaceTex=0;
 if(high_at180(camx,camz)!=n)pane_part201(p,q,v,u,.12f,1,high_lit180(n,f,side)?high_color180(n,f,side):0x414c53,high_lit180(n,f,side));
 pane_part201(p,q,v,u,0,.12f,(n+f+side)%2?0x8e8a7e:0x6e7770,0);
 for(int e=-1;e<=1;e+=2)box(x+e*.84f,y+wh*.5f,z,.08f,wh+.13f,.20f,0xb9b9ac);
 box(x,y-.045f,z,w+.18f,.09f,.28f,0xb9b9ac);box(x,y+wh+.05f,z,w+.18f,.08f,.20f,0xb9b9ac);
 beam(p,q,.035f,.035f,0xb9b9ac);beam(q,v,.035f,.035f,0xb9b9ac);beam(v,u,.035f,.035f,0xb9b9ac);beam(u,p,.035f,.035f,0xb9b9ac);beam(blend3(p,q,.5f),blend3(u,v,.5f),.03f,.03f,0xb9b9ac);
}
static void high_facade180(int n,int f,int front,U32 paint){float x=cityBlockX[n],z=cityBlockZ[n]+(front?-5.2f:5.2f),y=f*3,off=front?6.8f:4.8f;surfaceTex=&mat_concrete;
 for(int side=-1;side<=1;side+=2){float a=side<0?-7.9f:(front?1.8f:0),b=side<0?(front?-1.8f:0):7.9f,mid=side*off,lo=mid-.8f,hi=mid+.8f;
 box(x+(a+b)*.5f,y+(front?.49f:.04f),z,b-a,front?.98f:.08f,.2f,paint);box(x+(a+b)*.5f,y+2.78f,z,b-a,.46f,.2f,paint);
 box(x+(a+lo)*.5f,y+(front?1.77f:1.32f),z,lo-a,front?1.58f:2.48f,.2f,paint);box(x+(hi+b)*.5f,y+(front?1.77f:1.32f),z,b-hi,front?1.58f:2.48f,.2f,paint);
 }surfaceTex=0;for(int s=0;s<2;s++)high_window180(n,f,s,front);
}
static void high_extras190(int n,int f,int detail);
static void high_draw180(void){for(int n=0;n<4;n++){float x=cityBlockX[n],z=cityBlockZ[n];if(dst(camx,camz,x,z)>viewdist+20||!building_visible203(x,z-1.2f,16.4f,high_floors180(n)*3+2.2f,15.4f))continue;int floors=high_floors180(n),near=dst(camx,camz,x,z)<25,cf=high_floor180(n);U32 paint=(U32[]){0x8a918b,0x96988a,0x7a8988,0x94928b}[n];
 for(int f=0;f<floors;f++){float y=f*3;if(f==0)floor203(x,0,z,15.8f,10.6f);int detail=near&&ab(f-cf)<1.1f;high_facade180(n,f,0,paint);high_facade180(n,f,1,paint);high_extras190(n,f,detail);
 surfaceTex=&mat_concrete;for(int s=-1;s<=1;s+=2){box(x+s*7.9f,y+1.5f,z,.2f,3,10.6f,paint);box(x+s*1.8f,y+1.5f,z-6.42f,.2f,3,2.45f,paint);}
 if(f){box(x,y+1.5f,z-7.65f,3.6f,3,.2f,paint);}else{box(x-1.24f,y+1.5f,z-7.65f,1.12f,3,.2f,paint);box(x+1.24f,y+1.5f,z-7.65f,1.12f,3,.2f,paint);box(x,y+2.76f,z-7.65f,1.4f,.48f,.2f,paint);}
 surfaceTex=0;
 /* Floors are in the flats and landings only; no solid slab through the stairwell. */
 for(int s=-1;s<=1;s+=2){panel_slab(x+s*4.85f,y,z,6.1f,10.6f);panel_slab(x+s*1.25f,y,z-4.85f,1.1f,5.7f);}
 panel_slab(x,y,z-7.03f,1.4f,1.26f);panel_slab(x,y,z-3.48f,1.4f,2.95f);
 if(f<floors-1){panel_slab(x,y+1.5f,z+3.6f,3.4f,3.2f);if(detail)for(int k=0;k<16;k++){float b=-2+(k+.5f)*.25f;surfaceTex=&mat_concrete;floor_record203(x-.87f,y+(k+1)*1.5f/16,z+b,1.4f,.25f);box(x-.87f,y+(k+1)*1.5f/16-.08f,z+b,1.4f,.16f,.25f,0x969e95);floor_record203(x+.87f,y+3-k*1.5f/16,z+b,1.4f,.25f);box(x+.87f,y+3-k*1.5f/16-.08f,z+b,1.4f,.16f,.25f,0x969e95);surfaceTex=0;}}
 for(int side=0;side<2;side++){float sg=side?1:-1;unsigned bit=1u<<(f*2+side);
 surfaceTex=&art_wallpaper;box(x+sg*1.8f,y+1.5f,z-4.55f,.2f,3,1.5f,0xa4aaa0);box(x+sg*1.8f,y+1.5f,z+1.2f,.2f,3,7.8f,0xa4aaa0);box(x+sg*1.8f,y+2.75f,z-3.25f,.2f,.5f,1.1f,0xa4aaa0);surfaceTex=0;
 if(!detail)continue;
 /* Narrow 1.1m logical side door, turned along x only while open. */
 if(g.highDoors180[n]&bit)box(x+sg*2.33f,y+1.2f,z-3.80f,1.05f,2.4f,.055f,0x767566);else box(x+sg*1.8f,y+1.2f,z-3.25f,.055f,2.4f,1.1f,0x767566);
 float ax,bz,rx,rz,hh;surfaceTex=&art_wallpaper;for(int k=0;high_part190(n,k,&ax,&bz,&rx,&rz);k++)box(x+sg*ax,y+1.5f,z+bz,rx*2,3,rz*2,0xa9ad9c);surfaceTex=0;
 box(x+sg*4.85f,y+2.82f,z,5.7f,.06f,10.2f,0xb8bdb3);
 surfaceTex=&art_parquet;box(x+sg*4.85f,y+.02f,z+1.3f,5.65f,.035f,7.3f,0xa7a491);surfaceTex=0;
 for(int k=0;high_prop180(k,&ax,&bz,&rx,&rz,&hh);k++){float px=x+sg*ax,pz=z+bz;
 if(k==0){box(px,y+.28f,pz,rx*2,.20f,rz*2,0x696352);box(px,y+.48f,pz,rx*2-.04f,.22f,rz*2-.04f,(n+f+side)%2?0x858875:0x6f7b76);box(px,y+.64f,pz+.65f,.8f,.10f,.42f,0xc5c3b4);}
 if(k==1){surfaceTex=&art_wood;box(px,y+hh*.5f,pz,rx*2,hh,rz*2,0x9b8d72);surfaceTex=0;box(px-sg*(rx+.02f),y+1.0f,pz,.04f,1.9f,rz*2-.08f,0x8a7b62);box(px-sg*(rx+.05f),y+1.1f,pz,.05f,.16f,.045f,0xb8b19c);}
 if(k==2){box(px,y+.9f,pz,rx*2,1.8f,rz*2,0xaeb4a8);box(px-sg*(rx+.025f),y+.95f,pz,.04f,1.65f,.65f,0xc1c3b5);}
 if(k==3){art_kitchen_table(px,y,pz);soft_mesh(px,y+.89f,pz,.07f,.10f,.07f,0,0xc7b997);}
 if(k==4){box(px,y+.35f,pz,.55f,.7f,.65f,0xc0c3b5);box(px,y+.74f,pz,.6f,.08f,.70f,0x8b978e);}
 }
 radiator170(x+sg*6.60f,y,z+4.80f);box(x+sg*2.03f,y+1.4f,z-2.45f,.10f,.18f,.03f,high_lit180(n,f,side)?0xc5b387:0x535e57);
 box(x+sg*4.8f,y+2.57f,z+2.0f,.40f,.10f,.40f,high_lit180(n,f,side)?high_color180(n,f,side):0x5a655f);
 if(high_lit180(n,f,side))glow150(vp(x+sg*4.8f-.15f,y+2.51f,z+1.85f),vp(x+sg*4.8f+.15f,y+2.51f,z+1.85f),vp(x+sg*4.8f+.15f,y+2.51f,z+2.15f),vp(x+sg*4.8f-.15f,y+2.51f,z+2.15f),high_color180(n,f,side));
 }
 if(detail&&f<floors-1){stair_rail203(x-.21f,z-1.875f,z+1.875f,y+.09375f,y+1.5f);stair_rail203(x+.21f,z-1.875f,z+1.875f,y+3,y+1.59375f);}
 for(int s=-1;s<=1;s+=2)box(x+s*7.99f,y,z,.025f,.045f,10.4f,0x666f6c);
 }
 /* Lift car is a real moving floor inside the shaft. The front opens onto every landing. */
 surfaceTex=&mat_metal;for(int e=-1;e<=1;e+=2)box(x+e*.70f,floors*1.5f,z-5.7f,.10f,floors*3,1.5f,0x606c68);box(x,floors*1.5f,z-6.45f,1.4f,floors*3,.10f,0x606c68);surfaceTex=0;
 float ly=g.highY180[n];lift_display202(x,ly+2.0f,z-6.36f,ly,g.highTarget180[n]);panel_slab(x,ly,z-5.7f,1.2f,1.3f);box(x,ly+2.35f,z-5.7f,1.2f,.08f,1.3f,0x979e90);
 if(g.highWait180[n]>0||ab(ly-g.highTarget180[n])>.02f)box(x,ly+1.15f,z-4.99f,1.25f,2.3f,.08f,0x7e8980);
 surfaceTex=&mat_roof;box(x,floors*3+.03f,z,16,.18f,10.6f,0x6d7773);box(x,floors*3+.03f,z-6.43f,3.7f,.18f,2.55f,0x6d7773);box(x,floors*3+1.1f,z-5.7f,2.2f,2,2.3f,0x7c867e);surfaceTex=0;
 /* Entry hardware and facade service details have consistent locations. */
 box(x,2.70f,z-8.0f,2.4f,.16f,1.1f,0x5b6a63);entry_lamp170(x+.95f,2.43f,z-7.78f,g.hours>=16||g.hours<9);box(x+.92f,1.3f,z-7.78f,.16f,.32f,.06f,0x46554e);
 if(!(g.highEntry180&(1u<<n)))box(x,1.2f,z-7.65f,1.3f,2.4f,.07f,0x4f665b);else box(x-.65f,1.2f,z-6.4f,.07f,2.4f,1.2f,0x4f665b);
 for(int e=-1;e<=1;e+=2){beam(vp(x+e*7.6f,.1f,z-5.4f),vp(x+e*7.6f,floors*3,z-5.4f),.06f,.06f,0x596761);for(int f=1;f<floors;f++){float yy=f*3;box(x+e*7.98f,yy+1.2f,z+.5f,.03f,.4f,.7f,0x93998d);}}
 for(int f=1;f<floors;f+=3){box(x+6.8f,f*3+.5f,z-5.45f,.72f,.42f,.40f,0x969c90);beam(vp(x-6.6f,f*3+1.7f,z-5.5f),vp(x-6.6f,f*3+2.2f,z-5.5f),.025f,.025f,0x7f8982);soft_mesh(x-6.6f,f*3+2.0f,z-5.55f,.30f,.32f,.09f,0,0x9fa49a);}
 }}
static void high_lights180(void){for(int n=0;n<4;n++)if(high_at180(camx,camz)==n){int f=high_floor180(n);float y=f*3;for(int side=0;side<2;side++){float sg=side?1:-1;float ax,bz,rx,rz;for(int k=0;high_part190(n,k,&ax,&bz,&rx,&rz);k++)lighting_block(cityBlockX[n]+sg*ax,y+1.5f,cityBlockZ[n]+bz,rx,1.5f,rz);
 if(high_lit180(n,f,side)){U32 c=high_color180(n,f,side);lighting_room(cityBlockX[n]+sg*4.8f,y+2.45f,cityBlockZ[n]+2.6f,2.9f,2.6f,1.45f,c);lighting_room(cityBlockX[n]+sg*5.1f,y+2.45f,cityBlockZ[n]-2.2f,2.6f,2.7f,1.25f,c);}}
 lighting_room(cityBlockX[n],y+2.45f,cityBlockZ[n]-3.4f,1.7f,3.6f,.85f,0xb8a77d);}}
/* A connected pedestrian graph is baked against actual capsule collision, not arbitrary lines. */
#define WALK180_W 192
#define WALK180_H 144
static unsigned short walkPrev180[WALK180_W*WALK180_H],walkQueue180[WALK180_W*WALK180_H];
static unsigned char walkClosed180[WALK180_W*WALK180_H],walkPath180[WALK180_W*WALK180_H];static unsigned walkSeed180=0;static int walkReady180=0;
static void walk_mark180(float x,float z){int a=(int)((x+192)/2),b=(int)((z+144)/2);if(a<0||a>=192||b<0||b>=144)return;int c=b*192+a;if(walkClosed180[c]){int found=-1;for(int r=1;r<4&&found<0;r++)for(int dz=-r;dz<=r;dz++)for(int dx=-r;dx<=r;dx++){int xx=a+dx,zz=b+dz;if(xx<0||xx>=192||zz<0||zz>=144)continue;int q=zz*192+xx;if(!walkClosed180[q]&&walkPrev180[q]){found=q;break;}}if(found<0)return;c=found;}
 for(int k=0;k<27648&&walkPrev180[c];k++){walkPath180[c]=1;int p=walkPrev180[c]-1;if(p==c)break;c=p;}}
static void walk_build180(void){if(walkReady180&&walkSeed180==g.worldSeed)return;walkSeed180=g.worldSeed;walkReady180=1;memset(walkPrev180,0,sizeof(walkPrev180));memset(walkPath180,0,sizeof(walkPath180));int head=0,tail=0;float old=g.playerY;g.playerY=0;navStatic150=1;
 for(int b=0;b<144;b++)for(int a=0;a<192;a++){int c=b*192+a;float x=-191+a*2,z=-143+b*2;walkClosed180[c]=!body_clear150(x,z,0,.25f,0);if(!walkClosed180[c]&&(ab(ab(z)-7)<1.1f||ab(ab(x)-97)<1.1f||ab(ab(z)-104)<1.1f)){walkPrev180[c]=c+1;walkQueue180[tail++]=c;}}
 while(head<tail){int c=walkQueue180[head++],x=c%192,z=c/192;int offsets[4]={-1,1,-192,192};for(int k=0;k<4;k++){if((k==0&&x==0)||(k==1&&x==191)||(k==2&&z==0)||(k==3&&z==143))continue;int q=c+offsets[k];if(!walkClosed180[q]&&!walkPrev180[q]&&nav_edge(c,q)){walkPrev180[q]=c+1;walkQueue180[tail++]=q;}}}
 for(int h=0;h<NH;h++)walk_mark180(hx[h]+7.55f,hz[h]-9.5f);
 for(int p=0;p<PANEL_COUNT;p++){for(int w=0;w<2;w++)walk_mark180(panel_origin_x(p)+(w?7:-7),panel_origin_z(p)-9.7f);float x,z;quarter_shop(p,&x,&z);walk_mark180(x,z-2.8f);}
 for(int n=0;n<4;n++)walk_mark180(cityBlockX[n],cityBlockZ[n]-8.5f);walk_mark180(OX,OZ-7);walk_mark180(shopX,shopZ-7.5f);g.playerY=old;navStatic150=0;
}
static void district_plan180(void){walk_build180();surfaceTex=&mat_asphalt;
 /* Outer service streets and a continuous inner main street. */
 for(int s=-1;s<=1;s+=2){paved_box202(s*101,-.01f,0,5,.02f,260,0x55534f);box(0,-.01f,s*108,202,.02f,5,0x55534f);}
 surfaceTex=&mat_pavement;for(int b=0;b<144;b++){int run=-1;for(int a=0;a<=192;a++){int on=a<192&&walkPath180[b*192+a];if(on&&run<0)run=a;if(!on&&run>=0){float x0=-191+run*2,x1=-191+(a-1)*2,z=-143+b*2;if(dst(camx,camz,(x0+x1)*.5f,z)<viewdist+ab(x1-x0)*.5f)paved_box202((x0+x1)*.5f,.024f,z,x1-x0+1.25f,.02f,1.25f,0x999c90);run=-1;}}}
 surfaceTex=0;for(int h=0;h<NH;h++)residence_skin170(h);
 /* Deliberate courtyard territories: sand/grit, service apron, worn play surfacing. */
 surfaceTex=&mat_pavement;box(-13,.025f,-13,26,.025f,10,0x777b70);box(20,.025f,13,24,.025f,10,0x6e766c);box(-48,.025f,12,14,.025f,13,0x8b9286);surfaceTex=0;
 for(int n=0;n<15;n++){float x,z;loot_position(n*2,&x,&z);surfaceTex=&mat_asphalt;box(x+1.25f,.03f,z,5.5f,.035f,3.6f,0x636c63);surfaceTex=0;}
}
