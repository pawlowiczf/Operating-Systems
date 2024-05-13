#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
    //
    int fd[2]; // fd[0] - read end, fd[1] - write end
    if ( pipe(fd) == -1 ) { fprintf(stdout, "Error - opening pipe"); return 1; }

    pid_t id = fork (); 
    if (id == -1) { fpritnf(stdout, "Error - fork"); return 1; }
    
    if ( id == 0 ) { // child process 
        close(fd[0]);

        int x; 
        printf ("Input a number: ");
        scanf ("%d", &x);
        if ( write ( fd[1], &x, sizeof(int) ) == -1 ) { fprintf(stdout, "Error - writet"); return 1; }

        close (fd[1]);
    } else {
        close(fd[1]);

        int y; 
        if ( read (fd[0], &y, sizeof(int)) == -1 ) { fprintf(stdout, "Error - read"); return 1; }

        close(fd[0]);
        printf ("Got from child process %d\n", y);
    }

    return 0;
}