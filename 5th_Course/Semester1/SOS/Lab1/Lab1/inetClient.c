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
    srand(0);
    for(int i = 0; i < 512; i++) {
        data[i] = rand()%26 + 'a';
    }
    data[512] = 0;

    int s;
    struct sockaddr_in serv_addr;
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(54000);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        exit(1);
    }

    clock_t start = clock();
    if (connect(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed \n");
        exit(1);
    }
    
    TimeTook(0, "SOCKET_OPEN");
    printf("Start\n");
    TimeTook(sendPackets(s), "DATA TRANSFER");
    printf("Sent\n");
    TimeTook(close(s), "SOCKET_CLOSE");

    return 0;
}