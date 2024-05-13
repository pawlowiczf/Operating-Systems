#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>


int main (int argc, char* argv[]) {
    //
    if (argc != 3) { printf("Error - provide two arguments [a,b]\n"); return 1; }
    int a, b, err;
    double integralValue;
    a = atoi( argv[1] ); b = atoi( argv[2] );

    if ( mkfifo ("integralParameters", 0777) < 0 ) {
        if ( errno != EEXIST ) { printf ("Error - mkfifo"); return 1; } 
    }
    if ( mkfifo ("receiveValue", 0777) < 0 ) {
        if ( errno != EEXIST ) { printf ("Error - mkfifo"); return 1; } 
    }

    int fd = open ("integralParameters", O_WRONLY);
    if (fd < 0) { printf("Error - open"); return 1; }

    err = write ( fd, &a, sizeof(int) );
    err = write ( fd, &b, sizeof(int) );
    if (err < 0) { printf("Error - write"); return 1;}
    close(fd);
    
    fd = open ("receiveValue", O_RDONLY);
    if (fd < 0) { printf("Error - open"); return 1; }

    err = read ( fd, &integralValue, sizeof(double) );
    if (err < 0) { printf("Error - write"); return 1; }
    close (fd);

    printf ("Integral value: %f\n", integralValue);

    return 0;
}