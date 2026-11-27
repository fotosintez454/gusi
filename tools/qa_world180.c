#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
static int bad180=0,checks180=0;
#define T180(a,msg) do{checks180++;if(!(a)){bad180++;printf("FAIL180 n%d f%d side%d: %s at %.2f %.2f y%.2f\n",n,f,side,msg,g.x,g.z,g.playerY);}}while(0)
int main(void){savepath="build/test180.save";eventpath="build/test180.csv";audio_init();nextWorldSeed=140;test_reset();int n=0,f=0,side=0;
 for(int i=0;i<NN;i++){PERSON(i).x=189;PERSON(i).z=140;}physicsActor150=-1;
 float eye=player_eye180(),maxJump=0;pressed[32]=1;actor_tick180(.016f);pressed[32]=0;for(int i=0;i<100;i++){actor_tick180(.016f);maxJump=mx(maxJump,jump180);}T180(maxJump>.6f&&maxJump<1.0f&&jump180==0,"jump has ballistic ascent and lands exactly");
 keys[17]=1;for(int i=0;i<20;i++)actor_tick180(.016f);T180(player_eye180()<eye-.55f&&player_height180()<1.2f,"held Ctrl lowers both eye and capsule height");keys[17]=0;for(int i=0;i<20;i++)actor_tick180(.016f);T180(crouch180==0,"standing restored after Ctrl release");
 for(n=0;n<4;n++){
  float x=cityBlockX[n],z=cityBlockZ[n];g.highEntry180|=1u<<n;g.x=x;g.z=z-8.5f;g.playerY=0;
  move(0,1.8f);T180(ab(g.z-(z-6.7f))<.06f,"real entry admits a full capsule");move(1.1f,0);move(0,3.45f);T180(ab(g.z-(z-3.25f))<.06f,"lobby route passes around the lift shaft");
  for(f=0;f<high_floors180(n);f++){
   for(side=0;side<2;side++){float sg=side?1:-1;g.highDoors180[n]|=1u<<(f*2+side);g.x=x+sg*1.1f;g.z=z-3.25f;g.playerY=f*3;
    float entry=n>=2?3.7f:4.5f;move(sg*(entry-1.1f),0);T180(ab(g.x-(x+sg*entry))<.06f,"flat entrance leads into dry hall, not into toilet");
    move(0,4.45f);move(sg*(4.8f-entry),0);move(0,2.4f);T180(ab(g.z-(z+3.6f))<.07f,"actual hall-to-room path reaches rear window");
    g.x=x+sg*4.8f;g.z=z+3.8f;g.playerY=f*3;aim_at(x+sg*4.8f,f*3+1.8f,z+5.17f);work_assign190(2,n,f,side);g.jobs[2].apprentice=-2;work_set(g.workTarget[2],0);T180(high_use180(1)&&workPending==g.workTarget[2],"commissioned highrise window starts the real aimed skill check on every floor");work_cancel();
    int lit=high_lit180(n,f,side);g.highLightTouched180[n]|=1u<<(f*2+side);if(lit)g.highLightOff180[n]|=1u<<(f*2+side);else g.highLightOff180[n]&=~(1u<<(f*2+side));T180(high_lit180(n,f,side)!=lit,"shared household light state can be overridden");
   }
   if(f<high_floors180(n)-1){side=0;g.x=x-.87f;g.z=z-2.2f;g.playerY=f*3;move(0,4.8f);T180(ab(g.playerY-(f*3+1.5f))<.06f,"first stair flight reaches intermediate landing");move(1.74f,0);move(0,-4.8f);T180(ab(g.playerY-((f+1)*3))<.06f&&ab(g.z-(z-2.2f))<.08f,"second stair flight reaches next real floor");}
  }
  g.highY180[n]=g.highTarget180[n]=0;g.x=x;g.z=z-5.7f;g.playerY=0;g.highTarget180[n]=(high_floors180(n)-1)*3;for(int i=0;i<700;i++)high_tick180(.05f);T180(ab(g.playerY-g.highTarget180[n])<.01f,"lift carries rider to highest floor");
 }
 n=f=side=0;
 for(int k=0;k<3;k++){float x,z;windowpos(k,&x,&z);T180(ab(x-hx[0])>WINDOW180_W*.5f+WALL_HIT,"no rear window straddles the centre partition");}
 for(int h=0;h<NH;h++)for(int ff=0;ff<5;ff++)for(int ss=0;ss<2;ss++){unsigned bit=8;g.things150[unit150(h,ff,ss)]&=~bit;T180(flat_lit180(h,ff,ss),"interior and exterior use switched-on household state");g.things150[unit150(h,ff,ss)]|=bit;T180(!flat_lit180(h,ff,ss),"switching off also darkens facade");}

 /* Every original professional window must have an unobstructed full-body approach and keep E priority over furniture. */
 g.playerY=0;jump180=crouch180=0;g.highEntry180=15;
 for(int id=0;id<NW;id++){float wx,wy,wz,x,z;windowpos(id,&x,&z);window_handle_pos(id,&wx,&wy,&wz);int found=0;
  for(float dz=-2.3f;dz<-.5f&&!found;dz+=.25f)for(float dx=-1.75f;dx<1.75f&&!found;dx+=.25f){g.x=x+dx;g.z=z+dz;g.playerY=0;if(!body_clear150(g.x,g.z,0,.27f/WORLD_XZ,0))continue;aim_at(wx,wy,wz);interaction();if(targetWindow==id)found=1;}
  T180(found,"professional window has a real reachable aimed interaction, not a curtain-only target");
 }
 for(int h=0;h<NH;h++)for(int ff=1;ff<5;ff++)for(int k=0;k<3;k++){float x,y,z;work_point(1000+h*24+(ff-1)*6+3+k,&x,&y,&z);T180(ab(x-(hx[h]+window_a180(1,k)))<.001f,"upper-floor job marker coincides with its actual aperture");}
 g.x=cityBlockX[1]+4.8f;g.z=cityBlockZ[1]+3.8f;g.playerY=33;g.highDoors180[1]|=1u<<23;g.highWindows180[1]|=1u<<23;save_game();g.playerY=0;g.highWindows180[1]=0;int loaded=load_game();T180(loaded&&ab(g.playerY-33)<.05f&&(g.highWindows180[1]&(1u<<23)),"highest-floor position and highrise window state survive reload");
 printf("WORLD180: %d checks, %d failures\n",checks180,bad180);return bad180?1:0;
}
