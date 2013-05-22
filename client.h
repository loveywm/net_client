#ifndef __CLIENT_H__
#define __CLIENT_H__

#include"net.h"


#define 	SERVER_PORT         5005
#define         SERVER_IP           "192.168.1.150"
#define         SOCK_BUFFER_SIZE    1024
#define         TCP_SEND_TIMEOUT    5000
#define         TCP_RECV_TIMEOUT    5000



u8  tcpclient_connect(void);
u8  connect_nonb(int sockfd, struct sockaddr *saptr, socklen_t salen, int nsec);
int tcpclient_send(int sockfd,void *pbuffer,int buflen);
u8  tcp_send(void *pBuffer,int Buflen);
int tcpclient_recv(int sockfd,void *pbuffer,int buflen);
u8  tcp_recv(void *pBuffer,int Buflen);




#endif //client.h

