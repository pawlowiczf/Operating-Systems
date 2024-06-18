struct node {
    struct node* next; 
    int* clientSocket; 
}; 

typedef struct node node_t; 

void enQueue (int* clientSocket);
int* deQueue ();