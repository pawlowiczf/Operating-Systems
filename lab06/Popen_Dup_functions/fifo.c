#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
    //
    if ( mkfifo ("myfifo1", 0777) < 0 ) {
        if ( errno != EEXIST ) { printf ("Error - mkfifo"); return 1; } 
    }

    int fd = open ("myfifo1", O_WRONLY);
    int x = 97;

    if ( write (fd, &x, sizeof(x)) == -1 ) { return 2; };
    close (fd);

    return 0;
}