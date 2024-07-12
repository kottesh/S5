#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 7774 
#define BUFFER_SIZE 4096

int main (int argc, char **argv) {
    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("After Socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    //memset((char *)&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    struct sockaddr_in client_addr;

    if (bind(sock_fd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0) {
        perror("After binding transport address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int req_len = 0;

    while (1) {
        socklen_t len = sizeof(client_addr);
        req_len = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, (socklen_t *)&len);

        if (req_len > 0) {
            buffer[req_len] = 0;
            printf("Message: %s\n", buffer);

            time_t time_now; 
            time(&time_now);
            char* curr_time = ctime(&time_now);
            //struct tm *local_time = localtime(&time_now);
            //strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", local_time);
            sendto(sock_fd, curr_time, strlen(curr_time), 0, (const struct sockaddr*)&client_addr, sizeof(client_addr));
        }
    }

    return 0;
}

