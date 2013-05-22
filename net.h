#ifndef NET_H
#define NET_H


#include"stdio.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include"strings.h"
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>
#include <string.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int  u32;

#define 	TURE                1
#define 	FALSE               0
#define         ETH_NAME            "wlan0"//"em1"
#define         MAC_SIZE            6
#define         LOVEYWM             0
//type of eth_t
#define         TYPE_ARP             0x0806
#define         TYPE_RARP            0x0835
#define         TYPE_DATA            0x0800
//value of op
#define         OP_ARP_REQ           0x0001
#define         OP_ARP_REP           0x0002
#define         OP_RARP_REQ          0x0003
#define         OP_RARP_REP          0x0004
//value of IP proto
#define         IP_ICMP              0x01
#define         IP_IGMP              0x02
#define         IP_TCP               0x06
#define         IP_UDP               0x11



#define         MY_IP                 "192.168.1.160"
#define         DST_IP                 "192.168.1.150"

#define         MIN(_a_,_b_)    ((_a_) < (_b_) ? (_a_):(_b_))
#define         UI_IP_UNMAKE(ip,a,b,c,d)    {(a)=((ip)>>24)&0xff;(b)=((ip)>>16)&0xff;(c)=((ip)>>8)&0xff;(d)=(ip)&0xff;}

#define         ETH_LEN     sizeof(struct eth_t)
#define         ARP_LEN     sizeof(struct arp_t)
#define         IP_LEN      sizeof(struct ip_t)


#pragma pack(1)
typedef struct eth_t {
        u8 dst_mac[6];
        u8 src_mac[6];
        u16 type;
}Eth_Head;//6+6+2=14byte

typedef struct arp_t {
        u16 hwtype;
        u16 ptype;
        u8  hlen;
        u8  plen;

        u16 op;
        u8  src_mac[6];
        u32 src_ip;
        u8  dst_mac[6];
        u32 dst_ip;
}Arp_Head;//2+2+1+1+2+6+4+6+4=28byte

typedef struct ip_t{
    u8  h_verlen;       //4位首部长度(低位)+4位IP版本号(高位)
    u8  tos;            //服务类型
    u16 total_len;      //16位总长度（字节）
    u16 ident;          //16位表识
    u16 flag_and_flags; //13位偏移+3位标志位
    u8  ttl;            //8位生存时间
    u8  proto;          //8位协议
    u16 checksum;       //16位IP首部校验和
    u32 srcIP;          //32位源IP
    u32 dstIP;          //32位目的IP
}Ip_head;



#pragma pack()

void mk_eth(u8 *data,u8 *dst_mac,u8 *src_mac,u16 type);//制作以太网头部
void mk_arp(u8 *data,u16 op,u8 *src_mac,u8 *src_ip,u8 *dst_mac,u8 *dst_ip);//制作arp头部
void mk_arp_num(u8 *data, u16 op, u8 *src_mac, u32 src_ip, u8 *dst_mac, u32 dst_ip);
int  arp_func(void);
int arp_attack_func(void);
int eth_arp_show_func(void);
int GetMainTickCount(void);

void show_mac_addr(u8 *addr);
#endif // NET_H
