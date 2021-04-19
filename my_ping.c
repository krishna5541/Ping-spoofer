//////////////////////////////////////// BY KRISHNA PRABHAKAR ////////////////////////////////// CSB18042 //////////////////////////////////////////////// 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/ip_icmp.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<netinet/ip.h>
#include "my_header.h"



int main(int argc,char* argv[])
{
    struct sockaddr_in info;
    int num,delay,pktsize;
	unsigned long dest_ip;
	unsigned long saddr;
	char temp[50];

    if(argc==3){
		saddr = inet_addr(argv[1]);
		dest_ip = inet_addr(argv[2]);

	}

    else{
        printf("Enter Source ip for packet (x.x.x.x): ");
		scanf("%s",temp);
		saddr = inet_addr(temp);
		printf("Enter Destinetion ip for packet (x.x.x.x): ");
		scanf("%s",temp);
		dest_ip = inet_addr(temp);
		
    }
    
    num = 5;				// NUMBER OF PACKETS
    delay = 1000000;		// IN MICROSECONDS
    pktsize = 64;			// IN BYTES
	int sock, on = 1;
    if((sock=socket(AF_INET,SOCK_RAW,IPPROTO_RAW))<0)
    {
        printf("error getting socket");
        exit(-1);
    }
    if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}
    
	if (setsockopt (sock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}
    int packet_size=sizeof(struct iphdr)+sizeof(struct icmphdr)+pktsize;
    char *packet=malloc(packet_size);
    //Headers
    struct iphdr *ip=(struct iphdr*)packet;
    struct icmphdr *icmp=(struct icmphdr*)(packet+sizeof(struct iphdr));
	memset (packet, 0, packet_size);
    ip->version=4;
    ip->ihl=5;
	ip->tos = 0;
	ip->tot_len = htons (packet_size);
	ip->id = rand ();
	ip->frag_off = 0;
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = saddr;
	ip->daddr = dest_ip;
	ip->check = my_chksm ((u_int16_t *) ip, sizeof (struct iphdr));

    //ICMP
    icmp->type = ICMP_ECHO;
	icmp->code = 0;
  	icmp->un.echo.sequence = rand();
  	icmp->un.echo.id = rand();
	icmp->checksum = 0;
    info.sin_family=AF_INET;
    info.sin_port=htons(0);
    info.sin_addr.s_addr=dest_ip;
    memset(info.sin_zero,0,sizeof(info.sin_zero));
    int psnt_sz,i=0;
    while (i<num)
	{
		memset(packet + sizeof(struct iphdr) + sizeof(struct icmphdr), rand() % 255, pktsize);
		
		icmp->checksum = 0;
		icmp->checksum = my_chksm((unsigned short *)icmp, sizeof(struct icmphdr) + pktsize);
		
		if ( (psnt_sz = sendto(sock, packet, packet_size, 0, (struct sockaddr*) &info, sizeof (info))) < 1) 
		{
			perror("send failed\n");
			break;
		}
        i += 1;
		printf("%d packets sent\n", i);
		fflush(stdout);
		usleep(delay);	
	}

	
    return 0;
}