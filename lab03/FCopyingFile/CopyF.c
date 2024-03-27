#include <stdio.h>

void copyFileByOneChar() {
    //
    FILE *in  = fopen("novel", "r");
    FILE *out = fopen("novel-reversed", "w");
    if ( in == NULL || out == NULL ) { perror("Blad podczas otwierania plikow") ; return; }

    char c; 
    int length;

    // Funkcja fread zwraca liczbe elementow pomyslnie odczytanych. W przeciwnym razie zwraca pewien blad. 

    while ( (length = fread(&c, 1, 1, in)) > 0 ) {
        //
        fwrite(&c, 1, length, out);
    }

    flose(in);
    flose(out);
}

int main() {
    //
    copyFileByOneChar();

    return 0;
}

