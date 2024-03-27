#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int actionChange = 0;

void handler (int sig, siginfo_t *info, void *context) {
    int value = info -> si_value.sival_int; 

    if ( value == 1 ) {
        for (int a = 0; a <= 100; a++) { printf ("%d ", a); }
        printf ("\n");
    } 
    else if ( value == 2) {
        printf ("%d\n", actionChange);
    }
    else {
        kill ( info -> si_pid, SIGUSR1 );
        exit (1);
    }

    fflush(stdout);
    actionChange++;

}

int main () {
    //
    printf ("%d \n", getpid() );
    fflush(stdout);

    struct sigaction sa; 
    memset ( &sa, 0, sizeof (sa) );

    sa.sa_sigaction = &handler;
    sigaddset (&sa.sa_mask, SIGUSR1);

    sa.sa_flags = SA_SIGINFO; // dodawanie takiej flagi powoduje, ze funkcja handler staje sie bardziej rozbudowana

    sigaction (SIGUSR1, &sa, NULL);

    while (1) {
        continue;
    }
}