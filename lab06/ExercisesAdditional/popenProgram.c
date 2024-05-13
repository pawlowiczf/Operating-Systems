#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
Napisz program bedacy odpowiednikiem polecenia: ps aux | wc -l 
z wykorzystaniem funkcji popen, pclose 
*/

int main () {
    //
    char buffer[256];
    int counter = 0; 

    FILE* wcOutput = popen ("ps aux", "r");

    if ( wcOutput == NULL ) { return -1; }

    while ( fgets(buffer, sizeof(buffer), wcOutput) != NULL ) {
        counter++;
    }

    printf ("%d\n", counter);
    pclose(wcOutput);

    return 0; 
}