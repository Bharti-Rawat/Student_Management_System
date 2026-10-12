#include <stdio.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
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
    if (count >= MAX_STUDENTS) { printf("\nStudent record limit reached!\n"); return; }
    printf("\n--- Add Student ---\nEnter Roll Number: ");
    scanf("%d", &rollNo);
    if (findStudent(rollNo) != -1) { printf("Student with this Roll Number already exists!\n"); return; }
    students[count].rollNo = rollNo;
    printf("Enter Name: "); scanf(" %49[^\n]", students[count].name);
    printf("Enter Course: "); scanf(" %29[^\n]", students[count].course);
    printf("Enter Marks (0-100): "); scanf("%f", &students[count].marks);
    if (students[count].marks < 0 || students[count].marks > 100) { printf("Invalid marks! Student was not added.\n"); return; }
    count++;
    printf("Student added successfully!\n");
}

void displayStudents(void) {
    int i;
    if (count == 0) { printf("\nNo student records available.\n"); return; }
    printf("\n================ STUDENT RECORDS ================\n");
    printf("%-10s %-25s %-20s %-10s\n", "Roll No", "Name", "Course", "Marks");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < count; i++) printf("%-10d %-25s %-20s %-10.2f\n", students[i].rollNo, students[i].name, students[i].course, students[i].marks);
}

void searchStudent(void) {
    int rollNo, index;
    printf("\nEnter Roll Number to Search: "); scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) { printf("Student not found.\n"); return; }
    printf("\nStudent Found!\nRoll Number : %d\nName        : %s\nCourse      : %s\nMarks       : %.2f\n", students[index].rollNo, students[index].name, students[index].course, students[index].marks);
}

void updateStudent(void) {
    int rollNo, index; float marks;
    printf("\nEnter Roll Number to Update: "); scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) { printf("Student not found.\n"); return; }
    printf("Enter New Name: "); scanf(" %49[^\n]", students[index].name);
    printf("Enter New Course: "); scanf(" %29[^\n]", students[index].course);
    printf("Enter New Marks (0-100): "); scanf("%f", &marks);
    if (marks < 0 || marks > 100) { printf("Invalid marks! Student details were not updated.\n"); return; }
    students[index].marks = marks;
    printf("Student updated successfully!\n");
}

void deleteStudent(void) {
    int rollNo, index, i;
    printf("\nEnter Roll Number to Delete: "); scanf("%d", &rollNo);
    index = findStudent(rollNo);
    if (index == -1) { printf("Student not found.\n"); return; }
    for (i = index; i < count - 1; i++) students[i] = students[i + 1];
    count--;
    printf("Student deleted successfully!\n");
}

#ifdef __EMSCRIPTEN__
static char web_output[16000];

static void set_output(const char *text) {
    strncpy(web_output, text, sizeof(web_output) - 1);
    web_output[sizeof(web_output) - 1] = '\0';
}

EMSCRIPTEN_KEEPALIVE
int web_add_student(int rollNo, const char *name, const char *course, float marks) {
    if (count >= MAX_STUDENTS) { set_output("Student record limit reached!"); return 0; }
    if (findStudent(rollNo) != -1) { set_output("Student with this Roll Number already exists!"); return 0; }
    if (marks < 0 || marks > 100) { set_output("Invalid marks! Student was not added."); return 0; }
    students[count].rollNo = rollNo;
    strncpy(students[count].name, name, NAME_SIZE - 1); students[count].name[NAME_SIZE - 1] = '\0';
    strncpy(students[count].course, course, COURSE_SIZE - 1); students[count].course[COURSE_SIZE - 1] = '\0';
    students[count].marks = marks;
    count++;
    set_output("Student added successfully!");
    return 1;
}

EMSCRIPTEN_KEEPALIVE
const char *web_display_students(void) {
    int i, used = 0;
    if (count == 0) { set_output("No student records available."); return web_output; }
    used = snprintf(web_output, sizeof(web_output), "%-10s %-25s %-20s %-10s\n---------------------------------------------------------------\n", "Roll No", "Name", "Course", "Marks");
    for (i = 0; i < count && used < (int)sizeof(web_output); i++) {
        used += snprintf(web_output + used, sizeof(web_output) - used, "%-10d %-25s %-20s %-10.2f\n", students[i].rollNo, students[i].name, students[i].course, students[i].marks);
    }
    return web_output;
}

EMSCRIPTEN_KEEPALIVE
const char *web_search_student(int rollNo) {
    int index = findStudent(rollNo);
    if (index == -1) set_output("Student not found.");
    else snprintf(web_output, sizeof(web_output), "Student Found!\n\nRoll Number : %d\nName        : %s\nCourse      : %s\nMarks       : %.2f", students[index].rollNo, students[index].name, students[index].course, students[index].marks);
    return web_output;
}

EMSCRIPTEN_KEEPALIVE
int web_update_student(int rollNo, const char *name, const char *course, float marks) {
    int index = findStudent(rollNo);
    if (index == -1) { set_output("Student not found."); return 0; }
    if (marks < 0 || marks > 100) { set_output("Invalid marks! Student details were not updated."); return 0; }
    strncpy(students[index].name, name, NAME_SIZE - 1); students[index].name[NAME_SIZE - 1] = '\0';
    strncpy(students[index].course, course, COURSE_SIZE - 1); students[index].course[COURSE_SIZE - 1] = '\0';
    students[index].marks = marks;
    set_output("Student updated successfully!");
    return 1;
}

EMSCRIPTEN_KEEPALIVE
int web_delete_student(int rollNo) {
    int index = findStudent(rollNo), i;
    if (index == -1) { set_output("Student not found."); return 0; }
    for (i = index; i < count - 1; i++) students[i] = students[i + 1];
    count--;
    set_output("Student deleted successfully!");
    return 1;
}

EMSCRIPTEN_KEEPALIVE
int web_get_count(void) { return count; }

EMSCRIPTEN_KEEPALIVE
const char *web_get_message(void) { return web_output; }
#endif

int main(void) {
    int choice;
    do {
        printf("\n==================================================\n");
        printf("           STUDENT MANAGEMENT SYSTEM             \n");
        printf("==================================================\n");
        printf("1. Add Student\n2. Display Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Exit\n");
        printf("--------------------------------------------------\nEnter your choice: ");
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
