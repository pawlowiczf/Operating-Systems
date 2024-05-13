#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main () {
    //
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) { 
        close (fd[1]);
        dup2 (fd[0], STDIN_FILENO);
        execlp ("grep", "grep", "Ala", NULL);

    } else { 
        close ( fd[0] );

        char message[] = "Ala ma kota, ale Ala nie ma psa. Ala.";
        write( fd[1], &message, sizeof(message) ); 

        close (fd[1]);
        wait (NULL);
    }
}

