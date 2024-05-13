#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFF 256

/*
    Napisz program, w ktorym stworzysz kolejke komunikatow, wstawisz tam dwa komunikaty: "Witaj swiecie" oraz "Cala naprzod". Nastepnie odbierz wszystkie komunikaty z tej kolejki zapisujac je do innego bufora i wypisz je na ekran.
*/

struct msg {
    long msg_type;
    char msg_spot[ MAX_BUFF ]; 
} messageSend, messageReceive;

int main() {
    //
    messageSend.msg_type = 1;
    messageReceive.msg_type = 1;

    int msqID = msgget ( ftok(".", 123), 0666 | IPC_CREAT ); 
    if ( msqID < 0 ) { return 3; }

    strcpy (messageSend.msg_spot, "Witaj swiecie");

    if ( msgsnd (msqID, &messageSend, sizeof(messageSend), 0)  < 0 ) { return 1; }

    if ( msgrcv (msqID, &messageReceive, sizeof(messageReceive), 1, 0) < 0 ) { return 2; }

    printf ("Message received: %s\n", messageReceive.msg_spot);

    msgctl (msqID, IPC_RMID, NULL);
    return 0;
}