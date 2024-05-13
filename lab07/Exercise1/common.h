#include <stdlib.h>
#include <sys/types.h>

#define MAX_TEXT 512
#define MAX_CLIENTS_NUMBER 10
#define SERVER_ID 1 

typedef enum CallType {
    INIT = 0,
    OTHER = 1,
    INIT_RESPONSE = 3,

} CallType;

typedef struct Message {
    //
    long messageType; 
    char message[ MAX_TEXT ];

    CallType callType; // typ wiadomosci: init, init_response, other
    key_t clientMsgQueueKey; // klucz do kolejki komunikatow klienta 
    int clientID; // id klienta - uzywany przez serwer do rozrozniania klientow, ktorzy wysylaja komunikaty

} Message;
