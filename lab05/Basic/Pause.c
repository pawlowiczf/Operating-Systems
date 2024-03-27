#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void handler (int sig) {
    printf ("I received signal %d\n", sig);
}

int main () {
    //
    signal (SIGINT, &handler);

    printf ("I am waiting for a signal \n"); 
    
    pause ();

    printf ("I continue program \n");

    return 0;
}