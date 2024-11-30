#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, new_n;

    printf("Enter the initial number of elements: ");
    scanf("%d", &n);

    // Allocate initial memory using malloc
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) return 1;

    // Fill the array with some values
    for (int i = 0; i < n; i++) arr[i] = i + 1;

    printf("Original array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    // Resize the array using realloc
    printf("Enter the new number of elements: ");
    scanf("%d", &new_n);
    arr = (int *)realloc(arr, new_n * sizeof(int));
    if (!arr) return 1;

    // Initialize new elements if the array is expanded
    for (int i = n; i < new_n; i++) arr[i] = 0;

    printf("Resized array: ");
    for (int i = 0; i < new_n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}

