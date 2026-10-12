#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    int rollNo;
    char name[50];
    char course[30];
    float marks;
} Student;

Student students[MAX];
int count = 0;

void addStudent() {
    if (count >= MAX) {
        printf("\nStudent record limit reached!\n");
        return;
    }

    printf("\n--- Add Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &students[count].rollNo);

    printf("Enter Name: ");
    scanf(" %49[^\n]", students[count].name);

    printf("Enter Course: ");
    scanf(" %29[^\n]", students[count].course);

    printf("Enter Marks: ");
    scanf("%f", &students[count].marks);

    count++;
    printf("\nStudent added successfully!\n");
}

void displayStudents() {
    int i;

    if (count == 0) {
        printf("\nNo student records available.\n");
        return;
    }

    printf("\n================ STUDENT RECORDS ================\n");
    printf("%-10s %-25s %-20s %-10s\n", "Roll No", "Name", "Course", "Marks");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-10d %-25s %-20s %-10.2f\n",
               students[i].rollNo,
               students[i].name,
               students[i].course,
               students[i].marks);
    }
}

void searchStudent() {
    int roll, i;

    printf("\nEnter Roll Number to Search: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            printf("\nStudent Found!\n");
            printf("Roll Number : %d\n", students[i].rollNo);
            printf("Name        : %s\n", students[i].name);
            printf("Course      : %s\n", students[i].course);
            printf("Marks       : %.2f\n", students[i].marks);
            return;
        }
    }

    printf("\nStudent not found.\n");
}

void updateStudent() {
    int roll, i;

    printf("\nEnter Roll Number to Update: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            printf("Enter New Name: ");
            scanf(" %49[^\n]", students[i].name);

            printf("Enter New Course: ");
            scanf(" %29[^\n]", students[i].course);

            printf("Enter New Marks: ");
            scanf("%f", &students[i].marks);

            printf("\nStudent updated successfully!\n");
            return;
        }
    }

    printf("\nStudent not found.\n");
}

void deleteStudent() {
    int roll, i, j;

    printf("\nEnter Roll Number to Delete: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            for (j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }

            count--;
            printf("\nStudent deleted successfully!\n");
            return;
        }
    }

    printf("\nStudent not found.\n");
}

int main() {
    int choice;

    do {
        printf("\n==================================================\n");
        printf("          STUDENT MANAGEMENT SYSTEM              \n");
        printf("==================================================\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("--------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("\nThank you for using Student Management System!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}
