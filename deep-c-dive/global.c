#include <stdio.h>

int global_var[4]; /* Uninitialized variable stored in bss*/

int main(void)
{
	static int i;
	printf("%d", global_var[0]);
    return 0;
}
