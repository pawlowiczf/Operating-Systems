#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void* routine() {
    printf ("Test from threads\n");
    sleep (1); 
    printf ("Ending thread\n");
}

int main (int argc, char* argv[]) {
    //
    int err; 
    pthread_t threadID; 

    err = pthread_create (&threadID, NULL, &routine, NULL);
    if ( err < 0 ) { perror ("Error - pthread_create"); return 1; }

    err = pthread_join (threadID, NULL);
    if ( err < 0 ) { perror ("Error - pthread_join"); return 1; }

    return 0; 
}