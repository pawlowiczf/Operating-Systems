#include <sys/epoll.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <limits.h> 
#include <unistd.h> 
#include <endian.h>

#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

int err; 
int running = 1; 
struct epoll_event event, events[8]; 
int epollFD; 

void checkErr (int err, char* msg) {
    //
    if ( err < 0 ) { 
        perror (msg); 
        exit (1); 
    }
}

void setupServerSocket (int serverSocket, char* ServerIP, int ServerPort) {
    //
    struct sockaddr_in serverAddr; 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons (ServerPort); 

    printf ("ServerIP: %s\n", ServerIP);
    printf ("ServerPort: %d\n", ServerPort);

    err = inet_aton (ServerIP, &serverAddr.sin_addr);
    if (err == 0) { perror ("Error - inet_aton == 0"); exit (1); }
    checkErr (err, "Error - inet_aton");

    memset (&serverAddr.sin_zero, '\0', 8);

    int yes = 1; 
    err = setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    checkErr (err, "Error - sesockopt()");

    err = bind (serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    checkErr (err, "Error - bind()");

    err = listen (serverSocket, 25); 
    checkErr (err, "Error - listen()");
}

// Adds a client to static array and adds to epoll struct (monitores new socket)
void setupNewClient (int clientSocket) {
    //
    event.data.fd = clientSocket; 
    event.events = EPOLLIN | EPOLLET;   

    epoll_ctl (epollFD, EPOLL_CTL_ADD, clientSocket, &event);    
}

// Accepts clients connections and registeres them in the static-array
void* acceptConnRoutine (void* data) {
    //
    int serverSocket = *(int*) data; 

    while (running) {
        //
        int clientSocket = accept (serverSocket, NULL, NULL); 
        checkErr (clientSocket, "Error - clientSocket - accept()");

        puts ("[Server] |> Accepted client connection\n"); 

        setupNewClient (clientSocket);
    }   
}

int main () {
    //
    epollFD = epoll_create1 (0); 

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    setupServerSocket (serverSocket, "192.168.1.12", 9003); 

    pthread_t thread; 
    pthread_create (&thread, NULL, &acceptConnRoutine, (void*) &serverSocket); 
    sleep (10);
    while (1) {
        int numberOfEvents = epoll_wait (epollFD, events, 8, 3000000000); 
        if ( numberOfEvents == -1) { perror ("Error"); }
        printf ("Number of events: %d\n", numberOfEvents); 

    }


    puts ("Ending main program"); 
}

