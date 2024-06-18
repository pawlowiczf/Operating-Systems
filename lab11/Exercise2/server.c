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
#include "queue.h" 

#define THREAD_POOL_SIZE 20 
#define BACKLOG 100
#define PORT 9002 
#define IP_ADDRESS "192.168.1.12"
#define BUFFER_SIZE 256

pthread_t threadPool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

int flag = 1; 

void checkErr (int err, char* msg); 

void handleConnection (int* clientSocket_p) {
    //
    int clientSocket = *clientSocket_p;
    free (clientSocket_p); 

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
}

void* threadFunction (void* data) {
    //
    while (flag) {
        //
        pthread_mutex_lock (&mutex);
        pthread_cond_wait (&cond, &mutex); 

        int* clientSocket_p = deQueue (); 

        pthread_mutex_unlock (&mutex); 

        if (clientSocket_p == NULL) { continue; }
        handleConnection (clientSocket_p); 
    }
}

void setupThreadPool () {
    //
    int err; 

    for (int a = 0; a < THREAD_POOL_SIZE; a++) {
        err = pthread_create (&threadPool[a], NULL, &threadFunction, NULL); 
        checkErr (err, "Error - pthread_create");
    }
}

// Binduje gniazdo serwera do odpowiedniego portu i adresu IP
void setupServerSocket (int serverSocket) {
    //
    struct sockaddr_in serverAddr;

    int err = inet_pton (AF_INET, IP_ADDRESS, &serverAddr.sin_addr); 
    if (err <= 0) { perror ("Error - inet_pton()"); exit (1); }

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port   = htons(PORT); 
    memset (serverAddr.sin_zero, '\0', 8);

    int yes = 1; 
    setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    err = bind (serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)); 
    checkErr (err, "Error - bind()");
}

void sigHandler (int signal) {
    //
    flag = 0; 
    pthread_cond_broadcast (&cond); 
    puts ("");
    // for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
    //     pthread_join(threadPool[i], NULL);
    // }
}

int main () {
    //
    int number = 0, maxNumber = 2; 

    signal (SIGINT, &sigHandler);
    int err; 

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setupServerSocket (serverSocket); 
    setupThreadPool ();

    err = listen (serverSocket, BACKLOG); 
    checkErr (err, "Error - listen()"); 

    while (number < maxNumber) {
        //
        int clientSocket = accept (serverSocket, NULL, NULL); 
        checkErr (clientSocket, "Error - accept()"); 

        puts ("Connection with a client was established:");

        int* clientSocket_p = malloc (sizeof(int)); 
        if ( clientSocket_p == NULL ) { perror ("Error - malloc()"); exit (1); }

        *clientSocket_p = clientSocket; 

        pthread_mutex_lock (&mutex);
        enQueue (clientSocket_p); 
        pthread_cond_signal (&cond); 
        pthread_mutex_unlock (&mutex);

        number++; 
    }

    shutdown (serverSocket, SHUT_RDWR); 
    close (serverSocket); 

    pthread_exit (0);
}

void checkErr (int err, char* msg) {
    //
    if (err < 0) { perror(msg); exit (1); }
}