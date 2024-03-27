#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void handler (int sig) {
    printf ("Received SIGUSR1 confirmation back \n");
}

int main (int argc, char* argv[]) {
    //
    pid_t pid = atoi (argv[1]);
    int workType = atoi (argv[2]);

    union sigval value; 
    value.sival_int = workType; 

    sigqueue (pid, SIGUSR1, value);

    sigset_t signalSet; 
    sigemptyset (&signalSet);
    sigfillset (&signalSet);
    sigdelset (&signalSet, SIGUSR1); // proces sie odblokuje jesli dostanie sygnal, ktorego nie ma w masce    

    signal (SIGUSR1, &handler);
    sigsuspend (&signalSet);


}