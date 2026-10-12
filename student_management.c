#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <stdarg.h>
#include <emscripten.h>

EM_JS(char *, browser_prompt, (const char *message), {
    var value = window.prompt(UTF8ToString(message), "");
    if (value === null) return 0;
    var size = lengthBytesUTF8(value) + 1;
    var ptr = _malloc(size);
    stringToUTF8(value, ptr, size);
    return ptr;
});

static int browser_scanf(const char *format, ...) {
    char *input;
    int result = 0;
    va_list args;

    input = browser_prompt("Enter value:");
    if (input == 0) return 0;

    va_start(args, format);
    if (strstr(format, "%d") != NULL) {
        int *value = va_arg(args, int *);
        result = sscanf(input, "%d", value);
    } else if (strstr(format, "%f") != NULL) {
        float *value = va_arg(args, float *);
        result = sscanf(input, "%f", value);
    } else if (strstr(format, "%[") != NULL) {
        char *value = va_arg(args, char *);
        strncpy(value, input, 49);
        value[49] = '\0';
        result = 1;
    }
    va_end(args);
    free(input);
    return result;
}

#define scanf browser_scanf
#endif

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define COURSE_SIZE 30

typedef struct {
    int rollNo;
    char name[NAME_SIZE];
    char course[COURSE_SIZE];
    float marks;
} Student;

Student students[MAX_STUDENTS];
int count = 0;

int findStudent(int rollNo) {
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) return i;
    }
    return -1;
}

void addStudent(void) {
    int rollNo;
    if (count >= MAX_STUDENTS) {
        printf("\nStudent record limit reached!\n");
        return;
    }
    printf("\n--- Add Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &rollNo);
    if (findStudent(rollNo) != -1) {
        printf("Student with this Roll Number already exists!\n");
        return;
    }
    students[count].rollNo = rollNo;
    printf("Enter Name: ");
    scanf(" %49[^\n]", students[count].name);
    printf("Enter Course: ");
    scanf(" %29[^\n]", students[count].course);
    printf("Enter Marks (0-100): ");
    scanf("%f", &students[count].marks);
    if (students[count].marks < 0 || students[count].marks > 100) {
        printf("Invalid marks! Student was not added.\n");
        return;
    }
    count++;
    printf("Student added successfully!\n");
}

void displayStudents(void) {
    int i;
    if (count == 0) {
        printf("\nNo student records available.\n");
        return;
    }
    printf("\n================ STUDENT RECORDS ================\n");
    printf("%-10s %-25s %-20s %-10s\n", "Roll No", "Name", "Course", "Marks");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-10d %-25s %-20s %-10.2f\n", students[i].rollNo, students[i].name, students[i].course, students[i].marks);
    }
}

void searchStudent(void) {
    int rollNo, index;
    printf("\nEnter Roll Number to Search: ");
    scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    printf("\nStudent Found!\n");
    printf("Roll Number : %d\n", students[index].rollNo);
    printf("Name        : %s\n", students[index].name);
    printf("Course      : %s\n", students[index].course);
    printf("Marks       : %.2f\n", students[index].marks);
}

void updateStudent(void) {
    int rollNo, index;
    float marks;
    printf("\nEnter Roll Number to Update: ");
    scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    printf("Enter New Name: ");
    scanf(" %49[^\n]", students[index].name);
    printf("Enter New Course: ");
    scanf(" %29[^\n]", students[index].course);
    printf("Enter New Marks (0-100): ");
    scanf("%f", &marks);
    if (marks < 0 || marks > 100) {
        printf("Invalid marks! Student details were not updated.\n");
        return;
    }
    students[index].marks = marks;
    printf("Student updated successfully!\n");
}

void deleteStudent(void) {
    int rollNo, index, i;
    printf("\nEnter Roll Number to Delete: ");
    scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    for (i = index; i < count - 1; i++) students[i] = students[i + 1];
    count--;
    printf("Student deleted successfully!\n");
}

int main(void) {
    int choice;
    do {
        printf("\n==================================================\n");
        printf("           STUDENT MANAGEMENT SYSTEM             \n");
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
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("\nThank you for using Student Management System!\n"); break;
            default: printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 6);
    return 0;
}
