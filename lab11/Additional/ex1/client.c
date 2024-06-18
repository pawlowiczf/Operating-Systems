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
#include <fcntl.h>

#define SV_SOCK_PATH "/tmp/sock"
#define BUFF_SIZE 100

int err; 
void checkErr (int err, char* msg) {
    //
    if (err < 0) {
        perror (msg); 
        exit (1); 
    }
}

int main () {
    //
    struct sockaddr_un addr; 
    char buffer[BUFF_SIZE]; 
    int readBytes; 

    int serverSocket = socket (AF_UNIX, SOCK_STREAM, 0); 
    checkErr (serverSocket, "Error - socket()"); 

    memset ( &addr, 0, sizeof (struct sockaddr_un) ); 
    addr.sun_family = AF_UNIX; 
    strcpy (addr.sun_path, SV_SOCK_PATH); 

    err = connect (serverSocket, (struct sockaddr*) &addr, sizeof(addr)); 
    checkErr (err, "Error - connect()"); 

    while ( ( readBytes = read (STDIN_FILENO, buffer, BUFF_SIZE) ) > 0 ) {
        //
        err = send (serverSocket, &buffer, strlen(buffer) + 1, 0); 
        checkErr (err, "Error - send()"); 
    }

    close (serverSocket); 
}