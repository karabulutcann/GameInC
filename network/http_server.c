#include "http_server.h"
#include "http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 8192
#define BACKLOG 10

static int server_fd = -1;

int init_server(uint16_t port) {
    struct sockaddr_in server_addr;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket");
        return -1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Failed to set socket options");
        close(server_fd);
        return -1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        close(server_fd);
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Failed to listen on socket");
        close(server_fd);
        return -1;
    }

    return 0;
}

void handle_connections(void) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Accept connection
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Failed to accept connection");
        return;
    }

    // Read request
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("Failed to read from socket");
        close(client_fd);
        return;
    }
    buffer[bytes_read] = '\0';

    // Parse and handle request
    http_request_t request;
    parse_http_request(buffer, &request);

    // Generate response
    char response[BUFFER_SIZE];
    if (strcmp(request.path, "/") == 0) {
        // Serve index.html
        int fd = open("public/index.html", O_RDONLY);
        if (fd < 0) {
            const char* not_found = "HTTP/1.1 404 Not Found\r\n"
                                  "Content-Type: text/plain\r\n"
                                  "Content-Length: 13\r\n\r\n"
                                  "404 Not Found\n";
            write(client_fd, not_found, strlen(not_found));
        } else {
            char content[BUFFER_SIZE];
            ssize_t content_length = read(fd, content, BUFFER_SIZE);
            close(fd);

            snprintf(response, BUFFER_SIZE,
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: %ld\r\n\r\n"
                    "%.*s",
                    content_length, (int)content_length, content);
            write(client_fd, response, strlen(response));
        }
    } else {
        // 404 for unknown paths
        const char* not_found = "HTTP/1.1 404 Not Found\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: 13\r\n\r\n"
                              "404 Not Found\n";
        write(client_fd, not_found, strlen(not_found));
    }

    close(client_fd);
}

void cleanup_server(void) {
    if (server_fd != -1) {
        close(server_fd);
        server_fd = -1;
    }
}
