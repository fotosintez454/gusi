#include <stdio.h>
#include <string.h>
int main(void){char p[240];snprintf(p,sizeof(p),"E — спросить у бабок про двор");printf("len=%d find=%d\n",(int)strlen(p),strstr(p,"бабк")!=NULL);return 0;}
