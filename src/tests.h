static int failures=0,checks=0;
#define CHECK(a,msg) do{checks++;if(!(a)){printf("FAIL %s\n",msg);failures++;}else printf("PASS %s\n",msg);}while(0)
static void screenshot(const char*path){FILE*f=fopen(path,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int i=0;i<W*H;i++){unsigned char p[3]={(frame[i]>>16)&255,(frame[i]>>8)&255,frame[i]&255};fwrite(p,3,1,f);}fclose(f);}
static void shot(const char*name){render_frame();screenshot(name);}
static void test_reset(void){memset(keys,0,sizeof(keys));memset(pressed,0,sizeof(pressed));screen=0;help=0;mouseLook=1;uiMouseX=uiMouseY=-1000;uiWheel=0;new_game();yaw=pitch=0;simAccumulator=0;}
static int click_action(int action,int param){draw_ui();for(int n=0;n<uiCount;n++)if(uiButtons[n].action==action&&uiButtons[n].param==param){uiMouseX=uiButtons[n].x+uiButtons[n].w*.5f;uiMouseY=uiButtons[n].y+uiButtons[n].h*.5f;pressed[1]=1;tick(.016f);memset(pressed,0,sizeof(pressed));return 1;}return 0;}
static void aim_at(float x,float y,float z){yaw=g.rot=(float)atan2(x-g.x,z-g.z);pitch=g.look=(float)atan2(y-(g.playerY+player_eye180()),dst(g.x,g.z,x,z)*WORLD_XZ);}
static void aim_window(int id){float x,z;windowpos(id,&x,&z);g.x=x+.62f;g.z=z-1.2f;g.playerY=0;aim_at(x+.62f,1.7f,z);interaction();}
static int solve_work_test(int id){if(workPending!=id&&!work_interact(id))return 0;for(int k=0;k<3&&workPending==id;k++){bonusClock=bonus_center()*bonus_duration();pressed[32]=1;bonus_tick(0);pressed[32]=0;}return work_open(id)&&workPending<0;}
#include "tests_district.h"
#include "tests_hotfix.h"
#include "tests_v092.h"
#include "tests_v093.h"
#include "tests_v094.h"
#include "tests_v095.h"
#include "tests_v098.h"
#include "tests_v099.h"
#include "tests_v101.h"
#include "tests_v120.h"
#include "tests_v130.h"
static void tests(void){
 test_reset();int total=0;for(int id=1;id<=8;id++)total+=inv_count(id);CHECK(total==0,"fresh inventory has zero items");CHECK(g.quick[0]==0&&g.quick[1]==0,"fresh quick slots have no ghost medkit or cigarettes");CHECK(!g.homeFeedTaken,"one food package remains in the home world");CHECK(!solid(g.x,g.z),"home spawn has no collision");
 for(int n=0;n<48;n++)CHECK(g.invSlots[n]==0,"fresh inventory cell is empty");
 CHECK(inv_add(2,1)&&inv_add(6,1),"water and beer can be acquired");CHECK(inv_move(0,47)&&g.invSlots[47]==2,"drag data swaps stack without duplication");
 g.thirst=10;g.alcohol=0;inv_use(6);CHECK(itemAnim>3&&g.beer==1&&g.alcohol==0,"beer starts animation without instant effect");CHECK(!consume_begin(2),"a second consumption cannot interrupt or duplicate the first");consume_tick(1);CHECK(g.beer==1&&g.alcohol==0,"beer not consumed before mouth phase");consume_tick(1.1f);CHECK(g.beer==0&&g.bottles==1&&g.alcohol==35,"beer commits once at mouth phase and leaves a bottle");consume_tick(4);CHECK(itemAnim==0&&g.bottles==1,"lowering finishes without second effect");
 inv_use(2);consume_tick(2.1f);CHECK(g.water==0&&g.thirst>50,"water has timed consumption and hydration effect");consume_tick(5);g.cigarettes=1;g.stress=50;inv_use(5);consume_tick(4);CHECK(g.cigarettes==0&&g.stress==30,"smoking effect is delayed to animation phase");consume_tick(5);
 test_reset();g.x=-20;g.z=-9;Citizen*c=&PERSON(1);c->x=-20;c->z=-9;c->tx=20;c->tz=-9;citizenLen[1]=nav_plan(c->x,c->z,20,-9,citizenPath[1]);citizenStep[1]=0;lifeThink[1]=100;c->wait=100;int smooth=1,moving=0;for(int k=0;k<240;k++){float x=c->x,z=c->z;life_tick(1.f/120);float d=dst(x,z,c->x,c->z);if(d>.014f)smooth=0;if(d>.0001f)moving++;}CHECK(smooth&&moving>220,"near NPC locomotion advances every update without large jumps");
 float ground=nav_clear(-20,-9);g.playerY=12;CHECK(nav_clear(-20,-9)==ground&&g.playerY==12,"NPC collision query is ground-level and restores player height");g.playerY=0;
 int clear=1;for(int k=0;k<1800;k++){life_tick(1.f/30);for(int n=0;n<NN;n++)if(!nav_clear(PERSON(n).x,PERSON(n).z))clear=0;}CHECK(clear,"one-minute 48-person simulation stays outside solid geometry");
 int child=0,woman=0;for(int n=0;n<NN;n++){child+=npc_child(n);woman+=npc_woman(n);if(npc_child(n))CHECK(PERSON(n).activity!=9&&PERSON(n).activity!=10,"child never assigned smoking or beer drinking");}CHECK(child==4&&woman>=12,"district contains children and women as separate civilian types");
 int reachable=1;short route[MAXPATH];for(int h=0;h<NH;h++)if(nav_plan(-20,-9,hx[h],hz[h]-6,route)<1)reachable=0;CHECK(reachable,"all residential approaches have navigation routes");CHECK(nav_plan(-170,106,170,106,route)>1,"expanded outer street has a traversable route");
 test_reset();g.x=panel_cx(0);g.z=PANEL_Z-9.3f;aim_at(g.x,1.4f,PANEL_Z-8);CHECK(panel_solid(g.x,PANEL_Z-7.9f),"new entrance starts closed rather than already open");bonusActive=0;CHECK(panel_interact()&&(entryOpen&1)&&!bonusActive,"entrance opens physically without a spurious check");CHECK(!panel_solid(g.x,PANEL_Z-7.9f),"opened entrance admits the player");
 g.x=panel_cx(0)-.95f;g.z=PANEL_Z;aim_at(panel_cx(0)-1.8f,1.25f,PANEL_Z);CHECK(panel_solid(panel_cx(0)-1.8f,PANEL_Z),"flat door collision starts closed");CHECK(panel_interact()&&(flatOpen&1)&&!bonusActive,"aimed flat door opens instead of only displaying QTE");for(int k=0;k<30;k++)panel_motion(1.f/60);CHECK(flatAngles[0]>.98f,"flat leaf visibly rotates open");CHECK(!panel_solid(panel_cx(0)-1.8f,PANEL_Z),"open flat admits player");
 g.x=panel_cx(0)-4;g.z=PANEL_Z+6.5f;g.playerY=3;aim_at(g.x,4.7f,PANEL_Z+7.85f);CHECK(panel_interact()&&!(g.panelWindowOpen&(1ull<<4))&&!bonusActive,"unordered modular window remains shut without starting a check");unsigned long long mask=g.panelWindowOpen;for(int k=0;k<100;k++)panel_solid(g.x,g.z);CHECK(g.panelWindowOpen==mask,"collision queries never toggle windows or mutate world state");bonusActive=0;CHECK(panel_interact()&&!(g.panelWindowOpen&(1ull<<4)),"repeated click cannot bypass the order lock");bonusActive=0;
 panel_reset();entryOpen=3;g.x=panel_cx(0)-.82f;g.z=PANEL_Z+1.8f;g.playerY=0;for(int f=0;f<3;f++){for(int k=0;k<92;k++)move(0,.055f);for(int k=0;k<30;k++)move(.055f,0);for(int k=0;k<92;k++)move(0,-.055f);CHECK(ab(g.playerY-(f+1)*3)<.16f,"three switchback flights reach all four walk-up floors");for(int k=0;k<30;k++)move(-.055f,0);}
 for(int w=0;w<2;w++){panel_reset();g.x=panel_cx(w)-3.5f;g.z=PANEL_Z-5.8f;g.playerY=0;lift_request(w,3);for(int k=0;k<900;k++)panel_motion(1.f/60);CHECK(ab(g.playerY)<.02f&&liftDoor[w],"four-storey walk-up has no lift movement");CHECK(liftY[1-w]==0,"legacy lift save fields stay inert");}
 g.playerY=9;g.panelWindowOpen=1ull<<39;flatOpen=37;g.money=777;save_game();g.money=0;CHECK(load_game()&&g.money==777&&g.panelWindowOpen==(1ull<<39)&&g.playerY==9&&flatOpen==37,"v12 save preserves upper floor, windows, doors and money");
 test_reset();screen=1;CHECK(click_action(1,78)&&screen==0&&inv_weight()==0,"mouse New Game begins with an empty backpack");screen=2;CHECK(click_action(1,79)&&screen==6,"mouse opens redesigned options");CHECK(click_action(4,3)&&optionsTab==3,"mouse opens focus/winter tab");int winter=g.winter;CHECK(click_action(3,17)&&g.winter!=winter,"mouse toggles winter");CHECK(click_action(3,1)&&g.focusMode,"mouse toggles ADHD profile");
 int screens[]={1,2,3,4,5,6,7,8,9,10,11,14,26};int bounds=1;for(int k=0;k<13;k++){screen=screens[k];draw_ui();CHECK(uiCount>0,"redesigned screen has mouse controls");for(int n=0;n<uiCount;n++)if(uiButtons[n].x<0||uiButtons[n].y<0||uiButtons[n].x+uiButtons[n].w>800||uiButtons[n].y+uiButtons[n].h>450)bounds=0;}CHECK(bounds,"all menu hit targets remain inside viewport");
 screen=10;dialogNPC=1;CHECK(click_action(11,1)&&dialogTopic==1,"civilian dialogue is selectable with mouse");CHECK(click_action(1,52)&&screen==0,"mouse closes civilian dialogue");
 test_reset();skillLevel=200;int all=1;for(int k=0;k<10;k++){aim_window(k);WIN(k).handle=0;toggle_window(k);if(k<3){if(!WIN(k).handle||bonusActive)all=0;}else if(WIN(k).handle||bonusActive)all=0;}CHECK(all,"home windows open freely while even high skill cannot open unordered foreign windows");
 unsigned int seed=g.worldSeed;g.x=120;g.z=100;int home=1;for(int k=0;k<1800;k++){cat_tick(1.f/60);if(catX<hx[0]+.35f||catX>hx[0]+2.45f||catZ<hz[0]-1.55f||catZ>hz[0]+.1f)home=0;}CHECK(home&&g.worldSeed==(unsigned int)seed,"cat remains home during 30s away");
 tests_district();
 tests_hotfix();
 tests_v092();
 tests_v093();
 tests_v094();
 tests_v095();
 tests_v098();
 tests_v099();
 tests_v101();
 tests_v120();
 tests_v130();
 printf("TESTS: %d checks, %d failures\n",checks,failures);
}
int main(void){savepath="test_v091.save";eventpath="test_v091_events.csv";audio_init();tests();return failures?1:0;}
