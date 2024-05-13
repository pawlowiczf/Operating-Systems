#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXT 10
#define MAX_PRINTER 20

typedef enum {
    WAITING = 0,
    PRINTING = 1, 
} PrinterState; 

typedef struct {
    //
    char buffer[MAX_TEXT]; 
    PrinterState state; 
} printer_t;

typedef struct {
    //
    int semID; 
    printer_t printersArray[ MAX_PRINTER ];
    int numberOfPrinters;
} printers_t; 
