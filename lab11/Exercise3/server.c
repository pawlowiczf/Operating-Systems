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
#include "common.h"

Client clients[MAX_CLIENTS_NUMBER]; 
int currentClientsNumber = 0; 

void checkErr (int err, char* msg); 
void setupServerSocket (int serverSocket, char* ServerIP, int ServerPort);
void* acceptConnRoutine (void* data);
void setupNewClient (int clientSocket);
void removeClientFromChat (int clientID); 
void sendMessageToAllMembers (int clientID);
void sendMessageToChatMember (int, int); 
void listAllClientsFromChat (int);

int err; 

int epollFD; 
struct epoll_event event, events[MAX_CLIENTS_NUMBER]; 

int running = 1; 
pthread_t acceptConnThread; 
Message message; 

int main (int argc, char* argv[]) {
    //
    if (argc < 2) { perror ("Error - not enough argments"); return 1; }
    char* ServerIP = argv[2]; 
    int ServerPort = atoi (argv[1]);

    // Setting all free clients:
    for (int a = 0; a < MAX_CLIENTS_NUMBER; a++) { clients[a].empty_b = 1; }

    // Seting server socket to listen and accept connection:
    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    setupServerSocket (serverSocket, ServerIP, ServerPort); 

    // Monitoring multiple client sockets:
    epollFD = epoll_create1 (0); 

    // Routine responsible for accepting calls and updating epoll structure. 
    err = pthread_create (&acceptConnThread, NULL, &acceptConnRoutine, &serverSocket); 
    checkErr (err, "Error - pthread_create"); 

    while (running) {
        //
        int numberOfEvents = epoll_wait (epollFD, events, MAX_CLIENTS_NUMBER, 300000); 
            
        printf ("Number of events: %d\n", numberOfEvents);

        for (int a = 0; a < numberOfEvents; a++) {
            //
            int clientID = events[a].data.fd;
            // if ( clients[clientID].nowRegistered == 1 ) { clients[clientID].nowRegistered = 0; continue; }

            int clientSocket = clients[clientID].clientSocket;

            int readBytes = recv (clientSocket, &message, MESSAGE_SIZE, 0);
            checkErr (readBytes, "Error - recv in while loop");

            printf ("[Server] |> Received message from client ID %d\n", clientID);
            printf ("[Server] |> Message: %s\n", message.message); 
            printf ("[Server] |> Message type: %d\n", message.messageType);

            switch (message.messageType)
            {
                case MT_STOP:
                    removeClientFromChat (clientID);
                    break;
                
                case MT_LIST:
                    listAllClientsFromChat (clientID); 
                    break;

                case MT_SEND_ALL:
                    sendMessageToAllMembers (clientID);
                    break;

                case MT_SEND_ONE:  
                    sendMessageToChatMember (clientID, message.toClientID); 
                    break;

                case MT_STOP_SERVER:
                    running = 0; 
            }
        }
        
    }

    shutdown (serverSocket, SHUT_RDWR); 
    close (serverSocket); 

    for (int a = 0; a < MAX_CLIENTS_NUMBER; a++) { shutdown (clients[a].clientSocket, SHUT_RDWR); close (clients[a].clientSocket); }
    puts ("Ending server program");
    return EXIT_SUCCESS;
}

// Sends message to every member of chat except clientID
void sendMessageToAllMembers (int clientID) {
    //
    puts ("[Server] |> Sending message to all chat members");

    for (int a = 0; a < MAX_CLIENTS_NUMBER; a++) {
        //
        if (a == clientID || clients[a].empty_b == 1) { continue; }

        sendMessageToChatMember (clientID, a); 
    }
}

void sendMessageToChatMember (int fromClientID, int toClientID) {
    //
    printf ("[Server] |> Sending message from %d to %d\n", fromClientID, toClientID);

    send (clients[toClientID].clientSocket, &message, MESSAGE_SIZE, 0);
}

void listAllClientsFromChat (int clientID) {
    //
    puts ("[Server] |> Listing all clients from chat");

    for (int a = 0; a < MAX_CLIENTS_NUMBER; a++) {
        //
        if (clients[a].empty_b == 1) { continue; }

        printf ("%d ", a); 
    }

    puts ("");
}

void removeClientFromChat (int clientID) {
    // 
    puts ("[Server] |> Removing client from server");

    clients[clientID].empty_b = 1; 
    clients[clientID].clientSocket = -1; 
}

int getFreeClientID () {
    //
    if ( currentClientsNumber >= MAX_CLIENTS_NUMBER ) { return -1; }

    for (int a = 0; a < MAX_CLIENTS_NUMBER; a++) {
        if ( clients[a].empty_b == 1 ) { return a; }
    }

    return -1; 
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

// Adds a client to static array and adds to epoll struct (monitores new socket)
void setupNewClient (int clientSocket) {
    //
    int clientID = getFreeClientID (); 
    if (clientID == -1) {
        printf ("Too many clients already! Failed during registering\n");
        return; 
    }

    clients[clientID].empty_b = 0; 
    clients[clientID].id = clientID; 
    clients[clientID].clientSocket = clientSocket; 
    clients[clientID].nowRegistered = 1; 

    currentClientsNumber++; 

    message.fromClientId = clientID; 
    message.messageType = 0;
    // send (clientSocket, &message, MESSAGE_SIZE, 0);

    // event.data.fd = clientSocket; 
    event.data.fd = clientID; 
    event.events = EPOLLIN | EPOLLET;   

    err = send (clientSocket, &message, MESSAGE_SIZE, 0); 

    epoll_ctl (epollFD, EPOLL_CTL_ADD, clientSocket, &event);    
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

void checkErr (int err, char* msg) {
    //
    if ( err < 0 ) { 
        perror (msg); 
        exit (1); 
    }
}