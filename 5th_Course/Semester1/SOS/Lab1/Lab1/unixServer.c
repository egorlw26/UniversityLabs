#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/un.h>

char buffer[512];
char compare_buffer[512];

struct sockaddr_un addr_un;

struct sockaddr* addr;
int addrlen = sizeof(addr_un);

int main() 
{
    int blocking = 1;
    int s = 0;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        exit(1);
    }
    
    if (!blocking) {
        fcntl(s, F_SETFL, O_NONBLOCK);
    }

    addr_un.sun_family = AF_UNIX;
    strcpy(addr_un.sun_path, "./my_socket");
    unlink("./my_socket");

    addr = (struct sockaddr*)&addr_un;
    if (bind(s, addr, strlen(addr_un.sun_path) + sizeof (addr_un.sun_family)) < 0) {
        perror("Bind failed\n");
        exit(1);
    } else {
        printf("Bind successful\n");
    }
    if (listen(s, 8) < 0) {
        perror("Listen failed\n");
        exit(1);
    } else {
        printf("Listening\n");
    }
    
    socklen_t csize;
    int new_socket;
    while (1) {
        new_socket = accept(s, addr, &csize);
        if (new_socket < 0) {
            if (blocking == 1) {
                perror("Accept error");
                exit(1);
            } else {
                printf("Waiting for connection\n");
                sleep(1);
            }
        } else {
            printf("New connection!\n");
            int pid = fork();
            if (pid < 0) {
                perror("ERROR on fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                while (1) {
                    memset(buffer, 0, sizeof(buffer));
                    int val = read(new_socket, buffer, sizeof(buffer));
                    if (val == 0) {
                        break;
                    } else {
                        if (blocking && memcmp(buffer, compare_buffer, sizeof(buffer)) != 0) {
                            printf("Data corrupted\n");
                        }
                    }
                }
                close(new_socket);
                exit(0);
            } else {
                close(new_socket);
            }
        }
    }
    return 0;
}