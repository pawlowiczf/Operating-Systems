#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/stat.h>

// Calculates function value.
double f (double x) {
    return 4.0 / (x * x + 1);
}

// Calculates area of a rectangle. 
double integrate (double left, double right, double widthRectangle) {
    //
    double partialSum = 0;

    while (left < right) {
        partialSum += f(left) * widthRectangle; 
        left += widthRectangle; 
    }

    return partialSum;
}

#define WIDTH_RECTANGLE 0.001
int main() {
    //
    int fd = open ("integralParameters", O_RDONLY);
    if ( fd < 0 ) { printf("Error - open"); return 1; }

    int a, b, err;
    err = read (fd, &a, sizeof(int));
    err = read (fd, &b, sizeof(int));
    if ( err < 0 ) { printf("Error - read"); return 1; }
    close (fd);

    fd = open ("receiveValue", O_WRONLY);
    if ( fd < 0 ) { printf("Error - open"); return 1; }

    double integralValue = integrate (a, b, WIDTH_RECTANGLE);
    err = write ( fd, &integralValue, sizeof(double) );
    if ( err < 0 ) { printf("Error - write"); return 1; }
    close (fd);

    return 0; 
}