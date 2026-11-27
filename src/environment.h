/* Keep the four skyline blocks outside the remodeled long panel buildings.
   The former (-120/+120, -88) positions intersected wings 1 and 7. */
static const float cityBlockX[4]={-82,82,-66,66},cityBlockZ[4]={-18,18,-18,18};
static int city_solid(float x,float z){return high_solid180(x,z);}

/* Shared POI transforms: collision, interaction, map and render use these coordinates. */
static void quarter_shop(int n,float*x,float*z){float px=panel_origin_x(n),pz=panel_origin_z(n);*x=px+(px< -90?-19:px>90?19:0);*z=pz+(ab(px)>90?4:-12);}
static int quarter_solid(float x,float z){for(int n=0;n<15;n++){float a,b;quarter_shop(n,&a,&b);if(ab(x-a)<1.62f&&ab(z-b)<1.22f)return 1;}return 0;}
/* Static primitive colliders, spatially indexed in 2m bins. */
typedef struct{float x,z,rx,rz;int kind,id;} EnvCollider;
static EnvCollider env[512];static int envCount=0;
static unsigned short envCells[192*144][16];static unsigned char envCounts[192*144];
static int dialogNPC=0,dialogTopic=0,navHouse=-1;
static float catX=-29,catZ=-23,catTX=-29,catTZ=-23,catTimer=0,catPhase=0;static int catState=0;
static void env_add(float x,float z,float rx,float rz,int kind,int id){if(envCount>=512)return;int k=envCount++;env[k]=(EnvCollider){x,z,rx,rz,kind,id};int a=(int)clamp((x-rx+192)/2,0,191),b=(int)clamp((x+rx+192)/2,0,191),c=(int)clamp((z-rz+144)/2,0,143),d=(int)clamp((z+rz+144)/2,0,143);for(int zz=c;zz<=d;zz++)for(int xx=a;xx<=b;xx++){int cell=zz*192+xx;if(envCounts[cell]<16)envCells[cell][envCounts[cell]++]=(unsigned short)k;}}
static int env_solid(float x,float z){int xx=(int)((x+192)/2),zz=(int)((z+144)/2);if(xx<0||xx>=192||zz<0||zz>=144)return 0;int cell=zz*192+xx;if(envCounts[cell]>=16){for(int k=0;k<envCount;k++){EnvCollider*c=&env[k];if(c->kind==6&&c->id==ignoreBench141)continue;if(ab(x-c->x)<c->rx&&ab(z-c->z)<c->rz)return 1;}return 0;}for(int k=0;k<envCounts[cell];k++){EnvCollider*c=&env[envCells[cell][k]];if(c->kind==6&&c->id==ignoreBench141)continue;if(ab(x-c->x)<c->rx&&ab(z-c->z)<c->rz)return 1;}return 0;}
static void loot_position(int n,float*x,float*z){int p=n/2;float px=panel_origin_x(p);*x=px+(px< -90?19:px>90?-21:0)+(n%2)*1.6f;*z=panel_origin_z(p)+(ab(px)>90?3:11);}
static void env_build(void){envCount=0;memset(envCounts,0,sizeof(envCounts));for(int n=0;n<12;n++){float x=-52+n*9.3f+(world_value(n+150)-.5f)*2,z=(n%2?-11:11)+(world_value(n+180)-.5f)*2,sc=.75f+(n%3)*.12f;int reserved=0;for(int q=0;q<30;q++){float a,b;loot_position(q,&a,&b);if(dst(x,z,a+.55f,b)<3.6f)reserved=1;}for(int q=0;q<54;q++){float a,b;bench_position141(q,&a,&b);if(dst(x,z,a,b)<2.8f)reserved=1;}if(!reserved&&outside141(x,z,2.1f))env_add(x,z,.18f*sc,.18f*sc,0,n);}env_add(-44,-10,1.24f,2.15f,3,0);env_add(shopX,shopZ+1,3.9f,.7f,3,0);env_add(KX,KZ-1.4f,1.3f,.23f,3,1);env_add(OX,OZ+2.7f,4.1f,.7f,3,0);decor_build140();for(int n=0;n<54;n++){float a,b;bench_position141(n,&a,&b);env_add(a+2.05f,b+.25f,.18f,.18f,8,n);if(n<24)for(int k=0;k<3;k++)env_add(a-2.2f-k*.55f,b+.65f,.225f,.375f,8,n);}for(int n=0;n<15;n++){float a,b;loot_position(n*2,&a,&b);a+=1.35f;env_add(a,b+.85f,2.2f,.08f,8,n);for(int e=-1;e<=1;e+=2)env_add(a+e*2.16f,b,.08f,.9f,8,n);}street_build205();}
