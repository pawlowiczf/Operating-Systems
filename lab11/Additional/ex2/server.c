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
#define BUFF_SIZE 256
#define SERVER_IP "192.168.43.113"

int running = 1; 
int err; 
void checkErr (int err, char* msg) {
    //
    if (err < 0) {
        perror (msg); 
        exit (1); 
    }
}

void sigHandler (int signal) {
    running = 0; 
}

int main () {
    //
    signal (SIGINT, sigHandler); 
    struct sockaddr_in addr; 

    addr.sin_port = htons (8080); 
    addr.sin_family = AF_INET; 
    inet_aton (SERVER_IP, &addr.sin_addr); 

    int serverSocket = socket (AF_INET, SOCK_STREAM, 0); 
    checkErr (serverSocket, "Error - socket()"); 

    err = bind (serverSocket, (struct sockaddr*) &addr, sizeof (addr));  
    checkErr (err, "Error - bind()"); 

    err = listen (serverSocket, MAX_CONNECTION_NUMBER);
    checkErr (err, "Error - listen()"); 

    int readBytes; 
    char buffer[BUFF_SIZE]; 

    while (running) {
        //
        int clientSocket = accept (serverSocket, NULL, NULL); 
        puts ("Accepted client connection!");

        pid_t pid = fork (); 

        if (pid == 0) {
            //
            while ( ( readBytes = recv (clientSocket, buffer, BUFF_SIZE, 0) > 0 ) ) {
                printf ("Message read: %s\n", buffer); 
            }     
            close (clientSocket);   
        }

    }

    close (serverSocket);
    return EXIT_SUCCESS; 
}