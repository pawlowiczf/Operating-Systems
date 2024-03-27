#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    char c;
    int we,wy;

    we = open("novel", O_RDONLY);
    wy = open("copied-novel", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );

    while ( read(we,&c,1) == 1 ) { write(wy,&c,1); }

    close(wy);
    close(we);
} 