#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main () {
    //
    pid_t pid; 

    int shmID = shm_open ("/posix", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
    if ( shmID < 0 ) { perror ("Error: shm_open"); return 1; }

    pid = fork (); 
    if ( pid < 0 ) { perror ("Error: fork()"); return 1; }

    if ( pid == 0 ) {
        //
        sleep (1);
        char* buffer = mmap (NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
        printf ("%s\n", buffer);

        munmap (buffer, 512);

    } else {
        //
        int err = ftruncate (shmID, 512);
        if ( err < 0 ) { perror ("Error: truncate()"); return 1; }

        char* buffer = mmap (NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
        strcpy (buffer, "Jestem tutaj!");

        wait (NULL);
        munmap (buffer, 512);
    }

    shm_unlink ("/posix");
    return 0;
}