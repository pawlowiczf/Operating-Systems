#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h> 
#include <arpa/inet.h> 
#include <limits.h> // do funkcji gethostname i stalej HOST_NAME_MAX
#include <unistd.h> // do funkcji gethostname()

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    // 
    const char hostName[] = "onet.pl"; 

    struct hostent *hostInfo = gethostbyname (hostName);
    if ( hostInfo == NULL ) { perror ("Error: gethostbyname"); return EXIT_FAILURE; }

    printf ("Official name: %s\n", hostInfo->h_name);
    
    char** ipAddr; // wskaznik do stringa (tj. wskaznik do tablicy znakow). Tak naprawde jest to lista wskaznikow na struktury adresow IP o rozmiarze hostInfo->h_length. Nalezy dokonac odpowiedniego rzutowania. Ta tablica konczy sie NULL'em.

    puts ("Pierwszy sposob printowania adresow IP");
    for ( ipAddr = hostInfo->h_addr_list; *ipAddr != NULL; ipAddr++ ) {
        printf ( "- %s\n", inet_ntoa( *(struct in_addr*) *ipAddr ) );
    }

    puts ("Drugi sposob printowania adresow IP");
    for ( int a = 0; hostInfo->h_addr_list[a] != NULL; a++ ) {
        printf ("- %s\n", inet_ntoa ( *(struct in_addr*) hostInfo->h_addr_list[a] ) );
    }

    // Zwraca nazwe lokalnego komputera. 
    char computerName[ HOST_NAME_MAX + 1 ]; // musi byc +1, bo znak zakonczenia stringa '\0'
    int err = gethostname (computerName, HOST_NAME_MAX);
    if ( err < 0 ) { perror ("Error - gethostname()"); return EXIT_FAILURE; }
    puts (computerName);

    return 0;
}
