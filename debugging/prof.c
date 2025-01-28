#include <stdio.h>

void foo() {
	volatile int j=0;
    for (int i = 0; i < 100000000; i++) {
        // some computation
		j++;
    }
}

void bar() {
	volatile int k =0;
    for (int i = 0; i < 100000000; i++) {
        // some computation
		k++;
    }
}

int main() {
    printf("Start profiling example\n");
    foo();
    bar();
    return 0;
}

