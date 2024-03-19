#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn(char* program, char** arg_list) {
    pid_t child_pid; 

    // Duplicate this process:
    child_pid = fork ();

    if (child_pid != 0) {
        // parent process 
        return child_pid;
    } else {
        // execute different program 
        execvp (program, arg_list);
        // The execvp functions returns only if an error occurs
        fprintf (stderr, "an error occured in execvp \n");
        abort ();
    }
}

int main() {
    int child_status;

    // The argument list to pass to the "ls" command:
    char* arg_list[] = {
        "ls",  // argv[0] <- the name of the program, nie takie wazne 
        "-l", 
        "/",
        NULL // the argument list must end with a NULL 
    };

    // Spawn a child process running the "ls" command. Ignore the returned child process ID.
    spawn ("ls", arg_list);

    // Wait for the child process to complete (wait for any child, not particular)
    wait (&child_status);

    if ( WIFEXITED (child_status) ) {
        printf ("the child process exited normallny, with the exit code %d \n", WEXITSTATUS(child_status));
    } else {
        printf ("the child process exited abnormally \n");
    }


    printf ("done with the main program \n");   
    return 0;
}