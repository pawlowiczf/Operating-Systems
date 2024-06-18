// #include <stdlib.h>
// #include <sys/types.h>

#define MAX_CLIENTS_NUMBER 8
#define MAX_MESSAGE_SIZE 512
#define MAX_QUEUE_NAME_LENGTH 32

typedef enum MessageType {
    MT_INIT     = 1,
    MT_LIST     = 2,
    MT_SEND_ALL = 3,
    MT_SEND_ONE = 4,
    MT_MESSAGE  = 5,
    MT_STOP     = 6,
    MT_STOP_SERVER = 7
} MessageType;

typedef struct Message {
    //
    MessageType messageType;
    int fromClientId;
    int toClientID;
    char message[MAX_MESSAGE_SIZE];
} Message;

#define MESSAGE_SIZE sizeof(Message)

typedef struct Client {
    //
    int empty_b; 
    int id; 
    int clientSocket;
    int nowRegistered; 
} Client; 