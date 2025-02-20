#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>

// Initialize the server on the specified port
int init_server(uint16_t port);

// Handle incoming connections
void handle_connections(void);

// Clean up server resources
void cleanup_server(void);

#endif // HTTP_SERVER_H
