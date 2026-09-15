#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/socket.h>

typedef struct {
    char method[16];
    char path[256];
    char version[16];
} RequestLine;


ssize_t receive_request(int client_socket, char *buffer, size_t buffer_size) {
    ssize_t nread = recv(client_socket, buffer, buffer_size - 1, 0);

    if (nread < 0) {
        perror("recv");
    } else {
        // writes the last character to prevent running into error with bytes characters
        buffer[nread] = '\0';
    }

    return nread;
}


RequestLine parse_request(char *buffer, ssize_t buffer_length) {
    char *current = buffer;
    int line = 0;
    char temp[256];
    RequestLine http_request;
   
    while(current[0] != '\r' || current[1] != '\n') {
        char *end = strchr(current, '\r');
        *end = '\0';
        size_t temp_len = end - current;

        // Request line infos handling
        if (line == 0) {
            char *first_space = strchr(current, ' ');
            size_t method_len = first_space - current;
            memcpy(http_request.method, current, method_len);
            http_request.method[method_len] = '\0';
            current = first_space + 1;

            char *second_space = strchr(current, ' ');
            size_t path_len = second_space - current;
            memcpy(http_request.path, current, path_len);
            http_request.path[path_len] = '\0';
            current = second_space + 1;

            size_t version_len = end - current;
            memcpy(http_request.version, current, version_len);
            http_request.version[version_len] = '\0';
        }
        memcpy(temp, current, temp_len);

        // updates line and current to next buffer line
        line++;
        current = end + 2;
    }

    return http_request;
}
