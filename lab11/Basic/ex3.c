#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // do protokolow TCP itd. 

#include <limits.h> 
#include <unistd.h> 
#include <endian.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    //
    int err; 

    // 1) Tworzenie gniazda:

    int fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); // zwraca deskryptor
    if ( fd == -1 ) { perror ("Error: socket"); return EXIT_FAILURE; }

    // 2) Bindowanie gniazda do nazwy:

    struct sockaddr_in sockAddr;  // te prefiksy sin to skrot od 'socket internet'
    sockAddr.sin_family = AF_INET; 
    sockAddr.sin_port   = htons (8080); // htons zamienia port na notacje big-endian (sieciowa kolejnosc bajtow)
    sockAddr.sin_addr.s_addr = INADDR_ANY; // dowolny, dostepny adres 

    // Dla jasnosci, definicja struktury sockaddr6_in dla IPv6:
    struct sockaddr_in6 sockAddr6; 
    sockAddr6.sin6_family = AF_INET6; 
    sockAddr6.sin6_port   = htons (8080);
    sockAddr6.sin6_addr = in6addr_any; // dowolny, dostepny adres 

    err = bind (fd, (struct sockaddr*) &sockAddr, sizeof(sockAddr)); 
    if (err == -1) { perror ("Error: bind"); return 1; }

    close (fd); 
}