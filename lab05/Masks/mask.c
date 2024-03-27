#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main () {
    //
    int err;
    sigset_t newMask, oldMask; 

    sigemptyset (&newMask);
    sigaddset (&newMask, SIGINT); 

    err = sigprocmask (SIG_BLOCK, &newMask, &oldMask); // stara maska jest zapisywana do oldMask
    if (err == -1) { perror("Sigprocmask error"); return -1; }

    sleep (3); // w tym czasie nie bedziemy mogli zakonczyc Ctrl + C 

    err = sigprocmask (SIG_SETMASK, &oldMask, NULL); 
    if (err == -1) { perror("Sigprocmask set error"); return -1; }
    sleep (15);
}

// sigprocmask: SIG_BLOCK (blokuj stare i nowe maski), SIG_UNBLOCK (odblokuj podane sygnaly), SIG_SETMASK (ustaw nowa maske, nadpisz stara)

// kiedy sygnal zostanie odblokowany, to jest wykonywany zgodnie (kolejka sygnalow)