#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 41783
#define ADDRESS "127.0.0.1"
#define BACKLOG 10


int create_server() {
    printf("Creating server socket...\n\n");
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    socklen_t address_length = sizeof(server_address);

    server_address.sin_family = AF_INET;
    server_address.sin_port =  htons(PORT);
    inet_pton(AF_INET, ADDRESS, &server_address.sin_addr);

    int reuse = 1;

    setsockopt(
        server_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse,
        sizeof(reuse)
    );

    // validates binding action
    if (bind(server_socket, (struct sockaddr *)&server_address, address_length) < 0)
        printf("Binding Error Occurred\n");

    // validates listening connection
    if (listen(server_socket, BACKLOG) < 0)
        printf("Listen Error Occurred\n");

    return server_socket;
}

int accept_client(int server_socket) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    return accept(server_socket, 
                  (struct sockaddr *)&client_address, 
                  &address_length
    );
}
