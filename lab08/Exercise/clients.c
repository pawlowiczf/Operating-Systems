#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

#define KEY 1234 

void generateRandomString(char* buffer, int length) {
    // 
    for (int a = 0; a < length; a++) { buffer[a] = 'a' + rand() % 26; }
    buffer[length] = '\0';
}

int main (int argc, char** argv) {
    //
    int err; 

    if ( argc <  2 ) { perror ("Invalid or none number of clients"); return 1; }
    int clientsNumber = atoi ( argv[1] );

    int shmID = shmget (KEY, sizeof (printers_t), 0666);
    if ( shmID < 0 ) { perror ("Error: shmget"); return 1; }
    printers_t* printers = shmat (shmID, NULL, SHM_W | SHM_R);

    for (int a = 0; a < clientsNumber; a++) {
        //
        pid_t pid = fork ();
        if ( pid < 0 ) { perror ("Error: fork()"); return 1;}

        if (pid == 0) {
            
            while (1) {

                // Find free printer:
                int printerIndex = -1, val;
                for (int a = 0; a < printers->numberOfPrinters; a++) {
                    val = semctl ( printers->semID, a, GETVAL, 0);
                    if (val == 1) { printerIndex = a; break; }
                }

                // If all are busy, randomly pick one: 
                if ( printerIndex == -1 ) { printerIndex = rand() % printers->numberOfPrinters; }

                // Decrement semaphore by 1. 
                struct sembuf semOp;
                semOp.sem_flg = 0;
                semOp.sem_op = -1; 
                semOp.sem_num = printerIndex; 
                struct sembuf semOps[] = {semOp}; 
                err = semop (printers->semID, semOps, 1);

                char buffer[MAX_TEXT]; 
                generateRandomString(buffer, MAX_TEXT);
                strcpy ( printers->printersArray[printerIndex].buffer, buffer);
                printers -> printersArray[printerIndex].state = PRINTING; 

                sleep ( rand() % 3 + 1 ); 
            }
        }
    }

    wait (NULL);

    err = shmdt (printers);
    if ( err < 0 ) { perror ("Error: shmdt"); return 1; }

    err = shmctl (shmID, IPC_RMID, NULL);
    if ( err < 0 ) { perror ("Error: shmctl"); return 1; }
}   