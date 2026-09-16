#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/socket.h>
#include "http.h"


ssize_t receive_request(int client_socket, char *buffer, size_t buffer_size) {
    ssize_t nread = recv(client_socket, buffer, buffer_size - 1, 0);

    if (nread < 0) {
        perror("recv");
    } else {
        // last character to prevent running into error with bytes char
        buffer[nread] = '\0';
    }

    return nread;
}


int parse_request(char *buffer, RequestLine *request_line, Header *header, char **body) {
    char *current = buffer;
    int line_count = 0;
    int header_count = 0;
   
    while(current[0] != '\r' || current[1] != '\n') {
        char *end = strchr(current, '\r');
        if (end == NULL)
            break;

        *end = '\0';

        // Request line infos handling
        if (line_count == 0) {
            char *first_space = strchr(current, ' ');
            size_t method_len = first_space - current;
            memcpy(request_line->method, current, method_len);
            request_line->method[method_len] = '\0';
            current = first_space + 1;

            char *second_space = strchr(current, ' ');
            size_t path_len = second_space - current;
            memcpy(request_line->path, current, path_len);
            request_line->path[path_len] = '\0';
            current = second_space + 1;

            size_t version_len = end - current;
            memcpy(request_line->version, current, version_len);
            request_line->version[version_len] = '\0';

            // updates current pointer to next line start
            current = end + 2;

            line_count++;
        }else {
            //parsing header, using line since it's one header -> value per line
            char *colon = strchr(current, ':');
            if (colon == NULL)
                break;

            size_t header_len = colon - current;
            memcpy(header[header_count].name, current, header_len);
            header[header_count].name[header_len] = '\0';
            current = colon + 2;

            size_t value_len = end - current;
            memcpy(header[header_count].value, current, value_len);
            header[header_count].value[value_len] = '\0';
            current = end + 2;

            // updates line and current to next buffer line
            header_count++;
        }
    }
    current += 2;
    *body = current;

    return header_count;
}
