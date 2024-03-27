#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void writeBlockToFileReverse(int descriptor, char* block, int length) {
    //
    char c;

    for (int a = length - 1; a >= 0; a--) {
        c = block[a];
        write(descriptor, &c, 1);
    }
}

// Jak znalezc dlugosc pliku (lseek zwraca nowa pozycje w pliku)
int main() {
    //
    int in, out;
    long length;
    char block[1024];

    off_t offset = -sizeof(block);

    in  = open("novel", O_RDONLY);
    out = open("novel-reversed", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    // Lseek zwraca -1, gdy blad lub nowa pozycje w pliku 
    lseek(in, offset, SEEK_END);

    while ( ( length = read(in, block, sizeof(block)) ) > 0 )  {
        //
        writeBlockToFileReverse(out, block, length);
        offset = offset - length;
        lseek(in, offset, SEEK_END);
    }
    
    close(in);
    close(out);

}