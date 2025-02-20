#include "netgame.h"
#include <ctype.h>

int netgame_init(void) {
    #ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data);
    #else
    return NETGAME_SUCCESS;
    #endif
}

void netgame_cleanup(void) {
    #ifdef _WIN32
    WSACleanup();
    #endif
}

int netgame_create_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        log_error("Failed to create socket");
        return NETGAME_ERROR_SOCKET;
    }
    return sock;
}

int netgame_connect(int socket, const char* host, int port) {
    struct sockaddr_in server_addr;
    struct hostent* server;

    server = gethostbyname(host);
    if (server == NULL) {
        log_error("Could not resolve hostname");
        return NETGAME_ERROR_CONNECT;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log_error("Connection failed");
        return NETGAME_ERROR_CONNECT;
    }

    return NETGAME_SUCCESS;
}

int netgame_send(int socket, const char* data, size_t length) {
    size_t total_sent = 0;
    
    while (total_sent < length) {
        int sent = send(socket, data + total_sent, length - total_sent, 0);
        if (sent < 0) {
            log_error("Send failed");
            return NETGAME_ERROR_SEND;
        }
        total_sent += sent;
    }
    
    return total_sent;
}

int netgame_receive(int socket, char* buffer, size_t buffer_size) {
    int received = recv(socket, buffer, buffer_size - 1, 0);
    if (received < 0) {
        log_error("Receive failed");
        return NETGAME_ERROR_RECEIVE;
    }
    buffer[received] = '\0';
    return received;
}

void log_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    #ifdef _WIN32
    fprintf(stderr, "WSA Error Code: %d\n", WSAGetLastError());
    #endif
}

char* url_encode(const char* str) {
    if (!str) return NULL;
    
    char* encoded = (char*)malloc(strlen(str) * 3 + 1);
    if (!encoded) return NULL;

    char* p = encoded;
    while (*str) {
        if (isalnum(*str) || *str == '-' || *str == '_' || *str == '.' || *str == '~') {
            *p++ = *str;
        } else {
            sprintf(p, "%%%02X", (unsigned char)*str);
            p += 3;
        }
        str++;
    }
    *p = '\0';
    
    return encoded;
}