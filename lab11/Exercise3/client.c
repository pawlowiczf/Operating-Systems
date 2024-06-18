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

Message message; 
pthread_t thread; 
int err; 
int fromClientID; 
int runningReading = 1;

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

    err = inet_aton (ServerIP, &serverAddr.sin_addr);
    if (err == 0) { perror ("Error - inet_aton == 0"); exit (1); }
    checkErr (err, "Error - inet_aton");

    memset (&serverAddr.sin_zero, '\0', 8);

    int err = connect (serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    checkErr (err, "Error - connect to server"); 
}

void* receiveMessageRoutine (void* data) {
    //
    int serverSocket = *(int*) data; 

    while (runningReading) {
        //
        int readBytes = recv (serverSocket, &message, MESSAGE_SIZE, 0);
        if ( readBytes == 0 ) { runningReading = 0; }

        printf ("Message:\n %s\n", message.message);
    }   
}

MessageType toMessageType(const char *str) {
    //
    if (strcmp(str, "LIST") == 0)
        return MT_LIST;
    if (strcmp(str, "2ALL") == 0)
        return MT_SEND_ALL;
    if (strcmp(str, "2ONE") == 0)
        return MT_SEND_ONE;
    if (strcmp(str, "STOP") == 0)
        return MT_STOP_SERVER;

    printf("[Client] Wrong command (allowed: [LIST|2ALL|2ONE|STOP]). Exiting...\n");
    exit(1);
}

void stop (int serverSocket) {
    //
    runningReading = 0; 
    message.messageType = MT_STOP_SERVER; 

    err = send (serverSocket, &message,  MESSAGE_SIZE, 0); 
    checkErr (err, "Error - send stop message"); 
}

void list (int serverSocket) {
    //
    message.messageType = MT_LIST; 

    err = send (serverSocket, &message, MESSAGE_SIZE, 0); 
    checkErr (err, "Error - send list message");
}

void sendAll (int serverSocket) {
    //
    message.messageType = MT_SEND_ALL; 

    err = send (serverSocket, &message, MESSAGE_SIZE, 0); 
    checkErr (err, "Error - sendAll"); 
}

void sendOne (int serverSocket) {
    //
    message.messageType = MT_SEND_ONE; 
    
    err = send (serverSocket, &message, MESSAGE_SIZE, 0); 
    checkErr (err, "Error - sendOne"); 
}

int main (int argc, char* argv[]) {
    //
    if ( argc < 2 ) { printf ("Not enough arguments\n"); return 1; }
    char* ServerIP = argv[2]; 
    int ServerPort = atoi (argv[1]);

    int serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    setupServerSocket (serverSocket, ServerIP, ServerPort);

    err = recv (serverSocket, &message, MESSAGE_SIZE, 0); 
    fromClientID = message.fromClientId; 
    checkErr (err, "Error - recv"); 

    err = pthread_create (&thread, NULL, &receiveMessageRoutine, (void*) &serverSocket); 
    checkErr (err, "Error - pthread_create"); 

    char buffer[32];
    char msg[MAX_MESSAGE_SIZE] = "";

    while (fgets(buffer, 32, stdin) != NULL) {
        //
        char command[32];
        int clientID;

        sscanf(buffer, "%s", command);

        MessageType messageType = toMessageType(command);

        switch (messageType) {
            //
            case MT_STOP_SERVER:
                puts ("Ending server program...");
                stop(serverSocket);
                break;

            case MT_LIST:
                list(serverSocket);
                break;

            case MT_SEND_ALL:
                sscanf (buffer, "%*s %[^\r\n]", msg);
                strcpy (message.message, msg);
                sendAll (serverSocket);
                break;

            case MT_SEND_ONE:
                sscanf(buffer, "%*s %d %[^\r\n]", &clientID, msg);
                message.toClientID = clientID;
                strcpy(message.message, msg);
                sendOne(serverSocket);
                break;
        }

    }
    return EXIT_SUCCESS; 
}   