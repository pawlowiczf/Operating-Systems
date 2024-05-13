#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Kod do kompilacji: gcc -rt semPOSIX.c -o semPOSIX 
// Nalezy zlinkowac biblioteke rt 

int main () {
    //
    int err; 

    int shmID = shm_open ("/shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); 
    if ( shmID < 0 ) { perror ("Error: shm_open"); return 1; }

    // Okreslenie rozmiaru segmentu:
    err = ftruncate (shmID, 512);
    if ( err < 0 ) { perror ("Error: ftruncate"); return 1; }

    // Dolaczanie segmentu pamieci do przestrzeni adresowej:
    void* shmAddress = mmap (NULL, 512, PROT_READ, MAP_SHARED, shmID, 0); // memory map
    if ( shmAddress == (void*) -1 ) { perror ("Error: mmap"); return 1; }

    err = munmap (shmAddress, 512);
    if ( err < 0 ) { perror ("Error: munmap"); return 1; }

    err = shm_unlink ("/shm");
    if ( err < 0 ) { perror ("Error: shm_unlink"); return 1; }

    return 0;
}