#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* 
Rodzic przekazuje komunikat do dziecka, dziecko modyfikuje go delikatnie i odsyla do rodzica. Rodzic drukuje otrzymana wiadomosc na stdout. 
*/

int main () {
    //
    int fd_R_D[2]; 
    int fd_D_R[2];
    pipe (fd_R_D);
    pipe (fd_D_R);  

    pid_t id = fork (); 
    if (id < 0) { fprintf(stdout, "Error - fork"); return -1; } 

    if (id == 0) {
        close (fd_R_D[1]);
        close (fd_D_R[0]);

        char message[500]; 

        read ( fd_R_D[0], message, 500);
        printf("%s \n", message);
        message[0] = 'A';
        write (fd_D_R[1], message, sizeof(message));

        close (fd_R_D[0]);
        close (fd_R_D[1]);
        
    } else {
        close (fd_R_D[0]);
        close (fd_D_R[1]);
         
        char message[] = "Hello world!"; 
        write ( fd_R_D[1], message, sizeof(message) );
        read (fd_D_R[0], message, sizeof(message));

        printf("%s", message);

        close (fd_R_D[1]);
        close (fd_R_D[0]);
    }

}