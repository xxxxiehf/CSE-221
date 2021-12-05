#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <linux/socket.h>
#include <iostream>
#include <string>


#define SERVER_PORT 8889
#define BUFFER_SIZE 100

int main() {
    int fd , client_sock, read_size;
    struct sockaddr_in server, client;
    
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Fail to create socket~" << std::endl;
        close(fd);
    }
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);
     
    if(bind(fd,(struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Fail to bind to socket~" << std::endl;
        close(fd);
        exit(EXIT_FAILURE);
    }
     
    listen(fd , 3);
     
    std::cout << "Waiting...~" << std::endl;
    int l = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    client_sock = accept(fd, (struct sockaddr *)&client, (socklen_t*)&l);

    while (1) {
    	if (client_sock < 0) {
            std::cerr << "Fail to accept to socket~" << std::endl;
            close(fd);
            exit(EXIT_FAILURE);
    	}
        size_t recv_size = recv(client_sock, &buffer, sizeof(buffer), 0);
    }
}
