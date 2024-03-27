#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Ignorowanie sygnalu SIGINT uzywajac funkcji sigaction:
// Sigaction zwraca -1, gdy jest blad.

int main () {
    //
    struct sigaction sa; 
    sa.sa_handler = SIG_IGN; // lub tez SIG_DFL
    sa.sa_flags = SA_RESTART; 
    sigaction (SIGINT, &sa, NULL);

    while (1) { printf ("I'm waiting \n"); sleep (1); }
}