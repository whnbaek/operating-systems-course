#include <stdio.h>
#include <stdlib.h> // For malloc and free

int main() {
    int n; // Number of elements in the array
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Dynamically allocate memory for n integers
    int *arr = (int *)malloc(n * sizeof(int));
    
    // Check if malloc was successful
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit with an error
    }

    // Initialize and display the array
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("You entered: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(arr);
    printf("Memory freed successfully.\n");

    return 0;
}

