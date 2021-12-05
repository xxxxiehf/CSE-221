#include <arpa/inet.h>
#include <iostream>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

#define SERVER_PORT 8889
#define BUFFER_SIZE 100

int main() {
    int fd, client_sock;
    struct sockaddr_in server, client;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Fail to create socket~" << endl;
        close(fd);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Fail to bind to socket~" << endl;
        close(fd);
        exit(EXIT_FAILURE);
    }

    listen(fd, 3);

    cout << "Waiting...~" << endl;
    int l = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    client_sock = accept(fd, (struct sockaddr *)&client, (socklen_t *)&l);

    while (1) {
        if (client_sock < 0) {
            cerr << "Fail to accept to socket~" << endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
        recv(client_sock, &buffer, sizeof(buffer), 0);
    }
}
