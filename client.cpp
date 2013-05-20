#include"client.h"

int client_sock;
struct sockaddr_in client_addr;

u8 tcpclient_connect(void)
{
	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(client_sock < 0)
	{
            printf("erron==%d\n",errno);
            return 0;
	}
	bzero(&client_addr,sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(SERVER_PORT);
        client_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        if(connect_nonb(client_sock,(struct sockaddr*)&client_addr,sizeof(struct sockaddr),8) != 0)
        {
            printf("tcpclient connect error\n");
            close(client_sock);
            return FALSE;
        }
        printf("tcpclient connect ok!");
	return TURE;
}

//无阻塞连接
u8 connect_nonb(int sockfd, struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int flags, ret;
    //socklen_t len;
    fd_set rset, wset;
    struct timeval tval;
    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    errno = 0;
    if ( (ret = connect(sockfd, saptr, salen)) < 0)
    {
        printf("connect errno ==%d\n",errno);
        printf("ret ==%d\n",ret);
        printf("EINPROGRESS ==%d\n",EINPROGRESS);
        //当错误码不为阻塞，就返回，如果是就继续执行下面的select
        if (errno != EINPROGRESS)
        {
           return 1;
        }
    }
    if(ret == 0)//成功的话就退出
    {
        goto DONE;
    }

    //阻塞检测
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;  //  block copy
    tval.tv_sec = nsec;
    tval.tv_usec = 0;
    if((ret = select(sockfd+1, &rset, &wset, NULL,&tval)) < 1)
    {
        printf("select errror\n");
        printf("select ret ==%d\n",ret);
        close(sockfd);
        errno = ETIMEDOUT;
        return 2;
    }
    //EISCONN:The socket is already connected.
    if(connect(sockfd, saptr, salen) < 0 && errno != EISCONN)
    {
        //connect failed
        printf("second connect error\n");
        close(sockfd);
        return 3;
    }

DONE:
    fcntl(sockfd, F_SETFL, flags);
    printf("connect ok !\n");
    return 0;
}
//发送处理
int tcpclient_send(int sockfd,void *pbuffer,int buflen)
{
    int nCurrentWriteSize = 0;
    int nWrittenSize = 0;
    int nTime = 0;
    u8 *pBufIndex = (u8 *)pbuffer;

    nTime = GetMainTickCount() + TCP_SEND_TIMEOUT;
    while((buflen > 0) && (GetMainTickCount() < nTime))
    {
        nCurrentWriteSize = MIN(SOCK_BUFFER_SIZE,buflen);
        nCurrentWriteSize = send(sockfd,pBufIndex,nCurrentWriteSize,0);
        if(nCurrentWriteSize > 0)
        {
            pBufIndex += nCurrentWriteSize;
            buflen -= nCurrentWriteSize;
            nWrittenSize += nCurrentWriteSize;
            continue;
        }
        else if(nCurrentWriteSize < 0)
        {
            printf("send errno==%d\n",errno);
            continue;
        }
    }
    return nWrittenSize;//返回发送多少数据
}

u8  tcp_send(void *pBuffer,int Buflen)
{
    int ret;
    ret = tcpclient_send(client_sock,pBuffer,Buflen);
    if(ret == Buflen)
    {
        return TURE;
    }
    else
    {
        return FALSE;
    }
}


int GetMainTickCount(void)
{
    static int __start_time = 0; //second
    struct timeval tv;

    gettimeofday(&tv, NULL);

    if (__start_time == 0)
        __start_time = tv.tv_sec;

    return ((tv.tv_sec - __start_time) * 1000 + tv.tv_usec / 1000);
}

//接受处理
int tcpclient_recv(int sockfd,void *pbuffer,int buflen)
{
    int nCurrentReadSize = 0;
    int nReadSize = 0;
    int nTime = 0;
    u8 *pBufIndex = (u8 *)pbuffer;

    nTime = GetMainTickCount() + TCP_RECV_TIMEOUT;
    while((buflen > 0) && (GetMainTickCount() < nTime))
    {
        nCurrentReadSize = MIN(SOCK_BUFFER_SIZE,buflen);
        nCurrentReadSize = recv(sockfd,pBufIndex,nCurrentReadSize,0);
        if(nCurrentReadSize > 0)
        {
            pBufIndex += nCurrentReadSize;
            buflen -= nCurrentReadSize;
            nReadSize += nCurrentReadSize;
            //nTime = GetMainTickCount() + TCP_RECV_TIMEOUT;//again set timeout
            continue;
        }
        else if(nCurrentReadSize < 0)
        {
            printf("send errno==%d\n",errno);
            //nTime = GetMainTickCount() + TCP_RECV_TIMEOUT;//again set timeout
            continue;
        }
    }
    return nReadSize;//返回发送多少数据
}

u8  tcp_recv(void *pBuffer,int Buflen)
{
    int ret;
    ret = tcpclient_recv(client_sock,pBuffer,Buflen);
    if(ret == Buflen)
    {
        return TURE;
    }
    else
    {
        return FALSE;
    }
}
