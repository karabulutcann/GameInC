#ifndef NETGAME_H
#define NETGAME_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum buffer sizes
#define MAX_BUFFER_SIZE 4096
#define MAX_URL_LENGTH 256
#define MAX_HEADER_SIZE 1024
#define MAX_JSON_SIZE 8192

// Error codes
#define NETGAME_SUCCESS 0
#define NETGAME_ERROR_SOCKET -1
#define NETGAME_ERROR_CONNECT -2
#define NETGAME_ERROR_SEND -3
#define NETGAME_ERROR_RECEIVE -4
#define NETGAME_ERROR_MEMORY -5
#define NETGAME_ERROR_PARSE -6

// HTTP methods
typedef enum {
    HTTP_GET,
    HTTP_POST
} http_method_t;

// JSON value types
typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} json_type_t;

// JSON value structure
typedef struct json_value {
    json_type_t type;
    union {
        int boolean;
        double number;
        char* string;
        struct {
            struct json_value* values;
            int count;
        } array;
        struct {
            char** keys;
            struct json_value* values;
            int count;
        } object;
    } data;
} json_value_t;

// HTTP response structure
typedef struct {
    int status_code;
    char* body;
    size_t body_length;
} http_response_t;

// Core networking functions
int netgame_init(void);
void netgame_cleanup(void);
int netgame_create_socket(void);
int netgame_connect(int socket, const char* host, int port);
int netgame_send(int socket, const char* data, size_t length);
int netgame_receive(int socket, char* buffer, size_t buffer_size);

// HTTP functions
http_response_t* http_request(const char* url, http_method_t method, const char* body);
void http_response_free(http_response_t* response);

// JSON functions
json_value_t* json_parse(const char* json_str);
char* json_stringify(const json_value_t* value);
void json_free(json_value_t* value);

// Utility functions
char* url_encode(const char* str);
void log_error(const char* message);

#endif // NETGAME_H
