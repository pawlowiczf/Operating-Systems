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
#include "memory.h"

#define KEY 1234 

int main (int argc, char** argv) {
    //
    int err; 

    if ( argc <  2 ) { perror ("Invalid or none number of printers"); return 1; }
    int printersNumber = atoi ( argv[1] );

    int shmID = shmget (KEY, sizeof(printers_t), 0666 | IPC_CREAT);
    if ( shmID < 0 ) { perror ("Error: shmget"); return 1; }

    printers_t* printers = shmat (shmID, NULL, SHM_W | SHM_R);
    printers->numberOfPrinters = printersNumber;

    // Utworzenie semaforow:
    printers->semID = semget (12345, printersNumber, 0666 | IPC_CREAT);
    if ( printers->semID < 0 ) { perror ("Error: semget IPC_CREAT"); return 1; }


    for (int a = 0; a < printersNumber; a++) {
        // 
        err = semctl (printers->semID, a, SETVAL, 5); // ustawienie wartosci zerowego semafora na 5 
        if (err < 0 ) { perror("Error - semctl"); return 1; }

        pid_t pid = fork (); 
        if ( pid < 0 ) { perror ("Error: fork()"); return 1; }

        if (pid == 0) {
            //
            while (1) {
                if ( printers->printersArray[a].state == WAITING ) { continue; }

                for (int b = 0; b < MAX_TEXT; b++) {
                    printf ("%c\n", printers->printersArray[a].buffer[b]);
                    fflush (stdout);
                    sleep (1);

                }

                printf("\n");
                fflush (stdout);

                // Increment semaphore to signalize free printer:
                printers->printersArray[a].state = WAITING; 

                struct sembuf semOp;
                semOp.sem_flg = 0;
                semOp.sem_op = 1; 
                semOp.sem_num = a; 
                struct sembuf semOps[] = {semOp}; 

                err = semop (printers->semID, semOps, 1);
                if ( err < 0 ) { perror ("Error: semop"); return 1; }
            }
        } 
    }

    wait (NULL);

}