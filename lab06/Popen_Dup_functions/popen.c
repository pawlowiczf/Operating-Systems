#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main () {
    //
    // FILE* grep_input = popen ("ls", "r");   
    // char buffer[500];

    // while ( fgets (buffer, sizeof(buffer), grep_input) != NULL ) {
    //     printf ("%s", buffer);
    // }

    // pclose (grep_input);

    int fd[2];
    pipe (fd);

    pid_t pid; 
    pid = fork (); 

    if (pid == 0) {
        // close (fd[0]);
        dup2 (fd[1], STDOUT_FILENO);
        // close (fd[1]);

        execlp("ls", "ls", "-l", NULL);
        return 2;

    } else {
        sleep (1);
        close (fd[1]);

        char buffer[500];
        int bytes_read = 0; 
        waitpid (pid, NULL, 0);

        while ( ( bytes_read = read(fd[0], buffer, sizeof(buffer) - 1) ) > 0 ) {
            buffer[bytes_read] = '\0';
            printf ("Data: ");
            printf ("%s", buffer);
        }
        close (fd[0]);
    }

    return 0; 
}