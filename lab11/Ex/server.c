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

#define MAX_CONNECTION_NUMBER 10
#define BUFFER_SIZE 256
#define SERVER_IP "127.0.0.1"

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
    // sockAddr.sin_addr.s_addr = INADDR_ANY; 
    sockAddr.sin_family      = AF_INET; 
    sockAddr.sin_port        = htons (9002); 
    
    err = bind (serverSocket, (struct sockaddr*) &sockAddr, sizeof(sockAddr));
    if (err == -1) { perror ("Error: bind()"); return EXIT_FAILURE; }

    err = listen (serverSocket, MAX_CONNECTION_NUMBER);
    if (err == -1) { perror ("Error: listen()"); return EXIT_FAILURE; }
    
    while (flag) {
        //
        int newSocket = accept (serverSocket, NULL, NULL); 
        if (newSocket == EWOULDBLOCK) { continue; }
        if (newSocket == -1) { perror ("Error: accept()"); return EXIT_FAILURE; }

        int* clientSocket = malloc ( sizeof(int) ); 
        *clientSocket = newSocket; 

        pthread_create (&thread, NULL, &handleConnection, (int*) clientSocket);
        pthread_detach (thread);
    }

    close ( serverSocket ); 

    puts ("Server ended its work");
    return 0; 
}