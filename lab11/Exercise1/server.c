#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <limits.h> 
#include <unistd.h> 
#include <endian.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#define MAX_CONNECTION_NUMBER 10
#define BUFFER_SIZE 256
#define SERVER_IP "192.168.1.12"

int flag = 1; 

void* handleConnection(void* data) {
    //
    int clientSocket = *(int*) data; 
    free ( (int*) data ); 

    int readBytes; 
    char buffer[BUFFER_SIZE];

    while ( ( readBytes = recv (clientSocket, buffer, BUFFER_SIZE, 0) ) > 1 ) {
        //
        buffer[readBytes] = '\0';
        printf ("|> %s", buffer); 
    }

    shutdown (clientSocket, SHUT_RDWR);
    close (clientSocket); 

    puts ("Connection with a client ended");
    pthread_exit (NULL);
}

void sigHandler (int signal) {
    flag = 0; 
}

int main () {
    //
    signal (SIGINT, &sigHandler);

    int err; 
    pthread_t thread; 

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (serverSocket == -1) { perror ("Error: socket()"); return EXIT_FAILURE; }

    struct sockaddr_in sockAddr; 
    inet_aton (SERVER_IP, &sockAddr.sin_addr);
    sockAddr.sin_family      = AF_INET; 
    sockAddr.sin_port        = htons (9002); 
    memset (&sockAddr.sin_zero, '\0', 8);

    err = bind (serverSocket, (struct sockaddr*) &sockAddr, sizeof(sockAddr));
    if (err == -1) { perror ("Error: bind()"); return EXIT_FAILURE; }

    err = listen (serverSocket, MAX_CONNECTION_NUMBER);
    if (err == -1) { perror ("Error: listen()"); return EXIT_FAILURE; }
    
    while (flag) {
        //
        int newSocket = accept (serverSocket, NULL, NULL); 
        if (newSocket == -1) { perror ("Error: accept()"); return EXIT_FAILURE; }

        int* clientSocket = malloc ( sizeof(int) ); 
        *clientSocket = newSocket; 

        pthread_create (&thread, NULL, &handleConnection, (int*) clientSocket);
        pthread_detach (thread);

        // Na razie ten server akceptuje tylko jedno polaczenie i wychodzi. 
        shutdown (serverSocket, SHUT_RDWR);
        close (serverSocket);
        flag = 0; 
    }

    shutdown (serverSocket, SHUT_RDWR);
    close ( serverSocket ); 

    puts ("Main thread ended its work");
    pthread_exit (0); 
}