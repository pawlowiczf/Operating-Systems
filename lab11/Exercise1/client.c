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
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 256
#define SERVER_IP "192.168.1.12"
int flag = 1; 

void sigHandler (int signal) {
    flag = 0; 
}

int main () {
    //
    signal (SIGINT, &sigHandler); 

    int err; 

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (serverSocket == -1) { perror ("Error: socket()"); return EXIT_FAILURE; }

    struct sockaddr_in sockAddr; // struktura adresu serwera

    err = inet_aton (SERVER_IP, &sockAddr.sin_addr);
    if (err == 0) { perror ("Error - inet_aton()"); return 1; }

    sockAddr.sin_family      = AF_INET; 
    sockAddr.sin_port        = htons (9002); 
    memset (&sockAddr.sin_zero, '\0', 8);

    err = connect (serverSocket, (struct sockaddr *) &sockAddr, sizeof(sockAddr)); 
    if (err == -1) { perror ("Error: connect"); return EXIT_FAILURE; }

    char buffer[BUFFER_SIZE];

    while (flag) {
        //
        fgets (buffer, BUFFER_SIZE, stdin);
        if ( strlen(buffer) <= 1 ) { flag = 0; break; }

        send (serverSocket, &buffer, strlen (buffer), 0);
    }

    shutdown (serverSocket, SHUT_RDWR);
    close (serverSocket); 

    puts ("Client ended its work");
    return 0; 
}