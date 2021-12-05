#include "time.h"
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define SERVER_PORT 8889
#define TCP_TEST_NUM 20

void peak_bandwidth_client(char ip[]) {
    uint64_t start, end, diff;
    struct sockaddr_in server;
    int fd;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(SERVER_PORT);

    if ((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "Fail to create socket~" << endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Fail to connect to socket~" << endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
    for (off_t data_size = 2 * 1024 * 1024; data_size <= 256 * 1024 * 1024;
         data_size *= 2) {
        char *send_buffer = new char[data_size];
        for (auto k = 0; k < data_size; ++k) {
            send_buffer[k] = 'x';
        }
        uint64_t min_cycles = UINT64_MAX;
        for (auto k = 0; k < TCP_TEST_NUM; ++k) {
            diff = 0;
            start = rdtsc_start();
            send(fd, &send_buffer, data_size, 0);
            end = rdtsc_end();
            diff = end - start;
            if (diff < min_cycles) {
                min_cycles = diff;
            }
        }
        delete[] send_buffer;
        cout << ip << " send_size: " << data_size << " in " << min_cycles
             << " min cycles." << endl;
    }
    close(fd);
}

int main() {
    char local_ip[] = "127.0.0.1";
    peak_bandwidth_client(local_ip);
}
