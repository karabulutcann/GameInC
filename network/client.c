#include "netgame.h"

int main() {
    // Initialize networking
    if (netgame_init() != NETGAME_SUCCESS) {
        printf("Failed to initialize networking\n");
        return 1;
    }

    // Example HTTP GET request
    printf("Making GET request to httpbin.org...\n");
    http_response_t* response = http_request("http://httpbin.org/get", HTTP_GET, NULL);
    if (response) {
        printf("Status code: %d\n", response->status_code);
        printf("Response body: %s\n", response->body);

        // Parse JSON response
        json_value_t* json = json_parse(response->body);
        if (json) {
            // Example: Access JSON data
            if (json->type == JSON_OBJECT) {
                for (int i = 0; i < json->data.object.count; i++) {
                    printf("Key: %s\n", json->data.object.keys[i]);
                }
            }
            json_free(json);
        }

        http_response_free(response);
    } else {
        printf("GET request failed\n");
    }

    // Example HTTP POST request
    printf("\nMaking POST request to httpbin.org...\n");
    const char* post_data = "{\"name\":\"player1\",\"score\":100}";
    response = http_request("http://httpbin.org/post", HTTP_POST, post_data);
    if (response) {
        printf("POST Status code: %d\n", response->status_code);
        printf("POST Response body: %s\n", response->body);
        http_response_free(response);
    } else {
        printf("POST request failed\n");
    }

    // Cleanup
    netgame_cleanup();
    return 0;
}