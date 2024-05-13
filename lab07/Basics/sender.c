#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAX_TEXT 50 

struct my_msg {
    long int msg_type;
    char some_text[MAX_TEXT];
};

// Message queues 
int main () {
    //
    char buffer[20];
    int running = 1;
    int msgid;

    struct my_msg some_data;

    int msqID = msgget( (key_t) 12345, 0666 | IPC_CREAT );
    if ( msqID < 0 ) { printf ("Error in msgget"); return 1; }

    while ( running ) {
        printf ("Enter the text: \n");
        fgets (buffer, 20, stdin); // automatycznie dodaje znak konca tekstu

        some_data.msg_type = 1; 

        strcpy (some_data.some_text, buffer);

        if ( msgsnd (msqID, &some_data, MAX_TEXT, 0) < 0 ) { printf ("Error in msgsnd"); return 1; }

        if ( strncmp (buffer, "end", 3) == 0 ) { running = 0; }

    }
    
}