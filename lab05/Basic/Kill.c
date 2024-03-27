#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
    //
    pid_t pid; 
    pid = fork ();

    if (pid == 0) { 
        while (1) { printf ("Nie zatrzymasz mnie \n"); }
    } else {
        sleep (2);
        kill (pid, SIGKILL);
    }


    return 0; 
}