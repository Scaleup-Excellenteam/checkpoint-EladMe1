#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CLASSES 10
#define LEVELS 12
#define MAX_LEN 20
#define FILE_NAME "students_with_class.txt"

enum course {
    A, B, C, D, E, F, G, H, I, J
};

struct Student {
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    char telephone[LEVELS];
    int grades[NUM_CLASSES];
    char* Course[NUM_CLASSES];
};

struct School {
    struct Student* DB[LEVELS][NUM_CLASSES];
};

// Function declarations
void read_db();
void init_db();
void insert_to_db();
void delete_from_db();
void print_db();
void free_memory();
void display_menu();
void edit_student_info();
void search_student();

// Global variable
static struct School s;

int main() {
	
    // Read from the file
    read_db();

	display_menu();

    // Free allocated memory
    free_memory();

    return EXIT_SUCCESS;
}

void display_menu() {
    int choice;
    do {
        printf("\nMenu\n");
        printf("1. Admission of a new student\n");
        printf("2. Deleting a student\n");
        printf("3. Edit student information\n");
        printf("4. Search for a student by first and last name\n");
        printf("5. Print database\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insert_to_db();
                break;
            case 2:
                delete_from_db();
                break;
            case 3:
                edit_student_info();
                break;
            case 4:
                search_student();
                break;
            case 5:
                print_db();
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}


void read_db() {
    FILE* fp;
    char line[MAX_LEN * 16];
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    char telephone[12];
    int level, class_index;
    int grades[NUM_CLASSES];
    int line_count = 0;

    fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++;

        int items_read = sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                               first_name, last_name, telephone,
                               &level, &class_index,
                               &grades[0], &grades[1], &grades[2], &grades[3], &grades[4],
                               &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);

        if (items_read != 15) {
            fprintf(stderr, "Error reading line %d: Incorrect data format.\n", line_count);
            continue;
        }

        // Validate grades
        int i;
        for (i = 0; i < NUM_CLASSES; i++) {
            if (grades[i] < 0 || grades[i] > 100) {
                fprintf(stderr, "Error reading line %d: Invalid grade.\n", line_count);
                continue; 
            }
        }

        // Allocate memory for the student object
        struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
        if (new_student == NULL) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // Copy data to the student object
        strncpy(new_student->first_name, first_name, MAX_LEN);
        strncpy(new_student->last_name, last_name, MAX_LEN);
        strncpy(new_student->telephone, telephone, 12);
        int j;
        for (j = 0; j < NUM_CLASSES; j++) {
            new_student->grades[j] = grades[j];
        }

        // Initialize Course as NULL for now
        for (j = 0; j < NUM_CLASSES; j++) {
            new_student->Course[j] = NULL;
        }

        // Add the student to the database
        s.DB[level - 1][class_index] = new_student;
    }

    fclose(fp);
}

void insert_to_db() {
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    char telephone[LEVELS];
    int level, class_index;
    int grades[NUM_CLASSES];
   

    printf("Enter student's first name: ");
    scanf("%s", first_name);

    printf("Enter student's last name: ");
    scanf("%s", last_name);

    printf("Enter student's telephone: ");
    scanf("%s", telephone);

    printf("Enter student's level (1-12): ");
    scanf("%d", &level);

    if (level < 1 || level > LEVELS) {
        printf("Invalid level. Level must be between 1 and %d.\n", LEVELS);
        return;
    }

    printf("Enter student's class (0-9): ");
    scanf("%d", &class_index);

    if (class_index < 0 || class_index >= NUM_CLASSES) {
        printf("Invalid class index. Class must be between 0 and %d.\n", NUM_CLASSES - 1);
        return;
    }

    printf("Enter student's grades (up to 10 grades separated by space): ");
    int i;
    for (i = 0; i < NUM_CLASSES; i++) {
        if (scanf("%d", &grades[i]) != 1) {
            printf("Invalid input for grade. Please enter a valid number.\n");
            return;
        }
        if (grades[i] < 0 || grades[i] > 100) {
            printf("Invalid grade. Grade must be between 0 and 100.\n");
            return;
        }
    }

    // Allocate memory for the student object
    struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
    if (new_student == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Copy data to the student object
    strncpy(new_student->first_name, first_name, MAX_LEN);
    strncpy(new_student->last_name, last_name, MAX_LEN);
    strncpy(new_student->telephone, telephone, 12);
	
	int c;
    for (c = 0; c < NUM_CLASSES; c++) {
        new_student->grades[c] = grades[c];
    }

    // Initialize Course as NULL for now
    int j;
    for (j = 0; j < NUM_CLASSES; j++) {
        new_student->Course[j] = NULL;
    }

    // Add the student to the database
    s.DB[level - 1][class_index] = new_student;

    printf("Student added successfully.\n");
}

void edit_student_info() {
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];

    printf("Enter student's first name: ");
    scanf("%s", first_name);

    printf("Enter student's last name: ");
    scanf("%s", last_name);

    int level, class_index;

    printf("Enter level of the student: ");
    scanf("%d", &level);

    if (level < 1 || level > LEVELS) {
        printf("Invalid level. Level must be between 1 and %d.\n", LEVELS);
        return;
    }

    printf("Enter class of the student: ");
    scanf("%d", &class_index);

    if (class_index < 0 || class_index >= NUM_CLASSES) {
        printf("Invalid class index. Class must be between 0 and %d.\n", NUM_CLASSES - 1);
        return;
    }

    if (s.DB[level - 1][class_index] != NULL) {
        if (strcmp(s.DB[level - 1][class_index]->first_name, first_name) == 0 &&
            strcmp(s.DB[level - 1][class_index]->last_name, last_name) == 0) {

            int subject;
            printf("Enter the subject to update the grade (0-9): ");
            scanf("%d", &subject);

            if (subject < 0 || subject >= NUM_CLASSES) {
                printf("Invalid subject index. Subject must be between 0 and %d.\n", NUM_CLASSES - 1);
                return;
            }

            int new_grade;
            printf("Enter the new grade: ");
            scanf("%d", &new_grade);

            if (new_grade < 0 || new_grade > 100) {
                printf("Invalid grade. Grade must be between 0 and 100.\n");
                return;
            }

            // Update the grade
            s.DB[level - 1][class_index]->grades[subject] = new_grade;

            printf("Grade updated successfully for student '%s %s' in class %c.\n", first_name, last_name, class_index + 'A');
        } else {
            printf("Student '%s %s' not found in class %c.\n", first_name, last_name, class_index + 'A');
        }
    } else {
        printf("No student found at the specified level and class.\n");
    }
}

