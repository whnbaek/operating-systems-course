#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Allocate memory using calloc
    int *arr = (int *)calloc(n, sizeof(int));
    if (!arr) return 1;

    // Display the array initialized to 0
    printf("Array initialized with calloc: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}

