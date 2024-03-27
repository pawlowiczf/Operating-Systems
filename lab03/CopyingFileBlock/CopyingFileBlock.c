#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    char block[1024];
    int we, wy;
    int value;

    we = open("novel", O_RDONLY );
    wy = open("copied-novel", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );

    // Funkcja read() zwraca -1, gdy blad, 0 - gdy koniec pliku, 1 - poprawne wykonanie 
    // Jej argumentami sa: deskryptor wejscia, bufor do zapisu (wskaznik), liczba bajtow do odczytania 

    while( ( value = read( we, block, sizeof(block) ) ) > 0 )
        write(wy, block, value);
    //

    close(wy);
    close(we);
}

