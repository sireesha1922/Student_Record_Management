#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student { int roll; char name[50]; float marks; };
char currentRole[10];
char currentUser[50];

int loginSystem();
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin failed, exiting.....\n");
    }
    return 0;
}

int loginSystem(){
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("=========== Login screen ==========\n");
    printf("Username: ");
    if (scanf("%49s", username) != 1) return 0;
    printf("Password: ");
    if (scanf("%49s", password) != 1) return 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %19s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            printf("\nLogin successful. Welcome %s (%s)\n", currentUser, currentRole);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void mainMenu(){
    if (strcmp(currentRole, "ADMIN") == 0){
        adminMenu();
    }
    else if (strcmp(currentRole, "STAFF") == 0){
        staffMenu();
    }
    else {
        guestMenu();
    }
}

void adminMenu(){
    int choice;
    do{
        printf("\n======= ADMIN MENU ========\n");
        printf("1. Add new student\n");
        printf("2. Display all records\n");
        printf("3. Search record\n");
        printf("4. Update student details\n");
        printf("5. Delete student record\n");
        printf("6. Logout\n");
        printf("Enter the choice: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = -1; }
        switch(choice){
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out....\n"); return;
            default : printf("Invalid choice!\n");
        }
    } while (1);
}

void staffMenu(){
    int choice;
    do{
        printf("\n======= STAFF MENU ========\n");
        printf("1. Display all records\n");
        printf("2. Search record\n");
        printf("3. Update student details\n");
        printf("4. Logout\n");
        printf("Enter the choice: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = -1; }
        switch(choice){
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out....\n"); return;
            default : printf("Invalid choice!\n");
        }
    } while (1);
}

void guestMenu(){
    int choice;
    do{
        printf("\n======= GUEST MENU ========\n");
        printf("1. Display all records\n");
        printf("2. Search record\n");
        printf("3. Logout\n");
        printf("Enter the choice: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = -1; }
        switch(choice){
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out....\n"); return;
            default : printf("Invalid choice!\n");
        }
    } while (1);
}

void addStudent(){
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf("Permission denied: Only ADMIN can add students.\n");
        return;
    }

    struct Student s;
    printf("Enter roll number: ");
    if (scanf("%d", &s.roll) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    printf("Enter name (single token, no spaces): ");
    if (scanf("%49s", s.name) != 1) { printf("Invalid input.\n"); return; }
    printf("Enter marks: ");
    if (scanf("%f", &s.marks) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (fp) {
        struct Student tmp;
        while (fscanf(fp, "%d %49s %f", &tmp.roll, tmp.name, &tmp.marks) == 3) {
            if (tmp.roll == s.roll) {
                printf("A student with roll %d already exists. Aborting add.\n", s.roll);
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }

    fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening %s for writing.\n", STUDENT_FILE);
        return;
    }
    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayStudents(){
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No student records found (%s not present or empty).\n", STUDENT_FILE);
        return;
    }
    printf("\nRoll\tName\tMarks\n");
    printf("-----------------------------\n");
    struct Student s;
    int count = 0;
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
        count++;
    }
    if (count == 0) printf("No records to display.\n");
    fclose(fp);
}

void searchStudent(){
    int roll;
    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Student file not found.\n");
        return;
    }
    struct Student s;
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("Record found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Record with roll %d not found.\n", roll);
}

void updateStudent(){
    if (strcmp(currentRole, "GUEST") == 0) {
        printf("Permission denied: GUEST cannot update student details.\n");
        return;
    }

    int roll;
    printf("Enter roll number to update: ");
    if (scanf("%d", &roll) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Student file not found.\n");
        return;
    }
    FILE *tmp = fopen("tmp_students.txt", "w");
    if (!tmp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    struct Student s;
    int found = 0;
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Current details - Roll: %d, Name: %s, Marks: %.2f\n", s.roll, s.name, s.marks);
            printf("Enter new name (single token): ");
            if (scanf("%49s", s.name) != 1) { printf("Invalid input.\n"); fclose(fp); fclose(tmp); remove("tmp_students.txt"); return; }
            printf("Enter new marks: ");
            if (scanf("%f", &s.marks) != 1) { printf("Invalid input.\n"); fclose(fp); fclose(tmp); remove("tmp_students.txt"); return; }
            fprintf(tmp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        } else {
            fprintf(tmp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        }
    }

    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("No record with roll %d found. No updates made.\n", roll);
        remove("tmp_students.txt");
        return;
    }

    if (remove(STUDENT_FILE) != 0) {
        printf("Warning: could not remove original student file. Update aborted.\n");
        return;
    }
    if (rename("tmp_students.txt", STUDENT_FILE) != 0) {
        printf("Error renaming temporary file. Update may not have been saved.\n");
        return;
    }

    printf("Record updated successfully.\n");
}

void deleteStudent(){
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf("Permission denied: Only ADMIN can delete student records.\n");
        return;
    }

    int roll;
    printf("Enter roll number to delete: ");
    if (scanf("%d", &roll) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Student file not found.\n");
        return;
    }
    FILE *tmp = fopen("tmp_students.txt", "w");
    if (!tmp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    struct Student s;
    int found = 0;
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            continue;
        } else {
            fprintf(tmp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        }
    }

    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("No record with roll %d found. Nothing deleted.\n", roll);
        remove("tmp_students.txt");
        return;
    }

    if (remove(STUDENT_FILE) != 0) {
        printf("Warning: could not remove original student file. Delete aborted.\n");
        return;
    }
    if (rename("tmp_students.txt", STUDENT_FILE) != 0) {
        printf("Error renaming temporary file. Delete may not have been saved.\n");
        return;
    }

    printf("Record deleted successfully.\n");
}