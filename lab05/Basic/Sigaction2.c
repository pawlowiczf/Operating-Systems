#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void handler (int sig) {
    printf ("I handled signal \n");
}

int main () {
    struct sigaction act;
    act.sa_handler = &handler;
    act.sa_flags = SA_RESTART;

    sigemptyset (&act.sa_mask);
    sigaddset (&act.sa_mask, SIGINT);
    sigaction (SIGUSR1, &act, NULL);

    while (1) {
        printf ("Hello \n");
        sleep (3);
    }

    return 0;
}