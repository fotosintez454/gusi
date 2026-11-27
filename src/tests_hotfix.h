/* Regression scenarios reported by the player: actual input/UI chains. */
static unsigned int hf_frame_hash(void){unsigned int h=2166136261u;for(int i=0;i<W*H;i++)h=(h^frame[i])*16777619u;return h;}
static void tests_hotfix(void){
 for(int run=0;run<5;run++){test_reset();int count=0;for(int id=1;id<=8;id++)count+=inv_count(id);CHECK(count==0&&!g.quick[0]&&!g.quick[1],"five new sessions: inventory and quick slots remain empty");}
 test_reset();pressed[9]=1;tick(.016f);pressed[9]=0;CHECK(screen==3,"physical Tab opens work board");
 CHECK(click_action(32,1)&&screen==3&&job_accepted(1)&&g.activeJob==1,"mouse Accept changes job state, not merely selected number");
 CHECK(click_action(32,1)&&screen==11&&navHouse==g.jobs[1].house,"accepted job exposes a mouse route to the correct house");
 map_route();CHECK(mapPathLen>1&&mapNeedsExit,"job route from closed home continues outside with an explicit exit instruction");
 screen=3;CHECK(click_action(32,2)&&job_accepted(2)&&!job_accepted(1),"switching jobs leaves only one explicitly accepted job");
 save_game();g.jobs[2].apprentice=-1;CHECK(load_game()&&job_accepted(2)&&g.activeJob==2,"accepted job survives normal v12 save and load");
 test_reset();int h=1;work_assign(1,h,0);g.x=hx[h];g.z=hz[h]+3;g.playerY=0;g.salary=0;work_interact(h*3+1);CHECK(g.salary==0&&g.jobs[1].done==0&&!work_open(h*3+1),"unaccepted listing neither opens nor pays");accept_job(1);int fee=g.jobs[1].pay;solve_work_test(h*3+1);CHECK(g.salary>=fee&&g.completed==1&&!job_accepted(1),"three successful checks complete an accepted balcony and credit salary once");
 CHECK(navOffice,"completed job redirects route toward the cashier");g.x=OX;g.z=OZ+1.8f;g.playerY=0;screen=0;pressed['E']=1;tick(.016f);pressed['E']=0;CHECK(screen==9,"completed work can open physical office cashier");int salary=g.salary,money=g.money;CHECK(click_action(1,13)&&g.salary==0&&g.money==money+salary,"mouse cashier collection pays the completed accepted job");
 test_reset();g.popupMode=0;g.x=hx[0]+1.4f;g.z=hz[0]-3.25f;aim_at(hx[0]+1.4f,.28f,hz[0]-2.1f);interaction();CHECK(interactKind==13,"look at physical home packet selects cat food");pressed['E']=1;tick(.016f);pressed['E']=0;CHECK(g.feed==1&&g.homeFeedTaken,"actual world interaction picks up one food packet");
 g.petHunger=30;g.x=catX;g.z=catZ-1.5f;aim_at(catX,.35f,catZ);pressed[1]=1;tick(.016f);pressed[1]=0;CHECK(screen==15,"clicking cat opens explicit feed and pet controls");
 CHECK(click_action(33,0)&&g.feed==0&&g.petHunger>74&&catState==4,"mouse Feed consumes one packet, increases satiety and starts eating state");float sat=g.petHunger;click_action(33,0);CHECK(g.feed==0&&g.petHunger==sat,"repeated mouse feed without food neither duplicates nor consumes anything");
 CHECK(click_action(33,1)&&catState==3,"Pet is a separate mouse action");g.feed=1;g.petHunger=100;CHECK(!feed_cat()&&g.feed==1,"full cat does not waste a packet");
 g.petHunger=20;g.playerY=3;CHECK(!feed_cat()&&g.feed==1,"cannot feed through floor");g.playerY=0;g.x=hx[0]+20;CHECK(!feed_cat()&&g.feed==1,"cannot feed remotely outside home");
 test_reset();g.feed=1;g.petHunger=20;g.x=catX;g.z=catZ-1;screen=5;inv_sync();for(int n=0;n<48;n++)if(g.invSlots[n]==3)invSelected=n;CHECK(click_action(21,0)&&g.feed==0&&g.petHunger==65,"inventory Feed button calls the same validated feeding operation");
 test_reset();set_resolution(0);g.x=-20;g.z=-9;g.rot=-2.3f;g.look=-.1f;screen=0;hudMode=2;render_frame();unsigned int before=hf_frame_hash();g.apprentices=3;render_frame();CHECK(hf_frame_hash()==before,"hiring three apprentices adds no player-relative follower meshes");CHECK(render_mirror==0,"mirror is disabled and has no render path");
 test_reset();g.apprentices=1;g.apprenticeXP[0]=30;screen=7;CHECK(click_action(1,49)&&g.autoWork[0],"crew can send worker on independent jobs");CHECK(click_action(1,49)&&!g.autoWork[0],"returning worker means office standby, not escort");
 test_reset();g.x=20;g.z=-9;g.playerY=0;navOffice=0;navHouse=NH+14;mapGoal=-99;map_route();CHECK(mapPathLen>1,"work routing remains compatible with district map");
}
