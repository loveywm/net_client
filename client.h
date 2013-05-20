#ifndef __CLIENT_H__
#define __CLIENT_H__

#include"stdio.h"
//#include"string.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include"strings.h"
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int  u32;

#define 	TURE                1
#define 	FALSE               0
#define 	SERVER_PORT         5005
#define         SERVER_IP           "192.168.40.20"
#define         SOCK_BUFFER_SIZE    1024
#define         TCP_SEND_TIMEOUT    5000
#define         TCP_RECV_TIMEOUT    5000

#define         MIN(_a_,_b_)    ((_a_) < (_b_) ? (_a_):(_b_))

//#define 	PORT 3054

//int client_sock;
//struct sockaddr_in client_addr;

u8 tcpclient_connect(void);
u8 connect_nonb(int sockfd, struct sockaddr *saptr, socklen_t salen, int nsec);
int tcpclient_send(int sockfd,void *pbuffer,int buflen);
u8  tcp_send(void *pBuffer,int Buflen);
int tcpclient_recv(int sockfd,void *pbuffer,int buflen);
u8  tcp_recv(void *pBuffer,int Buflen);


int GetMainTickCount(void);
#endif //client.h

