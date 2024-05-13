#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Kod do kompilacji: gcc -lpthread ...

/*
1) Tworzenie watkow: int pthread_create

2) Pobieranie wlasnego identyfikatora watku: pthread_t pthread_self(); 

3) Porownywanie identyfikatorow watkow: int pthread_equal(pthread_t a, pthread_t b) - zwraca 0, gdy ID nie sa sobie rowne. 

4) Przerwanie watku z innego watku mozna wykonac przy uzyciu funkcji: int pthread_cancel(pthread_t tid);

5) Watek moze blokowac bycie cancelowanym przy uzyciu odpowiednich funkcji. 
*/

void* exampleFunction (void* data) {
    //
    int* value = (int*) data;
    printf ("The value is: %d\n", *value);

    pthread_exit (NULL);
}

int main () {
    //
    int err, value = 5; 

    pthread_t threadID; 

    err = pthread_create (&threadID, NULL, exampleFunction, &value);
    if ( err < 0 ) { perror ("Error: pthread_create"); return 1; }

    err = pthread_join (threadID, NULL);
    if ( err < 0 ) { perror ("Error: pthread_join"); return 1; }

    puts ("Koniec dzialania watku.");

    return 0;
}