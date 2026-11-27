#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/m.c.save";eventpath="build/m.c.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(0);
 for(int j=143;j>=0;j-=3){printf("%4d|",j);for(int i=0;i<192;i+=1){float x=-192+i*2,z=-144+j*2;float roof;int s=solid(x,z);char c=s?'#':'.';
   if(ab(x)<264&&ab(z)<6)c=s?'#':'-';
   if(dst(x,z,g.x,g.z)<2)c='P';
   putchar(c);}putchar('\n');}
 printf("POIs: home %.1f %.1f shop %.1f %.1f office %.1f %.1f kiosk %.1f %.1f\n",hx[0],hz[0],shopX,shopZ,OX,OZ,KX,KZ);
 return 0;}
