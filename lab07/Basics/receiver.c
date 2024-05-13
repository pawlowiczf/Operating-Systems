#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>

struct my_msg {
    long int msg_type;
    char some_text[BUFSIZ];
};

int main () {
    //
    int running = 1; 
    struct my_msg some_data; 
    long int msg_to_rec = 0;

    int msqID = msgget ( (key_t) 12345, 0666 | IPC_CREAT );

    while ( running ) {
        msgrcv (msqID, &some_data, BUFSIZ, msg_to_rec, 0); // 0 - najstarszy komunikat w kolejce
        printf ("Data received: %s\n", some_data.some_text);

        if ( strncmp (some_data.some_text, "end", 3) == 0 ) { running = 0; }
    }

    // msgctl (msqID, IPC_RMID, NULL);
    struct msqid_ds queueStruct; 
    msgctl (msqID, IPC_STAT, &queueStruct);

    int numberOfMessages = queueStruct.msg_qnum;

    printf ("Number of messages in queue: %d\n", numberOfMessages);

    msgctl (msqID, IPC_RMID, NULL);
}