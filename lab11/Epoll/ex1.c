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

#define MAX_EVENTS 5
#define READ_SIZE 10

int err; 
int running = 1;
char read_buffer[READ_SIZE + 1];

void checkErr (int err, char* msg); 

int main () {
    //
    struct epoll_event event, events[MAX_EVENTS];
    event.data.fd = STDIN_FILENO; 
    event.events = EPOLLIN; // mozliwy odczyt  

    int epollFD = epoll_create1 (0); 
    checkErr (epollFD, "Error - epoll_create1");

    // Mozliwe opcje: EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD
    epoll_ctl (epollFD, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    while (running) {
        //
        puts ("Waiting for input:"); 

        int numberOfEvents = epoll_wait (epollFD, events, MAX_EVENTS, 30000); 
        checkErr (numberOfEvents, "Error - epoll_wait()"); 

        printf("%d ready events\n", numberOfEvents);

        for (int i = 0; i < numberOfEvents; i++) {
            //
            printf ("Reading file descriptor '%d' -- \n", events[i].data.fd);

            int bytesRead = read (events[i].data.fd, read_buffer, READ_SIZE);
            printf ("%d bytes read.\n", bytesRead);
            read_buffer[bytesRead] = '\0';

            printf ("Read '%s'\n", read_buffer);
    
            if( strncmp(read_buffer, "stop\n", 5) == 0 ) { running = 0; }
		}
    }

    close (epollFD);
    return 0;
}

void checkErr (int err, char* msg) {
    //
    if ( err < 0 ) {
        perror (msg); 
        exit (1);
    }
}
