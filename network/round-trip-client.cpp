#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <errno.h>
#include <netinet/in.h>     // sockaddr
#include <arpa/inet.h>      // htons()
#include <sys/socket.h>
#include <iostream>
#include <string>
#include "time.h"

#define SERVER_PORT 8888
#define TCP_TEST_NUM 20
#define MAX_SIZE 1000
#define START_SIZE 100
#define ITERATION 100

void socket_warm_up(int fd) {
    uint64_t send_size = START_SIZE, receive_size = START_SIZE;
    char send_buffer[send_size], receive_buffer[receive_size];
    for (uint64_t i = 0; i < send_size; ++i) {
        send_buffer[i] = 'x';
    }
    uint64_t total_bytes = 0;
    send(fd, &send_buffer, send_size, 0);

    while (total_bytes < receive_size) {
        total_bytes += recv(fd, &receive_buffer, receive_size, 0);
    }
}

void print_errno(void) {
    fprintf(stderr, "Value of errno: %d\n", errno);
    perror("Error printed by perror");
    fprintf(stderr, "Output by strerror: %s\n", strerror(errno));
}

void socket_tcp_ping(char ip[]) {
    uint64_t start, end, diff;
    struct sockaddr_in echo_server_socket;
    int fd;
    uint64_t receive_size, total_bytes = 0;
    memset(&echo_server_socket, 0, sizeof(echo_server_socket));
    echo_server_socket.sin_family = AF_INET;
    echo_server_socket.sin_addr.s_addr = inet_addr(ip);
    echo_server_socket.sin_port = htons(SERVER_PORT);

    if ((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "Fail to create socket" << std::endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (connect(fd, (struct sockaddr*)&echo_server_socket, sizeof(echo_server_socket)) < 0) {
        std::cerr << "Fail to connect to socket" << std::endl;
        print_errno();
        close(fd);
        exit(EXIT_FAILURE);
    }
    socket_warm_up(fd);

    for (uint64_t i = START_SIZE; i <= MAX_SIZE; i += ITERATION) {
        char send_buffer[i];
        for (uint64_t k = 0; k < i; ++k) {
            send_buffer[k] = 'x';
        }
        receive_size = i;
        diff = 0;
        char receive_buffer[i];
        for (uint64_t k = 0; k < TCP_TEST_NUM; ++k) {
            total_bytes = 0;
            start = rdtsc_start();
            send(fd, &send_buffer, i, 0);
            while (total_bytes < receive_size) {
                total_bytes += recv(fd, &receive_buffer, receive_size, 0);
            }
            end = rdtsc_end();
            diff += end - start;
        }
        std::cout << ip << " send_size: " << i << " in " << (double) diff / (double)TCP_TEST_NUM << " cycles." << std::endl;
    }
    close(fd);
}

int main() {
    char local_ip[] = "127.0.0.1";
    socket_tcp_ping(local_ip);
}