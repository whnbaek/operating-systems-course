#include <stdio.h>

int global_var[4]; /* Uninitialized variable stored in bss*/

int main(void)
{
    static int static_var; /* Uninitialized static variable stored in bss */
	printf("%d", global_var[0]);
    return 0;
}
