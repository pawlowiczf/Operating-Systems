#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int main () {
    //
    sigset_t signalSet;
    sigemptyset (&signalSet); // zwraca: 0 - poprawne wykonanie, -1 - niepoprawne wykonanie 

    sigaddset (&signalSet, SIGINT); // dodanie pojedycznego sygnalu 
    sigaddset (&signalSet, SIGSTOP);

    sigdelset (&signalSet, SIGSTOP); // usuniecie pojedycznego sygnalu 

    printf ("%d \n", sigismember (&signalSet, SIGINT) ); // sprawdzenie, czy sygnal jest w zbiorze 
    printf ("%d \n", sigismember (&signalSet, SIGSTOP) );

    sigfillset (&signalSet); // dodanie wszystkich sygnalow do zbioru 

    return 0;
}