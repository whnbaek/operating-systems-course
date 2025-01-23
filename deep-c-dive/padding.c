#include <stdio.h>
struct  Example {
    int i;       // 4 bytes
	char c;      // 1 byte
};

int main() {
    printf("Size of char: %zu bytes\n", sizeof(char));
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of struct Example: %zu bytes\n", sizeof(struct Example));
	struct Example a;
	char * p = &a;
	p[6] = 1;
    return 0;
}

