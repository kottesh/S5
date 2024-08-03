#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>

#define DNS_PORT 53
#define DNS_SERVER "8.8.8.8"
#define MAX_BUFFER_SIZE 1024

typedef struct
{
    uint16_t id;
    uint16_t flags;
    uint16_t q_count;
    uint16_t ans_count;
    uint16_t ns_count;
    uint16_t add_count;
} DNS_header;

typedef struct
{
    char *name;
    uint16_t type;
    uint16_t class;
} DNS_question;

typedef struct
{
    uint16_t compression;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t length;
    struct in_addr addr;
} __attribute__((packed)) DNS_record_a_t;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *hostname = argv[1];

    int sock_fd;
    struct sockaddr_in address;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        perror("socket creation");
        exit(EXIT_FAILURE);
    }

    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(DNS_PORT);
    address.sin_addr.s_addr = inet_addr(DNS_SERVER);

    DNS_header header;
    memset((char *)&header, 0, sizeof(header));
    header.id = htons(0x7774);
    header.flags = htons(0x0100);
    header.q_count = htons(1);

    DNS_question question;
    question.type = htons(1);
    question.class = htons(1);
    question.name = calloc(strlen(hostname) + 2, sizeof(char));

    memcpy(question.name + 1, hostname, strlen(hostname));

    char *prev = (char *)question.name;
    int count = 0;

    for (size_t i = 0; i < strlen(hostname); i++)
    {
        if (hostname[i] == '.')
        {
            *prev = count;
            count = 0;
            prev = question.name + i + 1;
        }
        else
        {
            count++;
        }
    }

    *prev = count;
    size_t packet_size = sizeof(header) + strlen(hostname) + 2 + sizeof(question.type) + sizeof(question.class);

    uint8_t *packet = calloc(packet_size, sizeof(uint8_t));
    uint8_t *p = (uint8_t *)packet;

    memcpy(p, &header, sizeof(header));
    p += sizeof(header);

    memcpy(p, question.name, strlen(hostname) + 2);
    p += strlen(hostname) + 2;

    memcpy(p, &question.type, sizeof(question.type));
    p += sizeof(question.type);
    memcpy(p, &question.class, sizeof(question.class));

    if (sendto(sock_fd, packet, packet_size, 0, (struct sockaddr *)&address, (socklen_t)sizeof(address)) < 0)
    {
        perror("After Send");
    }

    socklen_t length = 0;
    uint8_t response[1024];
    memset(&response, 0, 1024);

    int bytes = recvfrom(sock_fd, response, 1024, 0, (struct sockaddr *)&address, &length);

    DNS_header *response_header = (DNS_header *)response;
    assert((ntohs(response_header->flags) & 0xf) == 0);

    uint8_t *start_of_name = (uint8_t *)(response + sizeof(DNS_header));
    uint8_t total = 0;

    uint8_t *field_length = start_of_name;

    while (*field_length != 0)
    {
        total += *field_length + 1;
        *field_length = '.';
        field_length = start_of_name + total;
    }

    DNS_record_a_t *records = (DNS_record_a_t *)(field_length + 5);
    for (int i = 0; i < ntohs(response_header->ans_count); i++)
    {
        // printf("TYPE: %" PRId16 "\n", ntohs(records[i].type));
        // printf("CLASS: %" PRId16 "\n", ntohs(records[i].class));
        // printf("TTL: %" PRIx32 "\n", ntohl(records[i].ttl));
        // printf("IPv4: %08" PRIx32 "\n", ntohl(records[i].addr));
        printf("IPv4: %s\n", inet_ntoa(records[i].addr));
    }

    return 0;
}
