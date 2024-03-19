#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
    pid_t child_pid;
    child_pid = fork ();

    if (child_pid > 0) {
        // Rodzic
        sleep (60);
    } else {
        // Dziecko
        exit (0);
    }

    return 0;
}