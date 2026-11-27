/* LAN lobby transport: one host, one client, TCP over Radmin/other virtual LAN. */
#ifdef HEADLESS
static char netAddress[16]="",netError[96]="";static int netAddressLen=0,netRole=0,netStatus=0;
static void net_host(void){}static void net_join(void){}static void net_disconnect(void){}static void net_tick(void){}static void net_key_input(void){}static int net_load(void){return 0;}static const char*net_status_text(void){return "Сетевая игра доступна только в Windows-сборке.";}
#else
typedef unsigned long long NetSocket;
typedef unsigned long NetUlong;
typedef struct { short family; unsigned short port; unsigned long address; char zero[8]; } NetSockAddr;
#define NET_INVALID ((NetSocket)~0ULL)
static void *netLib=0;
static int (*netWSAStartup)(unsigned short,void*),(*netWSACleanup)(void),(*netWSAGetLastError)(void);
static NetSocket (*netSocket)(int,int,int),(*netAccept)(NetSocket,void*,int*);
static int (*netBind)(NetSocket,const void*,int),(*netListen)(NetSocket,int),(*netConnect)(NetSocket,const void*,int);
static int (*netClose)(NetSocket),(*netIoctl)(NetSocket,long,NetUlong*),(*netGetOpt)(NetSocket,int,int,char*,int*);
static int (*netSend)(NetSocket,const char*,int,int),(*netRecv)(NetSocket,char*,int,int);
static NetSocket netListenSocket=NET_INVALID,netPeerSocket=NET_INVALID;
static int netRole=0,netStatus=0,netWsaReady=0,netHandshake=0,netHelloSent=0,netRxCount=0;static char netRx[4];
static char netAddress[16]="",netError[96]="";
static int netAddressLen=0;
static void net_close_socket(NetSocket *s){if(*s!=NET_INVALID){if(netClose)netClose(*s);*s=NET_INVALID;}}
static void net_set_error(const char*s){int n=0;while(s[n]&&n<95){netError[n]=s[n];n++;}netError[n]=0;}
static int net_load(void){
 if(netWsaReady)return 1;
 if(!netLib)netLib=imp_LoadLibraryA("ws2_32.dll");
 if(!netLib)return 0;
 #define NET_LOAD(var,sym) *(void**)(&var)=imp_GetProcAddress(netLib,sym)
 NET_LOAD(netWSAStartup,"WSAStartup");NET_LOAD(netWSACleanup,"WSACleanup");NET_LOAD(netWSAGetLastError,"WSAGetLastError");NET_LOAD(netSocket,"socket");NET_LOAD(netAccept,"accept");NET_LOAD(netBind,"bind");NET_LOAD(netListen,"listen");NET_LOAD(netConnect,"connect");NET_LOAD(netClose,"closesocket");NET_LOAD(netIoctl,"ioctlsocket");NET_LOAD(netGetOpt,"getsockopt");NET_LOAD(netSend,"send");NET_LOAD(netRecv,"recv");
 #undef NET_LOAD
 if(!netWSAStartup||!netSocket||!netAccept||!netBind||!netListen||!netConnect||!netClose||!netIoctl||!netGetOpt||!netSend||!netRecv)return 0;
 char data[512];memset(data,0,sizeof(data));if(netWSAStartup(0x0202,data)!=0)return 0;netWsaReady=1;return 1;
}
static int net_nonblock(NetSocket s){NetUlong one=1;return netIoctl(s,0x8004667eL,&one)==0;}
static void net_disconnect(void){net_close_socket(&netPeerSocket);net_close_socket(&netListenSocket);netRole=netStatus=netHandshake=netHelloSent=netRxCount=0;}
static void net_host(void){net_disconnect();netError[0]=0;if(!net_load()){net_set_error("Не удалось загрузить сетевую библиотеку Windows.");return;}netListenSocket=netSocket(2,1,6);if(netListenSocket==NET_INVALID||!net_nonblock(netListenSocket)){net_set_error("Не удалось создать сетевой сокет.");net_disconnect();return;}NetSockAddr a;memset(&a,0,sizeof(a));a.family=2;a.port=0x7969;a.address=0;if(netBind(netListenSocket,&a,sizeof(a))!=0||netListen(netListenSocket,1)!=0){net_set_error("Порт 27001 занят или недоступен.");net_disconnect();return;}netRole=1;netStatus=1;}
static void net_join(void){net_disconnect();netError[0]=0;if(netAddressLen<7){net_set_error("Введи Radmin-IP друга.");return;}if(!net_load()){net_set_error("Не удалось загрузить сетевую библиотеку Windows.");return;}netPeerSocket=netSocket(2,1,6);if(netPeerSocket==NET_INVALID||!net_nonblock(netPeerSocket)){net_set_error("Не удалось создать сетевой сокет.");net_disconnect();return;}NetSockAddr a;memset(&a,0,sizeof(a));a.family=2;a.port=0x7969;unsigned int ip[4]={0,0,0,0};int part=0,val=0,has=0;for(int i=0;i<netAddressLen;i++){char c=netAddress[i];if(c=='.'){if(part>3||!has||val>255){net_set_error("Неверный IPv4-адрес.");net_disconnect();return;}ip[part++]=val;val=has=0;}else if(c>='0'&&c<='9'){val=val*10+c-'0';has=1;if(val>255){net_set_error("Неверный IPv4-адрес.");net_disconnect();return;}}else{net_set_error("Неверный IPv4-адрес.");net_disconnect();return;}}if(part!=3||!has||val>255){net_set_error("Неверный IPv4-адрес.");net_disconnect();return;}ip[3]=val;a.address=ip[0]|(ip[1]<<8)|(ip[2]<<16)|(ip[3]<<24);int r=netConnect(netPeerSocket,&a,sizeof(a));if(r==0){netStatus=3;netRole=2;netHelloSent=0;}else if(netWSAGetLastError&&netWSAGetLastError()==10035){netStatus=2;netRole=2;}else{net_set_error("Не удалось подключиться. Проверь IP и Radmin.");net_disconnect();}}
static void net_tick(void){
 if(netStatus==1&&netListenSocket!=NET_INVALID&&netPeerSocket==NET_INVALID){int len=sizeof(NetSockAddr);NetSockAddr a;NetSocket s=netAccept(netListenSocket,&a,&len);if(s!=NET_INVALID){netPeerSocket=s;net_nonblock(s);netStatus=3;netHandshake=0;}}
 if(netStatus==2&&netPeerSocket!=NET_INVALID){int err=0,len=sizeof(err);if(netGetOpt(netPeerSocket,0xffff,0x1007,(char*)&err,&len)==0){if(err==0)netStatus=3;else{net_set_error("Подключение отклонено. Проверь IP и порт.");net_disconnect();}}}
 if(netStatus==3&&netPeerSocket!=NET_INVALID){char b[16];if(netRole==2&&!netHelloSent){if(netSend(netPeerSocket,"OKNO",4,0)==4)netHelloSent=1;}int n=netRecv(netPeerSocket,b,sizeof(b),0);if(n==0){net_set_error("Соединение разорвано.");net_disconnect();return;}if(n>0&&!netHandshake){for(int i=0;i<n&&netRxCount<4;i++)netRx[netRxCount++]=b[i];if(netRxCount==4){if(netRole==1&&netRx[0]=='O'&&netRx[1]=='K'&&netRx[2]=='N'&&netRx[3]=='O'){netSend(netPeerSocket,"ACK!",4,0);netHandshake=1;}else if(netRole==2&&netRx[0]=='A'&&netRx[1]=='C'&&netRx[2]=='K'&&netRx[3]=='!')netHandshake=1;else{net_set_error("Неверный ответ удалённого клиента.");net_disconnect();}}}}
}
static const char*net_status_text(void){if(netStatus==1)return "Лобби создано. Ожидание игрока...";if(netStatus==2)return "Подключаемся...";if(netStatus==3)return netHandshake?"Игрок подключён.":"Соединение установлено.";return "Не подключено.";}
static void net_key_input(void){if(screen!=26||netRole||netStatus)return;for(int k=0;k<10;k++)if(pressed['0'+k]&&netAddressLen<15){netAddress[netAddressLen++]='0'+k;netAddress[netAddressLen]=0;}if(pressed[190]||pressed[110])if(netAddressLen<15){netAddress[netAddressLen++]='.';netAddress[netAddressLen]=0;}if(pressed[8]&&netAddressLen){netAddress[--netAddressLen]=0;}}

#endif /* HEADLESS */
