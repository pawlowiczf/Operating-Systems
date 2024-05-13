#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>

#define MAX_BUFF 256

int main() {
    //
    struct mq_attr msg; 
    msg.mq_msgsize = MAX_BUFF;
    msg.mq_maxmsg = 15; 
    
    mqd_t msqID = mq_open ("/queue", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, msg);
    if ( msqID < 0 ) {
        perror("Error - mq_open");
        return 1;
    }

    char messageSend[] = "Halo Stachu";
    char messageReceived[MAX_BUFF];

    int error = mq_send (msqID, messageSend, strlen(messageSend), 0);
    if ( error < 0 ) { 
        perror("Error - mq_send"); 
        mq_close (msqID);
        mq_unlink ("/queue");    
        return 1;
    }

    int bytesRead = mq_receive (msqID, messageReceived, MAX_BUFF, NULL);

    if ( bytesRead < 0) {
        perror("Error - mq_receive");
        mq_close(msqID); 
        mq_unlink("/queue"); 
        return 2;
    }

    // messageReceived[bytesRead] = '\0';
    printf ("Message received: %s\n", messageReceived);

    mq_close (msqID);
    mq_unlink ("/queue");
    return 0; 
}