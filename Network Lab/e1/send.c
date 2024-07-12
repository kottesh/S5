#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0); // creating socket descriptor 
    if (sock_fd < 0) {
        perror("After socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network representation - long
                                              // using INADDR_ANY uses the localhost
    addr.sin_port = htons(0); // host to network representation - short

    if (bind(sock_fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) { // naming the socket
        perror("After binding");
        exit(EXIT_FAILURE);
    }

    // receiver side infos 
    struct sockaddr_in recv;
    recv.sin_addr.s_addr = htonl(INADDR_ANY);
    recv.sin_family = AF_INET;
    recv.sin_port = htons(7774);

    char *msg[5] = {
        "TXT 1",
        "TXT 2",
        "TXT 3",
        "TXT 4",
        "TXT 5"
    };


    char buffer[BUFFER_SIZE];
    int recv_msg_len = 0;

    for (int x = 0; x < 5; x++) {
        if (sendto(sock_fd, msg[x], strlen(msg[x]), 0, (const struct sockaddr*)&recv, sizeof(recv)) < 0) {
            perror("After sendto");
            exit(EXIT_FAILURE);
        }

        int addr_len = sizeof(recv);
        recv_msg_len = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv, (socklen_t *)&addr_len);
        if (recv_msg_len > 0) {
            buffer[recv_msg_len] = 0; 
            printf("Server Response: %s\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            recv_msg_len = 0;
        }
        sleep(2);
    }


    close(sock_fd);
    return 0;
}
