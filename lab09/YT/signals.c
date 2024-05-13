#include <pthread.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

// Watek blokuje otrzymanie sygnalu SIGINT. Mozna tez w procesie glownym uzyc funkcji signal, aby wskazac, jak dane watki maja obslugiwac dany sygnal. 

void* routine () {
    //
    int err; 
    // sigset_t sigMasks;

    // err = sigemptyset (&sigMasks); 
    // if ( err < 0 ) { perror ("Error: sigemptyset()"); return NULL; }

    // err = sigaddset (&sigMasks, SIGINT);
    // if ( err < 0 ) { perror ("Error: sigaddset()"); return NULL; }

    // err = pthread_sigmask (SIG_BLOCK, &sigMasks, NULL);
    // if ( err < 0 ) { perror ("Error: sigmask()"); return NULL;}

    puts ("Started thread");
    sleep (5);
    puts ("Ending thread");
    return NULL;
}

void sigHandler (int signal) {
    puts ("Handling signal...");
    sleep (1);
    puts ("Signal handled");
}

int main () {
    //
    signal (SIGINT, &sigHandler);

    int err; 
    pthread_t threadID; 

    err = pthread_create (&threadID, NULL, &routine, NULL);
    if ( err < 0 ) { perror ("Error: pthread_create"); return 1; }

    sleep (1);

    err = pthread_kill (threadID, SIGINT);
    if ( err < 0 ) { perror ("Error: pthread_kill"); return 1; }
    
    err = pthread_join (threadID, NULL);
    if ( err < 0 ) { perror ("Error: thread_join"); return 1; }

    return 0;
}