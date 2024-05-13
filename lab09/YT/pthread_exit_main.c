#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

/*
Uzycie pthread_exit() w funkcji main powoduje zakonczenie funkcji main, ale wszystkie watki nadal pracuja! Gdyby nie uzycie pthread_exit() to wywolanie returna, czy zakonczenie main'a powodowaloby zakonczenie wszystkich watkow. 
*/

void* routine () {
    sleep (1); 
    printf ("Thread working: %ld\n", pthread_self());
}
int main () {
    //
    for (int a = 0; a < 3; a++) {
        pthread_t threadID; 
        pthread_create (&threadID, NULL, &routine, NULL); 
    }

    pthread_exit (0);
}