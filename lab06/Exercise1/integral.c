#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h> 

#define RIGHT_INTERVAL 1.0

// Calculates function value.
double f (double x) {
    return 4.0 / (x * x + 1.0);
}

// Calculates area of a rectangle. 
double integrate (double left, double right, double widthRectangle) {
    //
    double partialSum = 0;

    while (left < right && left <= RIGHT_INTERVAL) {
        partialSum += f(left) * widthRectangle; 
        left += widthRectangle; 
    }

    return partialSum;
}

int main (int argc, char* argv[]) {
    //
    if (argc < 2) { printf("Not enough arguments \n"); return 1; }

    double widthRectangle = atof ( argv[1] );
    int numberIntervals = atoi ( argv[2] );  
    double widthInterval = 1.0 / numberIntervals;

    pid_t pid;

    if ( widthRectangle > widthInterval ) {
        printf ("Width of rectangle > width of interval\n");
        return 1; 
    }

    clock_t startTime = clock();

    int fd[numberIntervals][2]; 
    double sum = 0;
    double left = 0; 
    int error; 

    for (int a = 0; a < numberIntervals; a++) {
        //
        pipe (fd[a]);
        pid = fork ();

        if ( pid == 0 ) {
            close (fd[a][0]);
            
            double partialSum = integrate (a * widthInterval, (a + 1) * widthInterval, widthRectangle);
            error = write ( fd[a][1], &partialSum, sizeof(double) );

            if (error < 0) { printf("Error - write"); return 1; }
            close (fd[a][1]);
        
            exit (0);

        } else {
            close (fd[a][1]);
        }

    } // end 'for' loop 

    while ( wait(NULL) > 0 ) {}

    double partialSum = 0; 

    for (int a = 0; a < numberIntervals; a++) {
        //
        error = read ( fd[a][0], &partialSum, sizeof(double));
        sum += partialSum;

        if (error < 0) { printf("Error - read"); return 1; }
        close (fd[a][0]);
    }

    clock_t endTime = clock();

    char buffer[500];
    sprintf(buffer, "Number of processes: %d. Time: %f\n", numberIntervals, ((double) (endTime - startTime)) / CLOCKS_PER_SEC );

    int fdFile = open ("time-report.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if ( fdFile < 0 ) { return 1; }
    if ( write ( fdFile, buffer, strlen(buffer) ) < 0 ) { return 1; }

    printf ("The integrate is equal to: %f\n", sum);

    return 0; 
}