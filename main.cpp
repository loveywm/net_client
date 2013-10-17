#include <QtCore/QCoreApplication>

#include"client.h"
//#include"stdio.h"
//#include"string.h"
#include<pthread.h>

static int tcpclient_connect_status = 0;

void *create_tcpclient(void *arg)
{
    u8 ret;
    while(1)
    {
        if(tcpclient_connect_status == 0)
        {
            ret = tcpclient_connect();
            if(ret == 0)
            {
                tcpclient_connect_status = 0;
            }
            else
            {
                tcpclient_connect_status = 1;
            }
        }
        sleep(3);
        printf("create_tcpclient\r\n");
        printf("tcpclient_connect_status ==%d\n",tcpclient_connect_status);
    }
}

int main(int argc,char *argv[])
{
    QCoreApplication a(argc, argv);

    //arp_func();
    //arp_attack_func();
    //eth_arp_show_func();
    ping_func();

/*    pthread_t tcp_client_id;
    int loveywm;
    loveywm = pthread_create(&tcp_client_id,NULL,create_tcpclient,NULL);
    if(loveywm != 0)
    {
        printf("create pthread error\r\n");
        return 1;
    }
    sleep(5);
    int count = 10;
    while(count--)
    {
        tcp_send(&count,4);
        sleep(1);
    }
    int pdata;
    char buf[4];
    while(1)
    {
        loveywm = tcp_recv(buf,4);
        if(loveywm == 1)
        {
            for(pdata=0;pdata<4;pdata++)
            {
                printf("pdate==%d\n",buf[pdata]);
            }

        }

    }
*/
	//xxx
	//xxx1
    return a.exec();
    //return 0;
}
