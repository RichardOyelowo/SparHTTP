# SparHTTP

A from-scratch HTTP server. No framework, no parsing library, no malloc. First one in a series doing the same server across a few languages.

## What it does

Opens a raw TCP socket, accepts one client, reads the request off the wire, parses the request line and headers with `strchr` and `memcpy`, and sends back a fixed `200 OK`. Blocking, single connection, exits after.

## Files

- `server.c` / `server.h` - socket, bind, listen, accept
- `http.c` / `http.h` - receive, parse, build, serialize
- `main.c` - wires it together, prints what it parsed on the way through

## Parsing

Walks the buffer with a `current` pointer, cuts each line at `\r`, null-terminates in place:

```c
while(current[0] != '\r' || current[1] != '\n') {
    char *end = strchr(current, '\r');
    if (end == NULL)
        break;
    *end = '\0';

    if (line_count == 0) {
        char *first_space = strchr(current, ' ');
        size_t method_len = first_space - current;
        memcpy(request_line->method, current, method_len);
        ...
```

Request line on the first pass, everything after that treated as `Name: value` headers until the blank line.

## Buffers

```c
#define MAX_HEADER 10
#define MAX_BODY 500
```

4096 byte read buffer in `main.c`. No malloc anywhere: everything's stack allocated fixed size.

## Config

Hardcoded in `server.c`:

```c
#define PORT 41783
#define ADDRESS "127.0.0.1"
#define BACKLOG 10
```

## Running it

No Makefile yet, build it directly:

```bash
gcc *.c -o main
./main
```

```bash
curl http://127.0.0.1:41783/
```

## Not there yet

- handling more than one client per run
- routing by method/path, right now everything gets `200 OK`
- validating malformed input
- a Makefile

---

Built for the love of development by Richard Oyelowo
