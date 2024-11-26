#include <stdio.h>

int main() {
    #if __STDC_VERSION__ >= 201710L
        printf("C17 or C18\n");
    #elif __STDC_VERSION__ >= 201112L
        printf("C11\n");
    #elif __STDC_VERSION__ >= 199901L
        printf("C99\n");
    #elif defined(__STDC__)
        printf("C89 or C90\n");
    #else
        printf("Pre-standard C\n");
    #endif

    return 0;
}
