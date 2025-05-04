#include "packet.h"

#define PROXY_PORT 8080
#define RECEIVER_PORT 8081
#define SERVER_IP "127.0.0.1"

using namespace std;

bool shouldDropPacket()
{return (rand() % 100) < 10;} //10%chance

void randomDelay() 
{
    if ((rand() % 100) < 20)
    { usleep((rand() % 500) * 1000);}
}

int main()
{
    srand(time(0));

    int listenfd , clientfd , receiverfd;
    struct sockaddr_in proxyaddr , clientaddr ,  receiveraddr;
    socklen_t addrlen=sizeof(struct sockaddr_in);

    listenfd=socket(AF_INET, SOCK_STREAM,0);
    proxyaddr.sin_family=AF_INET;
    proxyaddr.sin_addr.s_addr=INADDR_ANY;
    proxyaddr.sin_port=htons(PROXY_PORT);
    bind(listenfd, (struct sockaddr*)&proxyaddr, sizeof(proxyaddr));
    listen(listenfd, 5);



    cout<<"Proxy waiting for client...\n";
    clientfd=accept(listenfd,(struct sockaddr*)&clientaddr,&addrlen);
    cout<<"Client connected.\n";

    receiverfd=socket(AF_INET, SOCK_STREAM, 0);
    receiveraddr.sin_family=AF_INET;
    receiveraddr.sin_port=htons(RECEIVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&receiveraddr.sin_addr);

    connect(receiverfd, (struct sockaddr*)&receiveraddr, sizeof(receiveraddr)) ;
    cout<<"Connected to receiver.\n";

    fd_set readfds;
    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(clientfd,&readfds);
        FD_SET(receiverfd,&readfds);

        select(max(clientfd,receiverfd)+1,&readfds,NULL,NULL,NULL);

        if (FD_ISSET(clientfd , &readfds))
        {
            Packet pkt;
            int n=recv(clientfd , &pkt , sizeof(Packet) , 0);
            if (n<=0) break;

            if (shouldDropPacket()) 
            {
                cout<<"Dropped packet with seq: "<<pkt.seq_num<<endl;
                continue;
            }

            randomDelay();
            send(receiverfd , &pkt  , sizeof(Packet) , 0);
            cout<<"Forwarded packet with seq: "<<pkt.seq_num<<endl;
        }

        if (FD_ISSET(receiverfd , &readfds))
        {
            ACK ack;
            int n=recv(receiverfd , &ack, sizeof(ACK), 0);
            if (n<=0) break;

            send(clientfd,  &ack , sizeof(ACK) , 0);
            cout<< "Forwarded ACK: "<<ack.ack_num<<endl;
        }
    }

    close(clientfd);
    close(receiverfd);
    close(listenfd);

    return 0;
}
