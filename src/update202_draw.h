/* Pavement never floats over the river: only the three modelled wooden bridges cross it. */
static void paved_box202(float x,float y,float z,float w,float h,float d,U32 c){
 if(z+d*.5f<78||z-d*.5f>94||x-w*.5f>141||x+w*.5f< -141){box(x,y,z,w,h,d,c);return;}
 int slices=(int)clamp((float)floor(w)+1,1,400);float step=w/slices;
 for(int n=0;n<slices;n++){float cx=x-w*.5f+(n+.5f)*step;
 if(ab(cx)>140){box(cx,y,z,step,h,d,c);continue;}
 float river=river_z(cx),lo=z-d*.5f,hi=z+d*.5f,south=mn(hi,river-4.15f-step*.04f),north=mx(lo,river+4.15f+step*.04f);
 if(south>lo)box(cx,y,(lo+south)*.5f,step,h,south-lo,c);
 if(hi>north)box(cx,y,(north+hi)*.5f,step,h,hi-north,c);
 }
}
static void lift_digit202(float x,float y,float z,int digit){
 static const unsigned char masks[10]={63,6,91,79,102,109,125,7,127,111};int mask=masks[digit%10];
 for(int s=0;s<7;s++)if(mask&(1<<s)){float xx=x,yy=y,w=.075f,h=.013f;if(s==0)yy+=.085f;if(s==3)yy-=.085f;if(s==6)yy+=0;
 if(s==1||s==2||s==4||s==5){xx+=(s==1||s==2)?-.042f:.042f;yy+=(s==1||s==5)?.043f:-.043f;w=.013f;h=.07f;}
 box(xx,yy,z,w,h,.012f,0x9df5b5);}
}
static void lift_display202(float x,float y,float z,float height,float target){if(dst(camx,camz,x,z)>16)return;const Texture*old=surfaceTex;surfaceTex=0;int floorNumber=(int)floor((height+1.5f)/3)+1;
 box(x,y,z,.43f,.29f,.025f,0x1b302d);if(floorNumber>=10)lift_digit202(x+.07f,y,z+.020f,floorNumber/10);lift_digit202(x+(floorNumber>=10?-.06f:.01f),y,z+.02f,floorNumber%10);
 if(ab(target-height)>.02f){float dy=target>height?.03f:-.03f;quadstrip(vp(x+.135f,y-dy,z+.03f),vp(x+.195f,y-dy,z+.03f),vp(x+.165f,y+dy,z+.03f),vp(x+.165f,y+dy,z+.03f),0xb8e6a4);}surfaceTex=old;
}
