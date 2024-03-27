#include <dlfcn.h>
#include <stdio.h>
int collatz_conjecture(int input);
int test_collatz_convergence(int input, int max_iter);

int main(void) {
    //
    void *handler = dlopen("./libCollatzLibrary.so", RTLD_LAZY);
    if (!handler) { printf("Blad otw. biblioteki \n"); return 0; }

    int (*f)(int);
    int (*g)(int, int);
    f = dlsym(handler, "collatz_conjecture");
    g = dlsym(handler, "test_collatz_convergence");

    if (dlerror() != 0) { printf("Blad funkcji \n"); return 0; }
    
    printf( "%d \n", f(10) );
    printf( "%d \n", g(15, 100) );

    dlclose(handler);

    return 0;
}