#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

static _Atomic unsigned int client_count = 0;
static int clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == 0) {
            clients[i] = client_socket;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == client_socket) {
            clients[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_to_all(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);

    // Get current time for timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[9]; // HH:MM:SS
    strftime(time_str, sizeof(time_str), "%H:%M:%S", t);

    char message_with_time[BUFFER_SIZE + 50];
    snprintf(message_with_time, sizeof(message_with_time), "[%s] %s", time_str, message);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != 0 && clients[i] != sender_socket) {
            if (write(clients[i], message_with_time, strlen(message_with_time)) < 0) {
                perror("Write to client failed");
                // We could remove client here if needed
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);

    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        send_message_to_all(buffer, client_socket);
    }

    if (bytes_read == 0) {
        printf("Client disconnected.\n");
    } else if (bytes_read == -1) {
        perror("Read failed");
    }

    close(client_socket);
    remove_client(client_socket);
    client_count--;
    pthread_exit(NULL);
}

int main() {
    int server_socket, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t tid;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        if (client_count >= MAX_CLIENTS) {
            printf("Max clients reached. Connection rejected.\n");
            close(client_socket);
            continue;
        }

        printf("Client connected.\n");

        add_client(client_socket);
        client_count++;

        new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        if (pthread_create(&tid, NULL, handle_client, (void *)new_sock) != 0) {
            perror("Thread creation failed");
            free(new_sock);
            continue;
        }

        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}
