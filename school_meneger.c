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
void print_db();
void free_memory();

// Global variable
struct School s;

int main() {
	
	// Initialize the database
    init_db();
	
    // Read from the file
    read_db();

    // Insert data to the database
    insert_to_db();

    print_db();

    // Free allocated memory
    free_memory();

    return EXIT_SUCCESS;
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
        
        
        int items_read = sscanf(line, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
                               first_name, last_name, telephone,
                               &level, &class_index,
                               &grades[0], &grades[1], &grades[2], &grades[3], &grades[4],
                               &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);

        if (items_read != 15) {
            fprintf(stderr, "Error reading line %d: Incorrect data format.\n", line_count);
            continue; 
        }

        if (level < 1 || level > LEVELS || class_index < 0 || class_index >= NUM_CLASSES) {
            fprintf(stderr, "Error reading line %d: Invalid level or class.\n", line_count);
            continue; 
        }

        // Validate grades
        int i;
        for (i = 0; i < NUM_CLASSES; i++) {
            if (grades[i] < 0 || grades[i] > 100) {
                fprintf(stderr, "Error reading line %d: Invalid grade.\n", line_count);
                continue; // Skip to the next line
            }
        }

        // Allocate memory for the student object
        struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));

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



void init_db() {
    int i, j;
    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            s.DB[i][j] = NULL;
        }
    }
}

void insert_to_db() {
   
}

void print_db() {
    int i, j, k;
    for (i = 0; i < LEVELS; i++) {
        for (j = 0; j < NUM_CLASSES; j++) {
            if (s.DB[i][j] != NULL) {
                printf("Level: %d, Class: %c\n", i + 1, j + 'A');
                printf("First Name: %s, Last Name: %s, Telephone: %s\n",
                       s.DB[i][j]->first_name, s.DB[i][j]->last_name, s.DB[i][j]->telephone);

                printf("Courses: ");
                for (k = 0; k < NUM_CLASSES; k++) {
                    printf("%c ", s.DB[i][j]->Course[k][0]);
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

