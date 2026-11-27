#define HEADLESS
#define main original_game_main
#include "../src/main.c"
#undef main
/* Scripted movement uses the real move/panel_step/collision functions, not teleports between floors. */
int main(int argc,char**argv){savepath="release_v098/walk.save";eventpath="release_v098/walk_events.csv";audio_init();test_reset();set_resolution(0);g.hours=13;realTime=0;g.x=panel_cx(0);g.z=PANEL_Z-9.3f;g.playerY=0;screen=0;noticeTime=0;showRadar=0;hudMode=1;
 float cx=panel_cx(0),zz=PANEL_Z;float path[][2]={{0,-6},{-.82f,-3},{-.82f,1.8f},{-.82f,6.7f},{.82f,6.7f},{.82f,1.8f},{.8f,0},{-3.5f,0},{-3.7f,3.6f},{-3.7f,1.5f},{-3.5f,0},{.82f,0},{.82f,1.8f},{.82f,6.7f},{-.82f,6.7f},{-.82f,1.8f},{0,-3},{0,-9.5f}};
 int skip=argc>1?atoi(argv[1]):0;int waypoint=0,frames=0,failed=0,opened=0;float stall=0,last=1e9f;FILE*trace=fopen("release_v098/walk_trace.csv","w");fprintf(trace,"seconds,waypoint,x,z,y\n");
 for(int step=0;step<1800;step++){realTime=step/30.f;noticeTime=0;panel_motion(1.f/30);if(step==0){aim_at(cx,1.4f,zz-8);opened+=panel_interact();}if(step>20&&waypoint<18){float tx=cx+path[waypoint][0],tz=zz+path[waypoint][1],d=dst(g.x,g.z,tx,tz);if(waypoint==7&&g.x>cx-1.2f){aim_at(cx-1.8f,4.25f,zz);if(!(flatOpen&1u<<2))opened+=panel_interact();}
 if(d<.065f){waypoint++;last=1e9f;stall=0;}else{aim_at(tx,g.playerY+1.52f,tz);float speed=.061f;move((tx-g.x)/d*mn(d,speed),(tz-g.z)/d*mn(d,speed));float now=dst(g.x,g.z,tx,tz);if(now>=last-.0001f)stall+=1.f/30;else stall=0;last=now;if(stall>1){failed=1;break;}}}
 if(step%10==0){update_interior();interaction();char p[128];SN(p,sizeof(p),"qa/v098/walk_frames/%04d.ppm",frames);if(frames++>=skip)shot(p);fprintf(trace,"%.3f,%d,%.4f,%.4f,%.4f\n",realTime,waypoint,g.x,g.z,g.playerY);fflush(trace);}}
 fclose(trace);FILE*f=fopen("release_v098/walkthrough.json","w");fprintf(f,"{\"seconds\":60,\"frames\":%d,\"fps\":3,\"waypoints_completed\":%d,\"waypoints_total\":18,\"opened_doors\":%d,\"stalled\":%s,\"final_height\":%.3f,\"scripted_headless\":true,\"windows_runtime\":false}\n",frames,waypoint,opened,failed?"true":"false",g.playerY);fclose(f);return failed||waypoint!=18||ab(g.playerY)>.2f?1:0;}
