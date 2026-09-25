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
    char servers_response[BUFFER_SIZE];
    Response response;

    ssize_t bytes_length;
    char buffer[BUFFER_SIZE];

    // buffer parsing & handling
    RequestLine http_request;
    Header header[MAX_HEADER];
    char *client_msg;
    int header_count;

    bytes_length = receive_request(client_socket, buffer, sizeof(buffer));
    header_count = parse_request(buffer, &http_request, header, &client_msg);

    // request_line and method
    printf("Reading %zd bytes...\n\n", bytes_length);
    printf(
        "Sender Request:\nMethod: %s\nPath: %s\nVersion: %s\n", 
        http_request.method, http_request.path, http_request.version
    );

    // headers info
    for (int i = 0; i < header_count; i++) {
        printf("%s: %s\n", header[i].name, header[i].value);
    }

    // response for sender
    build_response(&response);
    int response_length = serialize_response(
        &response, 
        servers_response, 
        sizeof(servers_response)
    );

    ssize_t bytes_sent = send(
        client_socket,
        servers_response,
        response_length,
        0
    );

    printf("\nSent %zd bytes\n", bytes_sent);

    return 0;
}
