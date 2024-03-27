#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handle_signal (int sig) {
    printf ("I handled signal %d", sig);
}

int main (int argc, char* argv[]) {
    //
    signal (SIGUSR1, &handle_signal);
    
    while (1) {
        printf (". \n");
    }

    return 0;
}