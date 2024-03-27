#include <stdio.h>
int collatz_conjecture(int input);
int test_collatz_convergence(int input, int max_iter);

int main() {
    printf( "%d \n", collatz_conjecture(20) );
    printf( "%d \n", test_collatz_convergence(15, 100) );
    return 0;
}