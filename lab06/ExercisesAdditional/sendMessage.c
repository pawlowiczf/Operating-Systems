#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* 
Napisz program, w ktorym stworzysz proces potomny, a nastepnie przeslesz od rodzica do potomka komunikat typu hello world z wykorzystaniem potoku nienazwanego. 
*/

int main () {
    //
    int fd[2]; 
    pipe (fd); 

    pid_t id = fork (); 
    if (id < 0) { fprintf(stdout, "Error - fork"); return -1; } 

    if (id == 0) {
        close (fd[1]);
        char message[5000]; 

        read ( fd[0], message, 5000);
        printf("%s \n", message);

        close(fd[0]);
        
    } else {
        close (fd[0]);
         
        char message[] = "Hello world!"; 
        write ( fd[1], message, sizeof(message) );

        close (fd[1]);
    }
    
    
}
