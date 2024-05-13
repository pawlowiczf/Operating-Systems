#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Returning value from thread:

void* exampleFunction (void* data) {
    //
    int* result = malloc ( sizeof (int) );
    if ( result == NULL ) { perror ("Error: malloc"); pthread_exit (NULL); }

    *result = 420; 
    pthread_exit ( (void*) result );
}

int main () {
    //
    int err;
    int* returned; 

    pthread_t threadID; 

    err = pthread_create (&threadID, NULL, &exampleFunction, NULL);
    if ( err < 0 ) { perror ("Error: pthread_create"); return 1; }

    pthread_join (threadID, (void**) &returned);
    printf ("The returned value from thread is: %d\n", *returned);

    free (returned);
}