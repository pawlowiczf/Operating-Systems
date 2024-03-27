#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handler (int sig) {
    printf ("I received a signal %d \n", sig);
}

int main (int argc, char* argv[]) {
    //
    char* arg = argv[1]; 
    
    if ( strcmp(arg, "none") == 0 ) {
        signal (SIGUSR1, SIG_DFL);
    } 
    else if ( strcmp(arg, "ignore") == 0 ) {
        signal (SIGUSR1, SIG_IGN);
    }
    else if ( strcmp(arg, "handler") == 0 ) {
        signal (SIGUSR1, &handler);
    } 
    else if ( strcmp(arg, "mask" ) == 0 ) {
        sigset_t set;
        sigemptyset (&set);
        sigaddset (&set, SIGUSR1);

        int status = sigprocmask ( SIG_SETMASK, &set, NULL );
        if (status < 0) { perror("Mask error"); return -1; }
    }

    raise (SIGUSR1);

    if ( strcmp(arg, "mask") == 0 ) {
        sigset_t currentBlockedSet; 
        int status = sigpending (&currentBlockedSet); // aktualnie maskowany sygnaly

        if (status < 0) { perror("Sigpending error"); return -1; }
        printf ("SIGUSR1 in pending state: %d \n", sigismember(&currentBlockedSet, SIGUSR1) );
    }

    printf ("End of the program \n");
    return 0;
}