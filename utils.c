#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "globals.h"
#include "doctor.h"
#include "patient.h"

void loadDoctorSchedule(void) {
    if (scheduleFile == NULL) {
        perror("Schedule file not open");
        return;
    }

    // Set the file pointer to the beginning.
    fseek(scheduleFile, 0, SEEK_SET);
    scheduleCount = 0;  // Reset the counter

    Schedule temp;
    while (fread(&temp, sizeof(Schedule), 1, scheduleFile) == 1) {
        if (temp.day >= 0 && temp.day < DAY_COUNT &&
            temp.shift >= 0 && temp.shift < SHIFT_COUNT) {
            if (scheduleCount < MAX_SCHEDULES) {
                scheduleList[scheduleCount] = temp;
                scheduleCount++;
            } else {
                fprintf(stderr, "Warning: Maximum schedule capacity reached. Some records may not be loaded.\n");
                break;
            }
        }
    }
}

void loadPatients(void) {
    if (patientsFile == NULL) {
        perror("Patients file not open");
        return;
    }

    // Set the file pointer to the beginning.
    fseek(patientsFile, 0, SEEK_SET);
    int count = 0;
    // Read the count of patient records
    if (fread(&count, sizeof(int), 1, patientsFile) != 1) {
        // File might be empty; no records to load.
        return;
    }

    for (int i = 0; i < count; i++) {
        // Create a temporary storage for patient fields.
        Patient temp;

        if (fread(&temp.id, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.name, sizeof(char), sizeof(temp.name), patientsFile) != sizeof(temp.name) ||
            fread(&temp.age, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.diagnosis, sizeof(char), sizeof(temp.diagnosis), patientsFile) != sizeof(temp.diagnosis) ||
            fread(&temp.roomNumber, sizeof(int), 1, patientsFile) != 1) {
            perror("Error reading patient data from file");
            break;
        }

        // Allocate a new Patient node.
        Patient *newPatient = malloc(sizeof(Patient));
        if (newPatient == NULL) {
            fprintf(stderr, "Error allocating memory for patient\n");
            exit(1);
        }
        // Copy the fields from temp into the new node.
        newPatient->id = temp.id;
        strncpy(newPatient->name, temp.name, sizeof(newPatient->name));
        newPatient->age = temp.age;
        strncpy(newPatient->diagnosis, temp.diagnosis, sizeof(newPatient->diagnosis));
        newPatient->roomNumber = temp.roomNumber;
        newPatient->next = NULL;

        // Append newPatient to the linked list.
        if (head == NULL) {
            head = newPatient;
        } else {
            Patient *current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newPatient;
        }
    }
}

int getValidInt(int minVal, int maxVal, const char *promptMsg) {
    char buffer[100];

    while (1) {
        printf("%s", promptMsg);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // If no newline found, flush remainder
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        char *endptr;
        long num = strtol(buffer, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }

        if (num < minVal || num > maxVal) {
            printf("Out of range. Enter a number between %d and %d.\n",
                   minVal, maxVal);
            continue;
        }
        return (int)num;
    }
}

void getValidString(char *dest, size_t maxLen, const char *promptMsg) {
    char buffer[512];

    while (1) {
        printf("%s", promptMsg);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        if (!strchr(buffer, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        size_t length = strlen(buffer);

        if (length == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        int allWhitespace = 1;
        for (size_t i = 0; i < length; i++) {
            if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r') {
                allWhitespace = 0;
                break;
            }
        }
        if (allWhitespace) {
            printf("Input cannot be whitespace only.\n");
            continue;
        }

        if (length > maxLen) {
            printf("Input too long! Maximum allowed is %zu characters.\n", maxLen);
            continue;
        }

        strncpy(dest, buffer, maxLen + 1);
        dest[maxLen] = '\0';
        break;
    }
}

void toLowerCase(char *str) {
    for (; *str; ++str) {
        *str = tolower(*str);
    }
}