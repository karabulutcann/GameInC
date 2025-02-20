#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#define MAX_PATH_LENGTH 2048
#define MAX_METHOD_LENGTH 8

typedef struct {
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
} http_request_t;

// Parse HTTP request and populate request structure
void parse_http_request(const char* request_str, http_request_t* request);

#endif // HTTP_PARSER_H
