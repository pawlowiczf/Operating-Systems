#include <stdlib.h>

int main() {
    // Wywolanie komendy w terminalu:
    
    int return_value;
    return_value = system ("ls -l /");
    return return_value;
}