void search_student() {
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];

    printf("Enter student's first name: ");
    scanf("%s", first_name);

    printf("Enter student's last name: ");
    scanf("%s", last_name);

    bool found = false;
	int i,j,k;
    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            if (s.DB[i][j] != NULL &&
                strcmp(s.DB[i][j]->first_name, first_name) == 0 &&
                strcmp(s.DB[i][j]->last_name, last_name) == 0) {

                printf("Student found:\n");
                printf("First Name: %s, Last Name: %s, Telephone: %s\n",
                       s.DB[i][j]->first_name, s.DB[i][j]->last_name, s.DB[i][j]->telephone);

                printf("Class: %c\n", j + 'A');

                printf("Grades: ");
                for (k = 0; k < NUM_CLASSES; k++) {
                    printf("%d ", s.DB[i][j]->grades[k]);
                }
                printf("\n");

                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    if (!found) {
        printf("Student '%s %s' not found in the database.\n", first_name, last_name);
    }
}

void delete_from_db() {
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];

    printf("Enter student's first name: ");
    scanf("%s", first_name);

    printf("Enter student's last name: ");
    scanf("%s", last_name);

    int level, class_index;

    printf("Enter level of student to delete (1-12): ");
    scanf("%d", &level);

    if (level < 1 || level > LEVELS) {
        printf("Invalid level. Level must be between 1 and %d.\n", LEVELS);
        return;
    }

    printf("Enter class of student to delete (0-9): ");
    scanf("%d", &class_index);

    if (class_index < 0 || class_index >= NUM_CLASSES) {
        printf("Invalid class index. Class must be between 0 and %d.\n", NUM_CLASSES - 1);
        return;
    }

    if (s.DB[level - 1][class_index] != NULL) {
        if (strcmp(s.DB[level - 1][class_index]->first_name, first_name) == 0 &&
            strcmp(s.DB[level - 1][class_index]->last_name, last_name) == 0) {

            // Free memory for the student's courses
            int k;
            for (k = 0; k < NUM_CLASSES; k++) {
                free(s.DB[level - 1][class_index]->Course[k]);
                s.DB[level - 1][class_index]->Course[k] = NULL;
            }

            // Free memory for the student
            free(s.DB[level - 1][class_index]);
            s.DB[level - 1][class_index] = NULL;

            printf("Student '%s %s' deleted from class %c.\n", first_name, last_name, class_index + 'A');
        } else {
            printf("Student '%s %s' not found in class %c.\n", first_name, last_name, class_index + 'A');
        }
    } else {
        printf("No student found at the specified level and class.\n");
    }
}


void init_db() {
    int i, j;
    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            s.DB[i][j] = NULL;
        }
    }
}

void print_db() {
    int i, j, k;

    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            if (s.DB[i][j] != NULL) {
                printf("Level: %d, Class: %c\n", i + 1, j + 'A');
                printf("First Name: %s, Last Name: %s, Telephone: %s\n",
                       s.DB[i][j]->first_name, s.DB[i][j]->last_name, s.DB[i][j]->telephone);

                printf("Grades: ");
                for (k = 0; k < NUM_CLASSES; k++) {
                    printf("%d ", s.DB[i][j]->grades[k]);
                }
                printf("\n\n");
            }
        }
    }
}


void free_memory() {
    int i, j, k;
    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            if (s.DB[i][j] != NULL) {
                for (k = 0; k < NUM_CLASSES; k++) {
                    free(s.DB[i][j]->Course[k]);
                }
                free(s.DB[i][j]);
            }
        }
    }
}
