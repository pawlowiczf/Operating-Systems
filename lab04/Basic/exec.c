#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
    Spawn a child process running a new program. PROGRAM is the name of the program to run; the path will be searched for this program. ARG_LIST is a NULL-terminated list of character strings to be passed as the program's argument list. Returns the process ID of the spawned process. 
*/

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
    // The argument list to pass to the "ls" command:
    char* arg_list[] = {
        "ls",  // argv[0] <- the name of the program, nie takie wazne 
        "-l", 
        "/",
        NULL // the argument list must end with a NULL 
    };

    // Spawn a child process running the "ls" command. Ignore the returned child process ID.
    spawn ("ls", arg_list);

    printf ("done with the main program \n");
    return 0;
}