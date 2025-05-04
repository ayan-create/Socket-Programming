
#include "packet.h"
#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define WINDOW_SIZE 5
#define TOTAL_PACKETS 10
#define TIMEOUT_SEC 1
using namespace std;

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd=socket(AF_INET, SOCK_STREAM,0);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&servaddr.sin_addr);
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    //create packets and dup counters and which acks recieved
    Packet packets[TOTAL_PACKETS];
    int dup_ack_counter[TOTAL_PACKETS]={0};
    bool packet_acked[TOTAL_PACKETS]={false};

    for (int i = 0; i < TOTAL_PACKETS; i++)
    {
        packets[i].seq_num = i * MAX_DATA_SIZE;
        packets[i].length = MAX_DATA_SIZE;
        memset(packets[i].data,'A'+(i%26), MAX_DATA_SIZE);
    }

    int base=0;
    int next_seq=0;

    while (base < TOTAL_PACKETS) 
    {
        //send packets within window
        while (next_seq < base + WINDOW_SIZE && next_seq < TOTAL_PACKETS)
        {
            if (!packet_acked[next_seq])//SEND PACKETS ALREADY ACKED
            {
                send(sockfd, &packets[next_seq], sizeof(Packet), 0);
                cout << "Sent packet with seq: " << packets[next_seq].seq_num << endl;
            }
            next_seq++;
        }


        // Set timeout for receiving ACKs
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        //these all for timeout implementation

        if (select(sockfd+1,&readfds,NULL,NULL,&timeout)>0) //check for timeout
        {
            //receive ACK
            ACK ack;
            int n=recv(sockfd, &ack, sizeof(ACK), 0);
            if (n>0)
            {
                cout<<"Received ACK for: "<<ack.ack_num<<endl;

                int acked_packet=ack.ack_num/MAX_DATA_SIZE-1;

                if (acked_packet>=base && acked_packet<TOTAL_PACKETS)
                {
                        packet_acked[acked_packet] = true;
                        while (base < TOTAL_PACKETS && packet_acked[base])
                        {
                            dup_ack_counter[base] = 0;
                            base++;
                        }
                        next_seq = base; // Reset next_seq to base for window sliding
                } 
                else if (acked_packet >= 0 && acked_packet < base)
                {
                        dup_ack_counter[acked_packet]++;
                        if (dup_ack_counter[acked_packet] == 3)
                        {
                            // Fast retransmit
                            cout << "Fast retransmit for packet: " << packets[acked_packet].seq_num << endl;
                            send(sockfd, &packets[acked_packet], sizeof(Packet), 0);
                            dup_ack_counter[acked_packet] = 0;
                        }
                }
            }
        } 
        else
        {
            cout<<"Timeout occurred. Resending window starting from: "<<base<<endl;
            next_seq=base;
        }
    }

    close(sockfd);
    cout<<"All packets sent and acknowledged successfully!"<<endl;
    return 0;
}