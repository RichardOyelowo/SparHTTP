#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <sys/socket.h>

#define MAX_HEADER 10
#define MAX_BODY 500

typedef struct {
    char name[64];
    char value[500];
} Header;

typedef struct {
    char method[16];
    char path[256];
    char version[30];
} RequestLine;

typedef struct {
    char status[256];
    Header header[MAX_HEADER];
    char body[MAX_BODY];
} Response;


ssize_t receive_request(int client_socket, char *buffer, size_t buffer_size);
int parse_request(char *buffer, RequestLine *request, Header *header, char **body);
void build_response(Response *response);

#endif
