#include <stdio.h>

struct Example {
    char c;      // 1 byte
    int i;       // 4 bytes
};

int main() {
    printf("Size of char: %zu bytes\n", sizeof(char));
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of struct Example: %zu bytes\n", sizeof(struct Example));
    return 0;
}

