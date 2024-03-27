#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>


int main (int argc, char* argv[]) {
    pid_t pid = fork (); 

    if (pid == -1) { perror("Error in fork"); return -1; }

    if (pid == 0) {
        while (1) {
            printf ("Some text goes here \n");
            usleep (50000); // waits 0.25 second
        }
    } else {
        sleep (1); // waits 1 second
        kill (pid, SIGKILL);
        wait (NULL);
    }


    return 0; 
}