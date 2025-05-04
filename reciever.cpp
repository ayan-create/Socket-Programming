#include "packet.h"
#define RECEIVER_PORT 8081

using namespace std;

int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    listenfd=socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_port=htons(RECEIVER_PORT);

    bind(listenfd , (struct sockaddr*)&servaddr , sizeof(servaddr));
    listen(listenfd,5);

    cout<<"Receiver waiting for proxy...\n";
    connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&addrlen);
    cout<<"Proxy connected.\n";

    int expected_seq  =  0;

    while (true) 
    {
        Packet pkt;
        int n=recv(connfd,&pkt,sizeof(Packet),0);
        if (n<=0) break;

        cout<<"Received packet with seq: "<<pkt.seq_num<<endl;

        ACK ack;
        if (pkt.seq_num==expected_seq)
        {
            expected_seq+=pkt.length;
            ack.ack_num=expected_seq;
            cout<<"Accepted. Sending ACK: "<<ack.ack_num<<endl;
        }
        else
        {
            ack.ack_num=expected_seq;
            cout<<"Out of order. Resending ACK: "<<ack.ack_num<<endl;
        }

        send(connfd , &ack , sizeof(ACK) , 0);
    }

    close(connfd);
    close(listenfd);

    return 0;
}
