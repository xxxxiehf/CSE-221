#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    
#include <errno.h>
#include <netinet/in.h>    
#include <arpa/inet.h>      
#include <sys/socket.h>
#include <iostream>
#include <string>
#include "time.h"

#define SERVER_PORT 8889
#define TCP_TEST_NUM 20
// 200 MB
#define SEND_DATA_SIZE 25000000


void peak_bandwidth_client(char ip[]) {
    uint64_t start, end, diff;
    struct sockaddr_in server;
    int fd;
    uint64_t receive_size, total_bytes = 0;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(SERVER_PORT);

    if ((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "Fail to create socket" << std::endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (connect(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Fail to connect to socket" << std::endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
    char* send_buffer = new char[SEND_DATA_SIZE];
    for (uint64_t k = 0; k < SEND_DATA_SIZE; ++k) {
        send_buffer[k] = 'x';
    }
    uint64_t min_cycles = UINT64_MAX;
    for (uint64_t k = 0; k < TCP_TEST_NUM; ++k) {
        receive_size = SEND_DATA_SIZE;
        diff = 0;
        total_bytes = 0;
        start = rdtsc_start();
        send(fd, &send_buffer, SEND_DATA_SIZE, 0);
        end = rdtsc_end();
        diff = end - start;
        if (diff < min_cycles) {
            min_cycles = diff;
        }
        
    }
    delete []send_buffer;
    std::cout << ip << " send_size: " << SEND_DATA_SIZE << " in " << min_cycles << " min cycles." << std::endl;
    close(fd);
}

int main() {
    char local_ip[] = "0.0.0.0";
    peak_bandwidth_client(local_ip);
}