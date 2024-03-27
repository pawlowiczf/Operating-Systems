#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int global = 0; 

int main ( int argc, char* argv[] ) {
    //
    int local = 0, status; 

    pid_t child_id; 
    child_id = fork ();

    if ( child_id == 0 ) {
        printf ("child process \n");
        global++; local++;
        printf ("child pid = %d, parent pid = %d \n", getpid(), getppid() );
        printf ("child's local = %d, child's global = %d \n", local, global);
        int procesStatus = execl ( "/bin/ls", "ls", argv[1], NULL );

        if (procesStatus == -1) {
            fprintf(stderr, "Proces-dziecko zakonczony niepowodzeniem");
            return -1; 
        }

    } else {
        printf ("parent process \n");
        printf ("parent pid = %d, child pid = %d \n", getppid(), child_id);

        child_id = wait(&status);
        printf ( "Child exit code: %d \n", WEXITSTATUS(status) );
        printf ( "Parent's local = %d, parent's global = %d \n", local, global);
    }

    return 0;
}   