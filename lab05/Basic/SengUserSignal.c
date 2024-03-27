#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


void handle_sigusr1 (int sig) {
    printf ("(HINT) Remember that multiplication is repetetive addition! \n");
}

int main (int argc, char* argv[]) {
    //
    pid_t pid = fork ();
    if (pid == -1) { perror("Fork error"); return -1; }

    if (pid == 0) { 
        sleep (5);
        kill ( getppid(), SIGUSR1 );


    } else {
        struct sigaction sa = {0};
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = &handle_sigusr1;
        sigaction (SIGUSR1, &sa, NULL);

        int x;
        printf ("What is the result of 3 x 5: ");
        scanf ("%d", &x);

        if (x == 15) { printf ("Right \n"); }
        else { printf ("Wrong \n"); }

        wait (NULL);
    }




    return 0;
}