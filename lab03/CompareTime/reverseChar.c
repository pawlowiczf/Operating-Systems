#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

void reverse() {
    int in, out;
    off_t offset = -1;
    char c;

    in  = open("novel", O_RDONLY);
    out = open("novel-reversed", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    
    while ( ( lseek(in, offset, SEEK_END) != -1 ) && ( read(in, &c, 1) == 1 ) ) {
        write(out, &c, 1);
        offset--; 
    }

    close(in);
    close(out);
}

int main() {
    //
    clock_t start, end; 
    
    start = clock(); 
    reverse();
    end = clock();
    printf("%f sekund \n", ( (double) (end - start) ) / CLOCKS_PER_SEC);

}