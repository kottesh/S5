#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 7774 

int main(int argc, char **argv) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock_fd < 0) {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sock_fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind transport address");
        exit(EXIT_FAILURE);
    }

    const int BUF_SIZE = 4096;
    char buffer[BUF_SIZE];

    struct sockaddr_in sender_addr;
    int sender_len = sizeof(sender_addr);

    // recvfrom() returns the number of bytes that have been read into the `buffer`
    int recvlen = 0;
    const char *recv_msg = "RECEIVED";
    while (1) {
        recvlen = recvfrom(sock_fd, buffer, BUF_SIZE, 0, (struct sockaddr *)&sender_addr, (socklen_t *)&sender_len);
        if (recvlen > 0) {
            buffer[recvlen] = 0;
            printf("Message: %s\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            recvlen = 0;
            if (sendto(sock_fd, recv_msg, strlen(recv_msg), 0, (const struct sockaddr*)&sender_addr, (socklen_t)sender_len) < 0) {
                perror("After sendto");
                exit(EXIT_FAILURE);
            }
        }
    }

    sendto(sock_fd, buf, BUF_SIZE, 0, (struct sockaddr *)&addr, (socklen_t)sock_len);

    close(sock_fd);
    return 0;
}

