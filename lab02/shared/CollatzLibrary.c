int collatz_conjecture(int input) {
    if (input % 2 == 0) { return input / 2; }
    return 3 * input + 1; 
}

int test_collatz_convergence(int input, int max_iter) {
    int counter = 1; 

    while (counter <= max_iter) {
        input = collatz_conjecture(input);
        if (input == 1) { return counter; }
        counter++;
    }

    return -1; 
}

