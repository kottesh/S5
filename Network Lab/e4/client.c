#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((uint16_t)atoi(argv[2]));
    
    time_t start, end;
    struct timespec latch;
    clock_gettime(CLOCK_MONOTONIC, &latch);
    
    start = latch.tv_nsec;
    
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect");
        exit(EXIT_FAILURE);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &latch);
    end = latch.tv_nsec;
    
    printf("Connection took %lu ns\n", end-start);

    char msg_buf[4096], recv_buf[4096];
    int recv_len = 0;
    

    while (1) {
        printf("Enter a message (type 'exit' to disconnect): ");
        if (fgets(msg_buf, sizeof(msg_buf), stdin) == NULL) {
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &latch);
        start = latch.tv_nsec;
        send(sock_fd, msg_buf, strlen(msg_buf), 0);
        msg_buf[strcspn(msg_buf, "\n")] = '\0'; // remove the terminating newline '\n' from the input string.

        if (strcmp(msg_buf, "exit") == 0) {
            close(sock_fd);
            printf("Disconnected.");
            return 0;
        }

        if ((recv_len = recv(sock_fd, recv_buf, 4096, 0)) == -1) {
            perror("ERROR: Can't Receive");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        recv_buf[recv_len] = '\0';
        printf("%s\n", recv_buf);
        clock_gettime(CLOCK_MONOTONIC, &latch);
        end = latch.tv_nsec;
        printf("Echo took %lu ns\n", end-start);
        memset(msg_buf, 0, sizeof(msg_buf));
        memset(recv_buf, 0, sizeof(recv_buf));
    }
    close(sock_fd);
    return 0;
}

