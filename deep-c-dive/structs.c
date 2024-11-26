#include <stdio.h>
#include <string.h>

// Define a struct for a Student
struct Student {
    int id;                 // Student ID
    char name[50];          // Student Name
    float grade;            // Student Grade
};

// Function to print student details
void printStudent(struct Student s) {
    printf("ID: %d\n", s.id);
    printf("Name: %s\n", s.name);
    printf("Grade: %.2f\n", s.grade);
}

int main() {
    // Declare and initialize a Student struct
    struct Student student1;
    student1.id = 101;
    strcpy(student1.name, "Alice");
    student1.grade = 87.5;

    // Print the student details
    printf("Details of Student 1:\n");
    printStudent(student1);

    // Declare and initialize another Student struct using designated initializers
    struct Student student2 = { .id = 102, .name = "Bob", .grade = 90.3 };

    // Print the second student's details
    printf("\nDetails of Student 2:\n");
    printStudent(student2);

    return 0;
}

