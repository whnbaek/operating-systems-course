#include <stdio.h>

int main() {
    // Declare an array of labels as values
    static void *labels[] = {&&label1, &&label2, &&label3};

    // Input choice
    int choice;

    printf("Enter a number (0-2): ");
    scanf("%d", &choice);

    // Check bounds to avoid invalid memory access
    if (choice < 0 || choice >= 3) {
        printf("Invalid choice.\n");
        return 1;
    }

    // Jump to the chosen label
    goto *labels[choice];

label1:
    printf("You chose label 1!\n");
    return 0;

label2:
    printf("You chose label 2!\n");
    return 0;

label3:
    printf("You chose label 3!\n");
    return 0;
}

