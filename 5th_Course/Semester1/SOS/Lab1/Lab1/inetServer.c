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
struct sockaddr_in addr_in;

struct sockaddr* addr;
int addrlen = sizeof(addr_in);

int main() 
{
    int blocking = 1;
    int s;
    int opt = 1;
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Can't create socket");
        exit(EXIT_FAILURE);
    }

    if (!blocking) {
        fcntl(s, F_SETFL, O_NONBLOCK);
    }

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(1);
    }
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(54000);
    addr = (struct sockaddr*)&addr_in;
    if (bind(s, addr, sizeof(addr_in)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    if (listen(s, 10) < 0) {
        perror("Listen...");
        exit(1);
    }

    socklen_t csize;
    int new_socket;
    while (1) {
        new_socket = accept(s, addr, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            if (blocking == 1) {
                perror("Accepting failed");
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
                            printf("Data not as expected\n");
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