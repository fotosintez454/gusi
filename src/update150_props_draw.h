static void props_draw150(void){if(!propCount150)props_init150();for(int n=0;n<propCount150;n++){Prop150*p=&props150[n];float x=p->x,z=p->z;if(dst(camx,camz,x,z)>viewdist+5)continue;surfaceTex=0;
#include "update151_props_draw.h"
 if(p->type==0)snowman(x,z);
 else if(p->type==1)tire_bed(x,z,n);
 else if(p->type==2){beam(vp(x,0,z),vp(x,p->height,z),.065f,.065f,0x676c59);if(n%4!=3)beam(vp(x,2.35f,z),vp(x+3,2.35f,z),.04f,.04f,0x77715e);if(n>=12)beam(vp(x,1.12f,z),vp(x,1.12f,z-3),.04f,.04f,0x827b75);}
 else if(p->type==3){box(x,.16f,z-p->rz,4,.32f,.16f,0x827b65);box(x,.16f,z+p->rz,4,.32f,.16f,0x827b65);for(int e=-1;e<=1;e+=2)box(x+e*p->rx,.16f,z,.16f,.32f,3.4f,0x827b65);box(x,.04f,z,3.8f,.08f,3.2f,winterEnabled?0xa6a9a5:0x9c977f);}
 else if(p->type==4||p->type==11){garage203(p,n);}
 else if(p->type==5)vehicle190(x,z,n);
 }
 /* Containers are grouped two per permanent fenced pad, with a clear service side. */
 for(int n=0;n<15;n++){float x,z;loot_position(n*2,&x,&z);x+=1.35f;if(dst(camx,camz,x,z)>viewdist)continue;surfaceTex=&mat_concrete;box(x,.015f,z,4.4f,.03f,2.3f,0x777d80);surfaceTex=&mat_metal;box(x,.8f,z+.85f,4.4f,1.6f,.12f,0x666e63);for(int e=-1;e<=1;e+=2)box(x+e*2.16f,.8f,z,.12f,1.6f,1.8f,0x666e63);surfaceTex=0;box(x-1.3f,.08f,z-.8f,.8f,.16f,.5f,0x827b65);if(n%3==0)box(x,2.35f,z,4.7f,.13f,2.5f,0x66716c);}

}
