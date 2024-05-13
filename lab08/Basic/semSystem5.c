#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY 1234

/*
Semafory systemu V:

1) Mozna powiekszac lub pomniejszac o wartosci wieksze niz 1, ale po kazdej operacji wartosc semafora musi byc >= 0. W przeciwnym razie watek jest blokowany, do momentu, gdy wartosc semafora pozwoli na operacje. 

2) Jesli chcemy pobrac deskryptor istniejacego juz zbioru semaforow, to za argument liczby semaforow dajemy 0 

3) Flagi do operacji sem_flg to: IPC_NOWAIT - operacja nie powinna blokowac procesu - jesli tak sie dzieje to jest zwracany blad, SEM_UNDO - w przypadku zakonczenia procesu operacja wykonana na semaforze powinna zostac cofnieta. 

4) Mozliwe komendy w funkcji semctl to SETVAL - ustawienie wartosci semafora, GETVAL - pobranie wartosci semafora, IPC_RMID - usuniecie zbioru semaforow z systemu.

5) Semafory indeksujemy od 0 w zbiorze semaforow!
*/

union semun {
    int val; 
};

int main () {
    //
    int err; 

    int semID = semget (KEY, 1, 0666 | IPC_CREAT); // 1 - liczba semaforow do utworzenia. 
    if ( semID < 0 ) { perror ("Error - semget"); return 1; }

    union semun arg;
    arg.val = 5; 

    err = semctl (semID, 0, SETVAL, arg); // ustawienie wartosci zerowego semafora na 5 
    if (err < 0 ) { perror("Error - semctl"); return 1; }

    printf("Current value of semaphore number 0 is: %d\n", semctl(semID, 0, GETVAL, 0));

    struct sembuf semOp; // operacja do wykonania na semaforze 
    semOp.sem_op = 1; // jesli sem_op = 0 to blokujemy, do momentu, gdy wartosc semafora bedzie wynosic 0
    semOp.sem_flg = SEM_UNDO; // jesli proces sie zakonczy, to zmiany w semaforze zostana cofniete. 
    semOp.sem_num = 0; // numer semaforu w zbiorze semaforow (zbior indeksujemy od 0)

    struct sembuf semOps[] = {semOp}; // tablica struktur - czyli operacji do wykonania

    err = semop (semID, semOps, 1); // 1 oznacza liczbe elementow tablicy semOps (liczba operacji do wykonania)
    if ( err < 0 ) { perror ("Error - semop"); return 1; }

    printf("Current value of semaphore number 0 is: %d\n", semctl(semID, 0, GETVAL, 0));

    err = semctl (semID, 0, IPC_RMID, 0);
    if (err < 0) { perror ("Error - semctl IPC_RMID"); return 1; }
    return 0;
}