#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY 1234 

/*
Shared memory w systemie V: 

1) Jesli chcemy uzyskac dostep do istniejacego juz segmentu pamieci to argument size powinien miec wartosc 0.

2) Drugim argumentem funkcji shmat jest wskaznik do miejsca w pamieci, gdzie system powinien dolaczyc segment pamieci wspolnej. Najczesciej dajemy tam NULL, czyli system sam dobierze to miejsce. Trzecim argumentem sa flagi. Flaga SHM_RDONLY oznacza, ze dolaczamy segment pamieci w trybie tylko do odczytu. 
*/

int main () {
    //
    int err; 

    int shmID = shmget (KEY, 512, 0666 | IPC_CREAT); // 512 - rozmiar wspodzielonej pamieci 
    if ( shmID < 0 ) { perror ("Error: shmget"); return 1; }

    void* shmAddress = shmat (shmID, NULL, SHM_RDONLY);// zwraca wskaznik do miejsca w pamieci, gdzie utworzono pamiec dzielona. 
    if ( shmAddress == (void*) -1 ) { perror ("Error: shmat"); return 1; }


    // Odlaczenie segmentu pamieci - detach - dt. 
    err = shmdt (shmAddress);
    if ( err < 0 ) { perror ("Error: shmdt"); return 1; }

    // Usuniecie segmentu pamieci 
    err = shmctl (shmID, IPC_RMID, NULL);
    if ( err < 0 ) { perror ("Error: smctl IPC_RMID"); return 1; }

    return 0;
}