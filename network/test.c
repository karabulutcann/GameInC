#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "http_server.h"

// Global flag for graceful shutdown
volatile sig_atomic_t keep_running = 1;

void handle_signal(int signum) {
    printf("\nReceived signal %d. Shutting down...\n", signum);
    keep_running = 0;
}

int main() {
    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Initialize server
    if (init_server(8000) != 0) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }

    printf("Server started on port 8000...\n");

    // Main server loop
    while (keep_running) {
        handle_connections();
    }

    // Cleanup
    cleanup_server();
    printf("Server shutdown complete\n");

    return 0;
}
