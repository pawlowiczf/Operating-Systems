#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int err; 
void checkErr(char*, int);

// UWAGA. Wyjasnienie nazw tych funkcji znajduje sie na dole. 

int main () {
    //
    struct in_addr inp = {0}; // struktura przechowujaca adres IPv4
    struct in6_addr inp6 = {0}; // struktura przechowujaca adres IPv6

    // 1. Konwersja tekstu na strukture:

    // Konwertuje tekstowy adres IPv4 na strukture in_addr. Zwraca 0 w przypadku bledu!
    err = inet_aton ("192.168.1.1", &inp);
    if (err == 0) { perror ("Error: inet_aton"); return EXIT_FAILURE; }
    printf ("IPv4 address: %u\n", inp.s_addr); 

    // Konwertuje tekstowy adres IPv4 lub IPv6 i zapisuje do odpowiedniej struktury: in_addr lub in6_addr 
    // Zwraca 1 -> sukces, 0 -> adres nieprawidlowy, -1 -> blad
    err = inet_pton (AF_INET, "192.168.1.1", &inp); 
    if (err == -1) { perror ("Error: inet_pton"); return EXIT_FAILURE; }

    err = inet_pton (AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7334", &inp6);
    if (err == -1 || err == 0) { perror ("Error: inet_pton"); return EXIT_FAILURE; }

    // -------------------------------------------------------------------------------------
    
    // 2. Aby dostac tekstowa reprezentacje adresu ze struktury musimy wywolac odpowiednie funkcje: 

    // Konwertuje strukture in_addr na tekstowa reprezentacje adresu 
    char* bufferA = inet_ntoa (inp); // UWAGA. Funkcja zwraca wskaznik do bufora, musimy skopiowac zawartosc. 
    puts (bufferA); 

    // Konwertuje struktury in_addr, in6_addr na tekstowa reprezentacje adresu
    char bufferB[INET6_ADDRSTRLEN]; 
    const char* returnedBuffer = inet_ntop (AF_INET6, &inp6, bufferB, INET6_ADDRSTRLEN);
    if ( returnedBuffer == NULL ) { perror ("Error: inet_ntop"); return EXIT_FAILURE; }
    puts (bufferB);

    return 0;
}

/*
Wyjasnienie nazw funkcji:

inet_aton - internet - ASCII to Network (dla IPv4)
inet_pton - internet - presentation to Network (dla IPv4 i IPv6)

inet_ntoa - internet - Network to ASCII (dla IPv4)
inet_ntop - internet - Network to presentation (dla IPv4 i IPv6)

*/

void checkErr (char* message, int err) {
    if (err >= 0) { return; }
    puts (message);
    exit (EXIT_FAILURE);
}