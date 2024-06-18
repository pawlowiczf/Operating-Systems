#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/un.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define SV_SOCK_PATH "/tmp/sock"
#define BUFF_SIZE 100

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
    signal (SIGINT, &sigHandler); 

    // err = remove (SV_SOCK_PATH); 
    // if (err != EEXIST) { checkErr (-1, "Error - remove()"); } 

    struct sockaddr_un addr; 
    int serverSocket = socket (AF_UNIX, SOCK_STREAM, 0); 

    memset ( &addr, 0, sizeof(struct sockaddr_un) );
    addr.sun_family = AF_UNIX; 
    strcpy (addr.sun_path, SV_SOCK_PATH);

    err = bind ( serverSocket, (struct sockaddr*) &addr, sizeof(addr) ); 
    checkErr (err, "Error - bind()"); 

    err = listen (serverSocket, 1); 
    checkErr (err, "Error - listen()"); 

    char buffer[BUFF_SIZE]; 
    int readBytes; 

    while (running) {
        //
        int clientSocket = accept (serverSocket, NULL, NULL); 
        
        puts ("Accepted client connection!");

        while ( ( readBytes = recv (clientSocket, buffer, BUFF_SIZE, 0) > 0 ) ) {
            printf ("Message read: %s\n", buffer); 
        }
        running = 0; 
    }


    err = remove (SV_SOCK_PATH); 
    close (serverSocket); 

    puts ("Ending server program!"); 
    return EXIT_SUCCESS; 
}