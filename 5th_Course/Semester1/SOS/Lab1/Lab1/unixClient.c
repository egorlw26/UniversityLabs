#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>

uint64_t start;
#define TimeTook(exp, name) \
    start = getTime();\
    exp;\
    printf("%s took: %.06f\n", name, (getTime() - start)/1000000.0);\

char data[512 + 1];

uint64_t getTime() 
{
    struct timespec tms;
    if (clock_gettime(CLOCK_REALTIME,&tms)) return -1;
    return tms.tv_sec * 1000000 + tms.tv_nsec/1000;
}

void sendPackets(int s) 
{
    for (int i = 0; i < 1024*128*16ll; i++) {
        long int val = write(s, data, 512);
    }
}

int main() 
{
    int blocking = 0;
    srand(0);
    for(int i = 0; i < 512; i++) {
        data[i] = rand()%26 + 'a';
    }
    data[512] = 0;
    
    TimeTook(0, "SOCKET_OPEN");

    int s = 0;
    struct sockaddr_un serv_addr;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        exit(1);
    }
    
    if (!blocking) {
        fcntl(s, F_SETFL, O_NONBLOCK);
    }

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "my_socket");
    if (connect(s, (struct sockaddr *)&serv_addr, strlen(serv_addr.sun_path) + sizeof (serv_addr.sun_family)) < 0) {
        printf("Connection Failed\n");
        exit(1);
    }

    printf("Start\n");
    TimeTook(sendPackets(s), "DATA TRANSFER");
    printf("Sent\n");
    TimeTook(close(s), "SOCKET_CLOSE");

    return 0;
}