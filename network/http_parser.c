#include "http_parser.h"
#include <string.h>
#include <stdio.h>

void parse_http_request(const char* request_str, http_request_t* request) {
    // Initialize request structure
    memset(request->method, 0, MAX_METHOD_LENGTH);
    memset(request->path, 0, MAX_PATH_LENGTH);

    // Extract method and path
    sscanf(request_str, "%7s %2047s", request->method, request->path);

    // Simple validation
    if (strlen(request->method) == 0 || strlen(request->path) == 0) {
        strcpy(request->method, "GET");
        strcpy(request->path, "/");
    }
}
