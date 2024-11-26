#include <stdio.h>
#include <string.h>

// Define a struct for an Address
struct Address {
    char street[50];
    char city[30];
    int zipCode;
};

// Define a struct for a Person that includes an Address
struct Person {
    char name[50];
    int age;
    struct Address address; // Nested struct
};

// Function to print person details
void printPerson(struct Person p) {
    printf("Name: %s\n", p.name);
    printf("Age: %d\n", p.age);
    printf("Address:\n");
    printf("  Street: %s\n", p.address.street);
    printf("  City: %s\n", p.address.city);
    printf("  ZIP Code: %d\n", p.address.zipCode);
}

int main() {
    // Initialize a nested struct
    struct Person person1;
    strcpy(person1.name, "John Doe");
    person1.age = 30;
    strcpy(person1.address.street, "123 Maple St");
    strcpy(person1.address.city, "Springfield");
    person1.address.zipCode = 12345;

    // Print person details
    printf("Details of Person 1:\n");
    printPerson(person1);

    // Another way to initialize using designated initializers
    struct Person person2 = {
        .name = "Jane Smith",
        .age = 25,
        .address = { .street = "456 Oak Ave", .city = "Greenville", .zipCode = 67890 }
    };

    // Print person details
    printf("\nDetails of Person 2:\n");
    printPerson(person2);

    return 0;
}

