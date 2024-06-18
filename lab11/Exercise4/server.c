#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define SERVER_PORT 8080

typedef struct {
    struct sockaddr_in addr;
    char name[20];
} ClientInfo;

ClientInfo clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void handle_client_message(char *buffer, struct sockaddr_in *client_addr, int sockfd);
void send_to_all(char *message, struct sockaddr_in *exclude_addr, int sockfd);
void remove_client(struct sockaddr_in *client_addr);
void send_message_to_client(struct sockaddr_in *client_addr, char *message, int sockfd);
int find_client_by_addr(struct sockaddr_in *client_addr);

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Błąd podczas tworzenia gniazda");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.12");
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Błąd podczas bindowania gniazda");
        exit(EXIT_FAILURE);
    }

    printf("Serwer nasłuchuje na porcie %d...\n", SERVER_PORT);

    while (1) {
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            handle_client_message(buffer, &client_addr, server_socket);
        }
    }

    close(server_socket);
    return 0;
}

void handle_client_message(char *buffer, struct sockaddr_in *client_addr, int sockfd) {
    pthread_mutex_lock(&clients_mutex);

    int client_index = find_client_by_addr(client_addr);
    if (client_index == -1) {
        // Nowy klient
        if (num_clients < MAX_CLIENTS) {
            strcpy(clients[num_clients].name, buffer);
            clients[num_clients].addr = *client_addr;
            client_index = num_clients;
            num_clients++;

            char join_message[BUFFER_SIZE];
            sprintf(join_message, "%s dołączył do czatu\n", clients[client_index].name);
            send_to_all(join_message, client_addr, sockfd);
        } else {
            // Brak miejsca dla nowych klientów
            pthread_mutex_unlock(&clients_mutex);
            return;
        }
    }

    // Obsługa komend
    if (strncmp(buffer, "LIST", 4) == 0) {
        char list_message[BUFFER_SIZE] = "Aktywni klienci:\n";
        for (int i = 0; i < num_clients; i++) {
            strcat(list_message, clients[i].name);
            strcat(list_message, "\n");
        }
        send_message_to_client(client_addr, list_message, sockfd);
    } else if (strncmp(buffer, "2ALL", 4) == 0) {
        char message[BUFFER_SIZE];
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0';
        sprintf(message, "[%s] %s: %s", time_str, clients[client_index].name, buffer + 5);
        send_to_all(message, client_addr, sockfd);
    } else if (strncmp(buffer, "2ONE", 4) == 0) {
        char *recipient_name = strtok(buffer + 5, " ");
        char *message_content = strtok(NULL, "\n");
        char message[BUFFER_SIZE];
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0';
        sprintf(message, "[%s] %s: %s", time_str, clients[client_index].name, message_content);
        for (int i = 0; i < num_clients; i++) {
            if (strcmp(clients[i].name, recipient_name) == 0) {
                send_message_to_client(&clients[i].addr, message, sockfd);
                break;
            }
        }
    } else if (strncmp(buffer, "STOP", 4) == 0) {
        printf("%s opuścił czat\n", clients[client_index].name);
        char left_message[BUFFER_SIZE];
        sprintf(left_message, "%s opuścił czat\n", clients[client_index].name);
        send_to_all(left_message, client_addr, sockfd);
        remove_client(client_addr);
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_all(char *message, struct sockaddr_in *exclude_addr, int sockfd) {
    for (int i = 0; i < num_clients; i++) {
        if (exclude_addr == NULL || memcmp(&clients[i].addr, exclude_addr, sizeof(struct sockaddr_in)) != 0) {
            sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
}

void remove_client(struct sockaddr_in *client_addr) {
    for (int i = 0; i < num_clients; i++) {
        if (memcmp(&clients[i].addr, client_addr, sizeof(struct sockaddr_in)) == 0) {
            for (int j = i; j < num_clients - 1; j++) {
                clients[j] = clients[j + 1];
            }
            num_clients--;
            break;
        }
    }
}

void send_message_to_client(struct sockaddr_in *client_addr, char *message, int sockfd) {
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}

int find_client_by_addr(struct sockaddr_in *client_addr) {
    for (int i = 0; i < num_clients; i++) {
        if (memcmp(&clients[i].addr, client_addr, sizeof(struct sockaddr_in)) == 0) {
            return i;
        }
    }
    return -1;
}
