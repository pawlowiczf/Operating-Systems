#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <pthread.h>

#include <stdio.h>
#include <errno.h>

// Kod do kompilacji: gcc -pthread semPOSIX.c -o semPOSIX 
// Nalezy zlinkowac biblioteke pthread. 

/*
Semafory w POSIX:

1) sem_wait moze byc operacja blokujaca. Mozna uzyc sem_trywait, ktory w przypadku blokowania zwraca -1, ale do zmienna errno ustawia na EAGAIN.

*/

int main () {
    //
    int err, currentValue; 

    // Funkcja sem_open zwraca wskaznik do semafora.
    sem_t* semAddress = sem_open ("/semaphore", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 5); // 5 - poczatkowa wartosc semafora
    if ( semAddress == SEM_FAILED ) { perror ("Error: sem_open"); return 1; }

    err = sem_post (semAddress); // inkrementacja semafora o 1 
    if ( err == -1 ) { perror ("Error: sem_post"); return 1; }

    // Odczytanie wartosci i wypisanie:
    err = sem_getvalue (semAddress, &currentValue);
    if ( err == -1 ) { perror ("Error: sem_getvalue"); return 1; }
    printf ("Current value of semaphore is: %d\n", currentValue);
    // Odczytanie wartosci i wypisanie

    err = sem_wait (semAddress); // dekrementacja semafora o 1, operacja moze byc blokujaca! 
    if ( err == -1 ) { perror ("Error: sem_wait"); return 1; }

    // Odczytanie wartosci i wypisanie:
    err = sem_getvalue (semAddress, &currentValue);
    if ( err == -1 ) { perror ("Error: sem_getvalue"); return 1; }
    printf ("Current value of semaphore is: %d\n", currentValue);
    // Odczytanie wartosci i wypisanie

    err = sem_close (semAddress);
    if ( err == -1) { perror ("Error: sem_close"); return 1; }

    err = sem_unlink ("/semaphore");
    if ( err == -1) { perror ("Error: sem_unlink"); return 1; }

    return 0;
}

