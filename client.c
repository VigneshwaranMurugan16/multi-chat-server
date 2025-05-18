#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int sock;
char username[32];

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            printf("\nDisconnected from server.\n");
            exit(EXIT_FAILURE);
        }
        buffer[bytes_read] = '\0';

        // Print message and reprint prompt
        printf("\r%s\n%s: ", buffer, username);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. You can start chatting now!\n");

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    char message[BUFFER_SIZE];
    while (1) {
        printf("%s: ", username);
        if (!fgets(message, sizeof(message), stdin)) break;

        message[strcspn(message, "\n")] = 0;  // Remove newline

        if (strlen(message) == 0) continue;

        char send_buffer[BUFFER_SIZE];
        snprintf(send_buffer, sizeof(send_buffer), "\033[1;34m[%s]\033[0m: %s", username, message);

        if (write(sock, send_buffer, strlen(send_buffer)) < 0) {
            perror("Write failed");
            break;
        }
    }

    close(sock);
    return 0;
}

