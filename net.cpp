#include"net.h"


int GetMainTickCount(void)
{
    static int __start_time = 0; //second
    struct timeval tv;

    gettimeofday(&tv, NULL);

    if (__start_time == 0)
        __start_time = tv.tv_sec;

    return ((tv.tv_sec - __start_time) * 1000 + tv.tv_usec / 1000);
}



void mk_eth(u8 *data, u8 *dst_mac, u8 *src_mac, u16 type)
{
        struct eth_t *eth = (struct eth_t *)data;

        memcpy(eth->dst_mac, dst_mac, 6);
        memcpy(eth->src_mac, src_mac, 6);
        eth->type = htons(type);
}

void mk_arp(u8 *data, u16 op, u8 *src_mac, u8 *src_ip, u8 *dst_mac, u8 *dst_ip)
{
        struct arp_t *arp = (struct arp_t *)data;

        arp->hwtype = htons(1);
        arp->ptype = htons(0x0800);//IP address
        arp->hlen = 6;
        arp->plen = 4;

        arp->op = htons(op);//1:arp request;2:arp reply
        memcpy(arp->src_mac, src_mac, 6);
        arp->src_ip = inet_addr((const char *)src_ip);
        memcpy(arp->dst_mac, dst_mac, 6);
        arp->dst_ip = inet_addr((const char *)dst_ip);
}

void mk_arp_num(u8 *data, u16 op, u8 *src_mac, u32 src_ip, u8 *dst_mac, u32 dst_ip)
{
        struct arp_t *arp = (struct arp_t *)data;

        arp->hwtype = htons(1);
        arp->ptype = htons(0x0800);//IP address
        arp->hlen = 6;
        arp->plen = 4;

        arp->op = htons(op);//1:arp request;2:arp reply
        memcpy(arp->src_mac, src_mac, 6);
        arp->src_ip = src_ip;
        memcpy(arp->dst_mac, dst_mac, 6);
        arp->dst_ip = dst_ip;
}

void show_mac_addr(u8 *addr)
{
        int i;
        for (i = 0; i < 6; i++)
                printf(":%02x", addr[i]);
}
