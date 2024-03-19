#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

// Listuje proces oraz jego parenta.

int main() {
    //
    printf ("The process ID is %d \n", (int) getpid() );
    printf ("The parent process ID is %d \n", (int) getppid() );

    return 0;
}