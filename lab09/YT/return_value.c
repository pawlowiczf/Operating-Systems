#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

void* rollDice () {
    //
    int result = rand () % 6 + 1; 
    int* returnValue = malloc ( sizeof(int) ); 

    *returnValue = result;
    return (void*) returnValue; 
}

int main () {
    //
    srand (time(NULL));
    int err; 

    pthread_t threadID; 
    int* returnValue; 

    err = pthread_create (&threadID, NULL, &rollDice, NULL); 
    if ( err < 0 ) { perror ("Error - pthread_create"); return 1; }

    err = pthread_join (threadID, (void**) &returnValue);
    if ( err < 0 ) { perror ("Error - pthread_join"); return 1; }

    printf ("The result is: %d\n", *returnValue);
    return 0; 
}