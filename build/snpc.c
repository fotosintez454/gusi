#define HEADLESS
#define main original_main
#include "../src/main.c"
#undef main
int main(void){savepath="build/dj.save";eventpath="build/dj.csv";audio_init();nextWorldSeed=140;test_reset();set_resolution(2);
 char p[240];SN(p,sizeof(p),"E — спросить у бабок про двор");printf("len=%d strstr=%d\n",(int)strlen(p),strstr(p,"бабк")!=0);
 SN(p,sizeof(p),"E — спросить у бабок про двор");printf("qa strstr=%d\n",strstr(p,"бабк")!=0);
 return 0;}
