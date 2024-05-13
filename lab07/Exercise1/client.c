#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

int sendInitMessage( key_t clientMsgQueueKey, int msgQueueServerID ) {
    //
    Message messageSend = {};

    messageSend.callType = INIT; 
    messageSend.clientMsgQueueKey = clientMsgQueueKey;
    messageSend.messageType = 1;

    int msgQueueClientID = msgget (clientMsgQueueKey, 0666);
    if ( msgQueueClientID < 0 ) { perror ("Client - open queue error"); return 1; }

    if ( msgsnd (msgQueueServerID, &messageSend, sizeof(messageSend) - sizeof(long), 0) < 0 ) {
        perror ("Client - send message error");
        return 1; 
    }

    Message messageReceived = {};

    if ( msgrcv (msgQueueClientID, &messageReceived, sizeof(messageReceived) - sizeof(long), 1, 0) < 0 ) {
        perror ("Client - receive message error");
        return 1;
    }

    return messageReceived.clientID; 
}

int main () {
    //
    srand ( time(NULL) );
    key_t clientMsgQueueKey = rand() % 900 + 100;

    int msgQueueClientID = msgget (clientMsgQueueKey, 0666 | IPC_CREAT);
    int msgQueueServerID = msgget ( (key_t) SERVER_ID, 0666 );

    if (msgQueueClientID < 0) { perror("Client - open client queue error"); return 1; }
    if (msgQueueServerID < 0) { perror("Client - open server queue error"); return 1; }
    
    int clientID = sendInitMessage( clientMsgQueueKey, msgQueueServerID );

    pid_t pid = fork (); 
    if ( pid < 0 ) { printf ("Error - fork()"); return 1; }

    if ( pid != 0 ) {

        while (1) {
            printf ("What message are you going to send to other clients?\n");
            fflush (stdout);

            char message[ MAX_TEXT ];
            fgets (message, MAX_TEXT, stdin);

            Message messageSend = {}; 
            messageSend.callType = OTHER;
            messageSend.messageType = 1;
            messageSend.clientID = clientID;
            messageSend.clientMsgQueueKey = clientMsgQueueKey;
            strcpy ( messageSend.message, message );


            if ( msgsnd (msgQueueServerID, &messageSend, sizeof(messageSend) - sizeof(long), 0) < 0 ) {
                perror ("Client - message send error");
                return 2; 
            }
        }
    }
    else {
        int msgQueueClientID = msgget (clientMsgQueueKey, 0666);
        Message messageReceived = {}; 

        while (1) {
            //
            if ( msgrcv (msgQueueClientID, &messageReceived, sizeof(messageReceived) - sizeof(long), 1, 0) < 0 ) {
                perror ("Client - message receive error in parent process");
                return 3; 
            }
            puts ( messageReceived.message );
        }
    }
    
    msgctl (msgQueueClientID, IPC_RMID, NULL);
    return 0;
}