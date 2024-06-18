#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int client_socket;
struct sockaddr_in server_addr;
char client_name[20];

void *receive_messages(void *arg);
void stop_client(int signum);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Użycie: %s nazwa_klienta adres_serwera port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(client_name, argv[1], sizeof(client_name) - 1);
    char *server_ip = argv[2];
    int server_port = atoi(argv[3]);

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Błąd podczas tworzenia gniazda");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Wysyłanie nazwy klienta do serwera
    sendto(client_socket, client_name, strlen(client_name), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    // Obsługa Ctrl+C
    signal(SIGINT, stop_client);

    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);

        if (strncmp(buffer, "LIST", 4) == 0) {
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
            break;
        } else {
            printf("Nieznana komenda. Dostępne komendy: LIST, 2ALL, 2ONE, STOP.\n");
        }
    }

    close(client_socket);
    return 0;
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    while (1) {
        int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("%s", buffer);
        }
    }
    return NULL;
}

void stop_client(int signum) {
    char stop_message[] = "STOP";
    sendto(client_socket, stop_message, strlen(stop_message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    close(client_socket);
    exit(EXIT_SUCCESS);
}
