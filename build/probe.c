#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/p.save";eventpath="build/p.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);props_init150();
 float px[]={58,-56,36,14,-36,-10,4,44,-2,-24,-160,0,70,-70,120,-120,100,-100};float pz[]={-30,-9.2f,-10.5f,-12,-34,-34.5f,-33,17.5f,-42,-40,-6,0,-9,9,-9,9,-9,9};
 for(int i=0;i<18;i++){float x=px[i],z=pz[i];printf("%2d %7.1f %7.1f solid=%d outside=%d\n",i,x,z,solid(x,z),outside141(x,z,2.5f));}
 printf("KX %.1f KZ %.1f OX %.1f OZ %.1f\n",KX,KZ,OX,OZ);
 for(int n=0;n<propCount150;n++)if(ab(props150[n].z)<70&&ab(props150[n].x)<80)printf("prop t%d %.1f %.1f r%.1f/%.1f h%.1f\n",props150[n].type,props150[n].x,props150[n].z,props150[n].rx,props150[n].rz,props150[n].height);
 for(int n=0;n<12;n++){printf("lamp n%d\n",n);}
 printf("entry0 %.1f %.1f\n",hx[0],hz[0]-4.1f);
 return 0;}
