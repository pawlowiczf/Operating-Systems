#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main( int argc, char* argv[] ) {
    //
    const int numberProcesses = atoi ( argv[1] );
    pid_t child_pid;

    for (int a = 0; a < numberProcesses; a++) {
        child_pid = fork (); 

        if ( child_pid == 0 ) {
            printf("Child process: %d \nParent process: %d \n-------------- \n", (int) getpid(), (int) getppid()  );
            exit(0);
        }
    }

    int status; 
    for (int a = 0; a < numberProcesses; a++) {
        child_pid = wait(&status); 

        if ( WIFEXITED(status) ) {
            printf("Proces %d zakonczl sie poprawnie. Status: %d \n", child_pid, WEXITSTATUS(status) );

        } else {
            printf("Proces %d zakonczyl sie niepowodzeniem. Status: %d \n", child_pid, WEXITSTATUS(status));
        
        }

        fflush(stdout);
    }

    printf("Liczba procesow: %d \n", numberProcesses);  
    
    return 0;

    // Funkcja wait() zwraca ID procesu oraz jako argument przyjmuje wskaznik na inta, w ktorym zapisuje status wyjscia.
    // WIFEXITED - sprawdza, czy proces zakonczyl sie poprawnie, WEXITSTATUS - zwraca status wyjscia. 
}
