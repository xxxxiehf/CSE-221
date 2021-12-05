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
using namespace std;

#define SERVER_PORT 8890
#define TCP_TEST_NUM 20
#define SEND_DATA_SIZE 100


void peak_bandwidth_client(char ip[]) {
    uint64_t start, end, diff;
    struct sockaddr_in server;
    int fd;
    memset(&server, 0, sizeof(server));
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(SERVER_PORT);

    char* send_buffer = new char[SEND_DATA_SIZE];
    for (uint64_t k = 0; k < SEND_DATA_SIZE; ++k) {
        send_buffer[k] = 'x';
    }
    for (uint64_t k = 0; k < TCP_TEST_NUM; ++k) {
        if ((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            cerr << "Fail to create socket~" << endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
        start = rdtsc_start();
        int conn_res = connect(fd, (struct sockaddr*)&server, sizeof(server));
        end = rdtsc_end();
        diff = end - start;
        cout << ip << " setup time: " << diff << endl;
        if (conn_res < 0) {
            cerr << "Fail to connect to socket~" << endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
        diff = 0;
        send(fd, &send_buffer, SEND_DATA_SIZE, 0);
    }
    delete []send_buffer;
    close(fd);
}

int main() {
    char local_ip[] = "127.0.0.1";
    peak_bandwidth_client(local_ip);
}
