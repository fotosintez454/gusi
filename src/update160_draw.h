/* Rooms first: lighting follows the same walls and per-flat switch as collision. */
static void rooms_light160(void){
 for(int h=0;h<NH;h++)if(dst(camx,camz,hx[h],hz[h])<26){int f=(int)clamp((float)floor(g.playerY/3),0,4);float y=f*3;
 lighting_block(hx[h],y+1.5f,hz[h]+1.25f,.10f,1.5f,3.15f);
 for(int s=0;s<2;s++){float sg=s?1:-1,x=hx[h],z=hz[h];unsigned fl=g.things150[unit150(h,f,s)];
 lighting_block(x+sg*(WALL_HIT+ROOM_DOOR_A0)*.5f,y+1.5f,z+ROOM_WALL_B,(ROOM_DOOR_A0-WALL_HIT)*.5f,1.5f,.09f);lighting_block(x+sg*(ROOM_DOOR_A1+HOUSE_HALF_W)*.5f,y+1.5f,z+ROOM_WALL_B,(HOUSE_HALF_W-ROOM_DOOR_A1)*.5f,1.5f,.09f);
 if(!(fl&(1u<<17)))lighting_block(x+sg*ROOM_DOOR_A,y+1.1f,z+ROOM_WALL_B,(ROOM_DOOR_A1-ROOM_DOOR_A0)*.5f,1.1f,.04f);
 lighting_block(x+sg*BATH_A,y+1.5f,z+(FLAT_ENTRY_B+BATH_DOOR_B0)*.5f,.10f,1.5f,(BATH_DOOR_B0-FLAT_ENTRY_B)*.5f);lighting_block(x+sg*BATH_A,y+1.5f,z+(BATH_DOOR_B1+BATH_B)*.5f,.10f,1.5f,(BATH_B-BATH_DOOR_B1)*.5f);
 lighting_block(x+sg*4,y+1.5f,z+.5f,.8f,1.5f,.09f);
 if(flat_lit180(h,f,s)){U32 lc=flat_color180(h,f,s);lighting_room(x+sg*2.4f,y+2.45f,z+2.5f,2.45f,1.95f,1.45f,lc);lighting_room(x+sg*1.55f,y+2.45f,z-.45f,1.4f,1.3f,1.20f,lc);lighting_room(x+sg*4,y+2.45f,z-.65f,.65f,1,1.10f,lc);}
 }
 lighting_room(hx[h]+6.6f,y+2.45f,hz[h]-4,1.6f,4,.85f,0xb8a77d);
 }
}
static float path_target160(int h){if(h<4)return -38.5f;if(h<8)return 8.5f;if(h<16)return (h==8||h==9||h==12||h==13)?-73.5f:-101.f;return (h==16||h==17||h==20||h==21)?48.5f:110.5f;}
static void district_draw160(void){
 surfaceTex=&mat_asphalt;
 box(-5,-.015f,-76,164,.03f,4.5f,0x55534f);box(-5,-.015f,72,164,.03f,4.5f,0x55534f);
 box(-86,-.015f,-74.5f,4.5f,.03f,63,0x55534f);box(78,-.015f,-74.5f,4.5f,.03f,63,0x55534f);
 box(-86,-.015f,57.5f,4.5f,.03f,29,0x55534f);box(78,-.015f,57.5f,4.5f,.03f,29,0x55534f);surfaceTex=0;
 for(int h=0;h<NH;h++){float x=hx[h]+7.55f,z=hz[h]-8.6f,t=path_target160(h);if(dst(camx,camz,x,z)>viewdist)continue;
 surfaceTex=&mat_pavement;paved_box202(x,.008f,(z+t)*.5f,2.1f,.016f,ab(z-t),0x827b75);surfaceTex=0;
 /* Only dynamic, ageing winter footprints are drawn. */
 }
 for(int p=0;p<PANEL_COUNT;p++){float x=panel_origin_x(p),z=panel_origin_z(p)-9.4f;if(dst(camx,camz,x,z)>viewdist+15)continue;
 surfaceTex=&mat_pavement;box(x,.008f,z,30,.016f,1.6f,0x827b75);surfaceTex=0;
 }

}
