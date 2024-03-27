#include <stdio.h>
#include <dirent.h>

#include <sys/stat.h>

int printDirectory() {
    DIR *directory; 
    struct dirent *entry; // entry -> d_type == DT_REG (regular file) albo DT_DIR (katalog)

    directory = opendir("."); // . means current working directory 
    if (directory == NULL) { perror("Blad otwarcia biezacego katalogu"); return 1; }

    while ( (entry = readdir(directory)) != NULL ) {
        printf("%s \n", entry -> d_name);
    }
    

    if ( closedir(directory) == -1 ) { perror("Blad zamkniecia biezacego katalogu"); return 1; }
    return 0;
}

// void rewinddir(DIR* directory) - ustawia strumien katalogowy na poczatek 
// int stat(const char *fileName, struct stat *buf) - pobranie statusu pliku 

int listFiles() {
    //
    DIR *directory;
    struct dirent *entry; 
    struct stat fileStats;
    long long filesSize = 0;

    directory = opendir(".");
    if (directory == NULL) { perror("Blad otwarcia biezacego katalogu"); return 1; }

    while ( (entry = readdir(directory)) != NULL ) {
        //
        if ( stat( entry -> d_name, &fileStats ) == -1 ) { perror("Blad statystyk"); return 1; }
        if ( S_ISDIR( fileStats.st_mode) ) { continue; }
        printf("%s %ld \n", entry -> d_name, fileStats.st_size);

        filesSize += fileStats.st_size;
    }   


    if ( closedir(directory) == -1 ) { perror("Blad zamkniecia biezacego katalogu"); return 1; }

    printf("Size of all files: %lld \n", filesSize);
    return 0;
}

int main() {
    //

    // printDirectory();
    listFiles();
}