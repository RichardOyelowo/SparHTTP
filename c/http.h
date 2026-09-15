#ifndef HTTP_H
#define HTTP_H

#include <sys/socket.h>

typedef struct {
    char method[16];
    char path[256];
    char version[30];
} RequestLine;


ssize_t receive_request(int client_socket, char *buffer, size_t buffer_size);
RequestLine parse_request(char *buffer, ssize_t buffer_length);


#endif
