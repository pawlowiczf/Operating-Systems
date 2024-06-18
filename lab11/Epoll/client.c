#include <sys/epoll.h>

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

int main (int argc, char* argv[]) {
    //
    // if ( argc < 2 ) { printf ("Not enough arguments\n"); return 1; }

    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons (9003); 
    inet_aton ("192.168.1.12", &servAddr.sin_addr); 
    memset (&servAddr.sin_zero, '\0', 8); 

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    int err = connect (serverSocket, (struct sockaddr*) &servAddr, sizeof(servAddr));
    

    shutdown (serverSocket, SHUT_RDWR);
    close (serverSocket); 
}   