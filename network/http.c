#include "netgame.h"

static const char* HTTP_GET_TEMPLATE = 
    "GET %s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: NetGame/1.0\r\n"
    "Accept: */*\r\n"
    "Connection: close\r\n\r\n";

static const char* HTTP_POST_TEMPLATE = 
    "POST %s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: NetGame/1.0\r\n"
    "Accept: */*\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: %zu\r\n"
    "Connection: close\r\n"
    "\r\n"
    "%s";

static char* extract_hostname(const char* url, char* path) {
    char* hostname = (char*)malloc(MAX_URL_LENGTH);
    if (!hostname) return NULL;

    const char* protocol = strstr(url, "://");
    const char* host_start = protocol ? protocol + 3 : url;

    const char* path_start = strchr(host_start, '/');
    if (path_start) {
        size_t host_length = path_start - host_start;
        strncpy(hostname, host_start, host_length);
        hostname[host_length] = '\0';
        strcpy(path, path_start);
    } else {
        strcpy(hostname, host_start);
        strcpy(path, "/");
    }

    return hostname;
}

http_response_t* http_request(const char* url, http_method_t method, const char* body) {
    char path[MAX_URL_LENGTH];
    char* hostname = extract_hostname(url, path);
    if (!hostname) return NULL;

    int sock = netgame_create_socket();
    if (sock < 0) {
        free(hostname);
        return NULL;
    }

    if (netgame_connect(sock, hostname, 80) != NETGAME_SUCCESS) {
        free(hostname);
        #ifdef _WIN32
        closesocket(sock);
        #else
        close(sock);
        #endif
        return NULL;
    }

    char request[MAX_BUFFER_SIZE];
    if (method == HTTP_GET) {
        snprintf(request, sizeof(request), HTTP_GET_TEMPLATE, path, hostname);
    } else if (method == HTTP_POST && body) {
        size_t body_len = strlen(body);
        snprintf(request, sizeof(request), HTTP_POST_TEMPLATE, 
                path, hostname, body_len, body);
        fprintf(stderr, "DEBUG: Sending POST request:\n%s\n", request);
    } else {
        free(hostname);
        #ifdef _WIN32
        closesocket(sock);
        #else
        close(sock);
        #endif
        return NULL;
    }

    if (netgame_send(sock, request, strlen(request)) < 0) {
        free(hostname);
        #ifdef _WIN32
        closesocket(sock);
        #else
        close(sock);
        #endif
        return NULL;
    }

    http_response_t* response = (http_response_t*)malloc(sizeof(http_response_t));
    if (!response) {
        free(hostname);
        #ifdef _WIN32
        closesocket(sock);
        #else
        close(sock);
        #endif
        return NULL;
    }

    char buffer[MAX_BUFFER_SIZE * 2];  // Increased buffer size
    char header_buffer[MAX_HEADER_SIZE];
    size_t header_size = 0;
    size_t total_received = 0;
    int header_complete = 0;

    response->body = (char*)malloc(MAX_BUFFER_SIZE);
    response->status_code = 0;
    response->body_length = 0;

    // First, accumulate the complete header
    while (!header_complete && header_size < MAX_HEADER_SIZE) {
        int received = netgame_receive(sock, buffer, sizeof(buffer));
        if (received <= 0) break;

        fprintf(stderr, "DEBUG: Received %d bytes\n", received);

        if (header_size == 0) {
            // Look for status code in first chunk
            char* status_line = strstr(buffer, "HTTP/1.1 ");
            if (status_line) {
                response->status_code = atoi(status_line + 9);
                fprintf(stderr, "DEBUG: Found status code: %d\n", response->status_code);
            }
        }

        // Look for header/body separator
        char* body_start = strstr(buffer, "\r\n\r\n");
        if (body_start) {
            header_complete = 1;
            body_start += 4;  // Skip the separator
            size_t body_length = received - (body_start - buffer);

            // Copy body portion to response
            if (body_length > 0) {
                memcpy(response->body, body_start, body_length);
                total_received = body_length;
            }

            fprintf(stderr, "DEBUG: Header complete, initial body length: %zu\n", body_length);
        } else {
            // Still accumulating header
            size_t to_copy = received;
            if (header_size + to_copy > MAX_HEADER_SIZE) {
                to_copy = MAX_HEADER_SIZE - header_size;
            }
            memcpy(header_buffer + header_size, buffer, to_copy);
            header_size += to_copy;
        }
    }

    // Continue receiving body
    while (total_received < MAX_BUFFER_SIZE - 1) {
        int received = netgame_receive(sock, buffer, sizeof(buffer));
        if (received <= 0) break;

        size_t to_copy = received;
        if (total_received + to_copy > MAX_BUFFER_SIZE - 1) {
            to_copy = MAX_BUFFER_SIZE - 1 - total_received;
        }
        memcpy(response->body + total_received, buffer, to_copy);
        total_received += to_copy;
    }

    response->body[total_received] = '\0';
    response->body_length = total_received;

    fprintf(stderr, "DEBUG: Final response length: %zu\n", total_received);

    free(hostname);
    #ifdef _WIN32
    closesocket(sock);
    #else
    close(sock);
    #endif

    return response;
}

void http_response_free(http_response_t* response) {
    if (response) {
        free(response->body);
        free(response);
    }
}