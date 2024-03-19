#include <unistd.h>
#include <sys/types.h> 
#include <stdio.h>

int main() {
    // Zmienne typu: pid_t, uid_t, gid_t

    printf("PID: %d \n", (int) getpid() );
    printf("PPID: %d \n", (int) getppid() );

    printf("UID user: %d \n", (int) getuid() );
    printf("UID effective: %d \n", (int) geteuid() );

    printf("GID: %d \n", (int) getgid() );
}