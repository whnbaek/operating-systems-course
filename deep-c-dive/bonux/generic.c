#include <stdio.h>

#define PRINT_VALUE(x) _Generic((x),            \
    int: printf("Integer: %d\n", (int)(x)),     \
    float: printf("Float: %.2f\n", (double)(x)), \
    double: printf("Double: %.2f\n", (double)(x)), \
    default: printf("Unknown type\n"))

int main() {
    int i = 42;
    double d = 3.14;
    float f = 2.71f;

    PRINT_VALUE(i);  // Correct type-safe printing
    PRINT_VALUE(d);
    PRINT_VALUE(f);

    return 0;
}

