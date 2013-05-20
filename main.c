#include"client.h"
#include"stdio.h"
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
	}
}

int main(int argc,char *argv[])
{
	pthread_t tcp_client_id;
	int loveywm;
	loveywm = pthread_create(&tcp_client_id,NULL,create_tcpclient,NULL);
	if(loveywm != 0)
	{
		printf("create pthread error\r\n");
		return 1;
	}

	return 0;
}
