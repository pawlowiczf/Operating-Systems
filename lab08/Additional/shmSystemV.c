#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define KEY 1234

/*
Napisz program, w ktorym stworzysz pamiec wspolna przylaczyc ja do procesu. Zapiszesz w niej wartosc typu cala naprzod, odlaczysz pamiec od procesu. Przylaczysz pamiec do procesu ponownie i odczytasz do innego bufora zawartosc pamieci i wypiszesz ja na ekranie.
*/

int main() {
    //
    pid_t pid; 
    char* buffer;

    pid = fork (); 
    if ( pid < 0 ) { perror ("Error: fork()"); return 1; }

    if ( pid == 0 ) {
        //
        sleep (1);

        int shmID = shmget (KEY, 0, 0);
        char* buffer = (char*) shmat ( shmID, NULL, 0); 

        printf ("%s", buffer);
    }
    else {
        //
        int shmID = shmget (KEY, 512, 0666 | IPC_CREAT);
        if ( shmID < 0 ) { perror ("Error: shmget()"); return 1; }
    
        char* buffer = (char*) shmat ( shmID, NULL, 0); 
        strcpy (buffer, "Cala naprzod!\n");
        
        wait (NULL); 
    }

    shmdt (buffer);
    shmctl (KEY, IPC_RMID, NULL);

}