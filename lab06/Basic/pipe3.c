#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main (int argc, char* argv[]) {
    int arr[] = { 1, 2, 3, 4, 1, 2 }; 
    int arrSize = sizeof (arr) / sizeof (int);
    int start, end;

    int fd[2]; 

    if ( pipe (fd) == -1 ) { printf ("Error - pipe"); return 1; }

    pid_t id = fork (); 
    if ( id == -1 ) { printf ("Error - fork"); return 1; }

    if ( id == 0) {
        start = 0; 
        end = arrSize / 2;
    } else {
        start = arrSize / 2; 
        end = arrSize; 
    }

    int sum = 0; 
    for (int a = start; a < end; a++) { sum += arr[a]; }
    printf ("Process: %d, calculated partial sum: %d\n", id, sum); 

    if (id == 0) {
        close (fd[0]);
        write (fd[1], &sum, sizeof(sum)); 
        close (fd[1]);
    } else {
        int sumFromChild; 
        close (fd[1]); 
        read (fd[0], &sumFromChild, sizeof(sumFromChild));
        close (fd[0]);

        int totalSum = sum + sumFromChild; 
        printf ("Total sum is %d\n", totalSum);
        wait (NULL); 
    }

    return 0;
}