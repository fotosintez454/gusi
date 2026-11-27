/* One measured cylindrical mesh shared by the player and smoking residents.
   Axis follows the hand; no world-axis rectangular beam or giant ember billboard. */
static V3 cig_add201(V3 a,V3 b,float t){return vp(a.x+b.x*t,a.y+b.y*t,a.z+b.z*t);}
static V3 cig_cross201(V3 a,V3 b){return vp(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
static V3 cig_unit201(V3 a){float l=(float)sqrt(a.x*a.x+a.y*a.y+a.z*a.z);return l>.00001f?vp(a.x/l,a.y/l,a.z/l):vp(0,0,1);}
static V3 cig_ring201(V3 center,V3 r,V3 up,float angle,float radius){return cig_add201(cig_add201(center,r,cos(angle)*radius),up,sin(angle)*radius);}
static void cig_tube201(V3 a,V3 b,V3 right,V3 up,float ra,float rb,U32 color){for(int k=0;k<12;k++){float t=k*PI/6,tt=(k+1)*PI/6;quadstrip(cig_ring201(a,right,up,t,ra),cig_ring201(a,right,up,tt,ra),cig_ring201(b,right,up,tt,rb),cig_ring201(b,right,up,t,rb),shade(color,.86f+.14f*(float)sin(t)));}}
static void cig_cap201(V3 center,V3 right,V3 up,float radius,U32 color,int glowing){for(int k=0;k<12;k++){V3 a=cig_ring201(center,right,up,k*PI/6,radius),b=cig_ring201(center,right,up,(k+1)*PI/6,radius);if(glowing)glow150(center,a,b,b,color);else quadstrip(center,a,b,b,color);}}
/* Soft, depth-tested translucent wisps, never solid debris-shaped beams. */
static void cig_smoke201(V3 at,float size,float alpha){PV p=transform(at.x,at.y,at.z,0,0);if(p.z<.10f)return;
#ifndef HEADLESS
 if(gpuActive){gpu_flush_world();gpu_projection();glDisable(0x0de1);glDisable(0x0b60);glEnable(0x0be2);glBlendFunc(0x0302,0x0303);glDepthMask(0);glBegin(4);for(int k=0;k<10;k++){float a=k*PI/5,b=(k+1)*PI/5;glColor4ub(185,193,187,(unsigned char)(alpha*255));glVertex3f(p.x,p.y,-p.z);glColor4ub(185,193,187,0);glVertex3f(p.x+cos(a)*size,p.y+sin(a)*size,-p.z);glVertex3f(p.x+cos(b)*size,p.y+sin(b)*size,-p.z);}glEnd();glDepthMask(1);glDisable(0x0be2);glEnable(0x0b60);return;}
#endif
 float cx=W*.5f+p.x*focal/p.z,yy=H*.5f-p.y*focal/p.z,r=clamp(size*focal/p.z,1,32),iz=1/p.z;
 int lo=(int)mx(0,cx-r),hi=(int)mn(W-1,cx+r),top=(int)mx(0,yy-r),bottom=(int)mn(H-1,yy+r);
 for(int y=top;y<=bottom;y++)for(int x=lo;x<=hi;x++){float dx=(x+.5f-cx)/r,dy=(y+.5f-yy)/r,edge=1-dx*dx-dy*dy;int i=y*W+x;if(edge>0&&iz>=depth[i])frame[i]=mix(frame[i],0xb9c1bb,alpha*edge*edge);}
}
static void cigarette141(V3 a,V3 b,int lit){
 surfaceTex=0;V3 axis=cig_unit201(vp(b.x-a.x,b.y-a.y,b.z-a.z));V3 right=cig_unit201(cig_cross201(axis,ab(axis.y)>.92f?vp(1,0,0):vp(0,1,0))),up=cig_cross201(right,axis);
 V3 filter=blend3(a,b,.29f),seam=blend3(a,b,.31f),paper=blend3(a,b,.90f),ash=blend3(a,b,.975f);
 cig_tube201(a,filter,right,up,.0038f,.0038f,0xc09761);
 cig_tube201(filter,seam,right,up,.0039f,.0039f,0xb99c62);
 cig_tube201(seam,paper,right,up,.0037f,.0037f,0xe9e4d3);
 cig_tube201(paper,ash,right,up,.0038f,.0035f,0x929589);
 cig_tube201(ash,b,right,up,.0035f,.0031f,lit?0xa44b29:0x625e4e);
 cig_cap201(a,right,up,.0038f,0xcebc91,0);
 cig_cap201(b,right,up,.0031f,lit?0xe18a43:0x655d4a,lit);
 /* Sparse cork flecks, laid on the filter surface rather than broad rings. */
 for(int k=0;k<9;k++){float t=.045f+(k%3)*.075f,ang=(k*2.39996f);V3 c=cig_ring201(blend3(a,b,t),right,up,ang,.00384f),d=cig_add201(c,axis,.00065f);V3 side=cig_add201(vp(0,0,0),right,.00030f);quadstrip(cig_add201(c,side,-1),cig_add201(c,side,1),cig_add201(d,side,1),cig_add201(d,side,-1),0x997349);}
 if(lit&&g.uiMotion)for(int k=0;k<3;k++){float t=(float)fmod(realTime*.30f+k*.31f,1);V3 q=vp(b.x+(float)sin(realTime*.8f+k)*.012f*t,b.y+.01f+t*.13f,b.z+(float)cos(realTime*.7f+k)*.012f*t);cig_smoke201(q,.006f+t*.009f,.18f*(1-t));}
}
