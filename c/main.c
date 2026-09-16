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

    // buffer parsing & handling
    RequestLine http_request;
    Header header[MAX_HEADER];
    char *client_msg;
    int header_count;

    bytes_length = receive_request(client_socket, buffer, sizeof(buffer));
    header_count = parse_request(buffer, &http_request, header, &client_msg);

    // print parsed infos
    printf("Reading %zd bytes...\n\n", bytes_length);

    printf("HTTP Request method: %s\n", http_request.method);
    printf("HTTP Request path: %s\n", http_request.path);
    printf("HTTP Request version: %s\n", http_request.version);

    // printing header infos
    for (int i = 0; i < header_count; i++) {
        printf("%s: %s\n", header[i].name, header[i].value);
    }

    // message body 
    printf("Client message: %s\n", client_msg);

    return 0;
}
