#include <stdlib.h>
#include "queue.h"
#include <stdio.h>
#include <unistd.h>

node_t* head = NULL; // pierwszy element kolejki
node_t* tail = NULL; // ostatni element kolejki 

// Wstawia element na koniec kolejki
void enQueue (int* clientSocket) {
    //
    node_t* newNode = malloc ( sizeof(node_t) ); 
    if (newNode == NULL) { perror ("Error - malloc()"); exit (1); }
    
    newNode->clientSocket = clientSocket; 
    newNode->next = NULL; 

    if ( tail != NULL ) {
        tail->next = newNode; 
        tail = newNode; 
    } else {
        head = newNode;
        tail = newNode; 
    }
}

// Wyciaga pierwszy element z kolejki 
int* deQueue () {
    //
    if ( head == NULL ) { return NULL; }

    int* clientSocket = head->clientSocket; 

    node_t* tempHead = head; 
    head = head->next;

    if (head == NULL) { tail = NULL; } 
    free (tempHead); 

    return clientSocket; 
}
