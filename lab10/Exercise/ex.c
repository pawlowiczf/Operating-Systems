#include <pthread.h>

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define CYCLE_NUMBER 2
#define REINDEERS_NUMBER 9 

void checkErr(int, char*);

// Global variables:
int reindeersCounter = 0; 

pthread_mutex_t reindeersCounterMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeersCounterCond = PTHREAD_COND_INITIALIZER; 

pthread_mutex_t reindeersStartAgainMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t reindeersStartAgainCond = PTHREAD_COND_INITIALIZER;


void* reindeerVacation (void* data) {
    //
    for (int a = 0; a < CYCLE_NUMBER; a++) {
        // 
        int sleepTime = 5 + rand () % 6;
        sleep (sleepTime);

        pthread_mutex_lock (&reindeersCounterMutex);

        printf ("Reinder %d is returning to santa!\n", reindeersCounter);
        reindeersCounter++; 
        pthread_cond_signal (&reindeersCounterCond);

        pthread_mutex_unlock (&reindeersCounterMutex);

        pthread_mutex_lock (&reindeersStartAgainMutex);
        pthread_cond_wait (&reindeersStartAgainCond, &reindeersStartAgainMutex);
        pthread_mutex_unlock (&reindeersStartAgainMutex);
    }
}

void* santaSleeping (void* data) {
    //
    for (int a = 0; a < CYCLE_NUMBER; a++) {
        //
        pthread_mutex_lock (&reindeersCounterMutex); 

        while ( reindeersCounter < REINDEERS_NUMBER ) {
            //
            pthread_cond_wait (&reindeersCounterCond, &reindeersCounterMutex);
        }

        pthread_mutex_unlock (&reindeersCounterMutex); 
        reindeersCounter = 0;

        puts ("Santa: I'm waking up!");
        puts ("Santa: I'm giving toys to everyone!");
        sleep (3); 
        puts ("Santa: I'm going to sleep soon :("); 

        pthread_cond_broadcast (&reindeersStartAgainCond);
    }
}

int main () {
    //
    int err;
    pthread_t reindeers[9];
    pthread_t santa; 

    for (int a = 0; a < 9; a++) {
        err = pthread_create (&reindeers[a], NULL, &reindeerVacation, NULL);
        checkErr (err, "Reindeers pthread_create");
    } 
    err = pthread_create (&santa, NULL, &santaSleeping, NULL); 
    checkErr (err, "Santa pthread_create");

    err = pthread_join (santa, NULL);
    checkErr (err, "Santa pthread_join");
    
    for (int a = 0; a < 9; a++) {
        err = pthread_join (reindeers[a], NULL);
        checkErr (err, "Reindeers pthread_join");
    }

    puts ("Santa won't wake up again :(");
}

// Prints error and exits program, if the error value is less than 0. 
void checkErr(int err, char* message) {
    //
    if ( err >= 0 ) { return; }
    perror (message); 
    exit(1);
}