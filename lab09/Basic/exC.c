#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

// Cancelling thread from another thread: 

// pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL) - mozna tez PTHREAD_CANCEL_ENABLE 
// pthread_setcanceltype () - to uzywamy, gdy watek dopuszcza anulowanie. Mozliwe flagi to: PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DEFERRED 

void* exampleFunction (void* data) {
    //
    int err = pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL); // pozwolenie na cancelowanie
    if ( err < 0 ) { perror ("Error: pthread_setcancelstate"); pthread_exit (NULL); }

    err = pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
    if ( err < 0 ) { perror ("Error: pthread_setcanceltype"); pthread_exit (NULL); }

    while (1) {
        puts ("I'm still alive!");
        usleep (500000);
        fflush (stdout);
    }

    pthread_exit (NULL);
}

int main () {
    //
    int err; 
    int* returned; 
    pthread_t threadID; 

    err = pthread_create (&threadID, NULL, &exampleFunction, NULL);
    if ( err < 0 ) { perror ("Error: pthread_create"); return 1; }

    sleep (3);
    err = pthread_cancel (threadID);
    if ( err < 0 ) { perror ("Error: pthread_cancel"); return 1; }

    err = pthread_join (threadID, (void**) &returned);
    if ( err < 0 ) { perror ("Error: pthread_join"); return 1; }

    if ( returned == PTHREAD_CANCELED ) {
        puts ("Watek poboczny zostal anulowany!");
    }

    puts ("Koniec watku glownego");
    return 0;   
}