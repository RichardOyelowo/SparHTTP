#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
#include "http.h"

#define BUFFER_SIZE 4096


int main() {
    printf("Welcome to SparHTTP\n");
    int server_socket = create_server();
    int client_socket = accept_client(server_socket);

    ssize_t bytes_length;
    char buffer[BUFFER_SIZE];
    RequestLine http_request;
 
    bytes_length = receive_request(client_socket, buffer, sizeof(buffer));


    printf("Read %zd bytes\n\n", bytes_length);
    printf("HTTP Request method: %s\n", http_request.method);
    printf("HTTP Request path: %s\n", http_request.path);
    printf("HTTP Request version: %s\n", http_request.version);

    return 0;
}
