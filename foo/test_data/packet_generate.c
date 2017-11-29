/*  
 * File:   main.c 
 * Author: tianshuai 
 * 
 * Created on 2011年11月29日, 下午10:34 
 * 
 * 主要实现：发送20个文本消息，然后再发送一个终止消息 
 */  
  
#include <stdio.h>  
#include <stdlib.h>  
  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  

#define PACKET_TYPE_INIT 1
#define PACKET_TYPE_END  2
#define PACKET_TYPE_DATA 3

int port=1234;  
int main(int argc, char** argv) {  
    int socket_descriptor; //套接口描述字  
    int iter=0;  
    char buf[80];  
    struct sockaddr_in address;//处理网络通信的地址  
  
    bzero(&address,sizeof(address));  
    address.sin_family=AF_INET;  
    address.sin_addr.s_addr=inet_addr("192.168.1.1");//这里不一样  
    address.sin_port=htons(port);  
    unsigned char type = 1;
    unsigned char flag = 1;
    unsigned short seq_no;
    unsigned char ipv4[] = {192, 168, 1, 1};
    int offset = 0;

    //创建一个 UDP socket  
    socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 数据报套接字（UDP协议）  
  
    for(iter=0;iter<=20;iter++)  
    {
        offset = 0;
        if(0 == iter)
        {
            type = PACKET_TYPE_INIT;
        }
        else if(20 == iter)
        {
            type = PACKET_TYPE_END;
        }
        else
        {
            type = PACKET_TYPE_DATA;
        }
        memcpy(buf + offset, &type, sizeof(type));
        offset += sizeof(type);

        memcpy(buf + offset, &flag, sizeof(flag));
        offset += sizeof(flag);
        
        seq_no = htons(iter);
        memcpy(buf + offset, &seq_no, sizeof(seq_no));
        offset += sizeof(seq_no);
        
        memcpy(buf + offset, ipv4, sizeof(ipv4));
        offset += sizeof(ipv4);
         
        /*int PASCAL FAR sendto( SOCKET s, const char FAR* buf, int len, int flags,const struct sockaddr FAR* to, int tolen);　　 
         * s：一个标识套接口的描述字。　 
         * buf：包含待发送数据的缓冲区。　　 
         * len：buf缓冲区中数据的长度。　 
         * flags：调用方式标志位。　　 
         * to：（可选）指针，指向目的套接口的地址。　 
         * tolen：to所指地址的长度。   
　　      */  
        sendto(socket_descriptor,buf,offset,0,(struct sockaddr *)&address,sizeof(address));  
    }  

    close(socket_descriptor);  
    printf("Messages Sent,terminating\n");  
  
    exit(0);  
  
    return (EXIT_SUCCESS);  
}
