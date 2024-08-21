#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 7777 
#define BUF_SIZE 4096

void trim_newline(char *string) {
    int len = strlen(string);

    while (len > 0 && (string[len-1] == '\n' || string[len-1] == '\r')) {
        string[len-1] = '\0';
        len--;
    }
}
 
int main(int argc, char *argv[]) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)& server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    listen(sock_fd, 3);
    printf("Listening on PORT: %d\n", PORT);

    int conn_fd;
    struct sockaddr_in client_addr;
    int client_addr_len;

    char buf[BUF_SIZE];
    int n;

    for (;;) {
        conn_fd = accept(sock_fd, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr_len);
        if (conn_fd == -1) {
            fprintf(stderr, "Error occured connecting to client.");
            continue;
        }
        printf("Connected with client(%s)\n", inet_ntoa(client_addr.sin_addr)); // extracting the ipv4 from the client addr struct.

        while ( (n = recv(conn_fd, buf, BUF_SIZE, 0)) > 0) {
            buf[n] = '\0';
            trim_newline(buf);
            if (strcmp(buf, "exit") == 0) {
                printf("Disconnected from client(%s)\n", inet_ntoa(client_addr.sin_addr));
                break;
            }
            send(conn_fd, buf, strlen(buf), 0);
            //printf("CLIENT: %s\n", buf);
            memset(buf, 0, BUF_SIZE);
        }
        close(conn_fd);
        printf("\n");
    }

    close(sock_fd);
    return 0;
}

