#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

#include <time.h>

#include "globals.h"
#include "utils.h"

// Global variable for generating unique IDs.
int nextPatientID = 1;

int generatePatientID(void) {
    return nextPatientID++;
}

void addPatient(void) {
    Patient *newPatient = malloc(sizeof(Patient));
    if (newPatient == NULL) {
        fprintf(stderr, "Error allocating memory for newPatient\n");
        exit(1);
    }
    newPatient->id = generatePatientID();
    newPatient->next = NULL;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(newPatient->admissionDate, sizeof(newPatient->admissionDate),
             "%Y-%m-%d %H:%M:%S", tm_info);

    newPatient->dischargeDate[0] = '\0';

    getValidString(newPatient->name, 99, "Enter patient name: \n");
    newPatient->age = getValidInt(0, 200, "Enter patient age (0-200): \n");
    getValidString(newPatient->diagnosis, 199, "Enter patient diagnosis: \n");
    newPatient->roomNumber = getValidInt(0, 9999, "Enter patient room number (0-9999): \n");

    if (head == NULL) {
        head = newPatient;
    } else {
        Patient *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPatient;
    }
    savePatientsToFile(patientsFile);
    printf("Patient added successfully! ID: %d\n", newPatient->id);
}

void displayPatient(void) {
    printf("=== All Patients Currently Checked In ===\n");
    if (head == NULL) {
        printf("No patients to display.\n");
        return;
    }
    Patient *current = head;
    while (current != NULL) {
        printf("\nPatient ID: %d\n", current->id);
        printf("Patient Name: %s\n", current->name);
        printf("Patient Age: %d\n", current->age);
        printf("Patient Diagnosis: %s\n", current->diagnosis);
        printf("Patient Room No.: %d\n", current->roomNumber);
        printf("Admission Date: %s\n", current->admissionDate);
        if (strlen(current->dischargeDate) == 0) {
            printf("Discharge Date: N/A\n");
        } else {
            printf("Discharge Date: %s\n", current->dischargeDate);
        }
        current = current->next;
    }
}

void searchPatientByID(void) {
    int searchId = getValidInt(-1, 9999, "\nEnter the Patient ID to search: ");
    if (searchId == -1) {
        return;
    }
    Patient *current = head;
    while (current != NULL) {
        if (current->id == searchId) {
            printf("\nPatient Found:\n");
            printf("Patient ID: %d\n", current->id);
            printf("Patient Name: %s\n", current->name);
            printf("Patient Age: %d\n", current->age);
            printf("Patient Diagnosis: %s\n", current->diagnosis);
            printf("Patient Room No.: %d\n", current->roomNumber);
            printf("Admission Date: %s\n", current->admissionDate);
            if (strlen(current->dischargeDate) == 0) {
                printf("Discharge Date: N/A\n\n");
            } else {
                printf("Discharge Date: %s\n\n", current->dischargeDate);
            }
            return;
        }
        current = current->next;
    }
    printf("Error: Patient with ID %d not found!\n", searchId);
}

void searchPatientByName(void) {
    char searchName[100];
    getValidString(searchName, 99, "\nEnter the Patient Name to search: ");
    if (strcmp(searchName, "-1") == 0) {
        return;
    }

    char searchNameLower[100];
    strcpy(searchNameLower, searchName);
    toLowerCase(searchNameLower);

    Patient *current = head;
    while (current != NULL) {
        char currentNameLower[100];
        strcpy(currentNameLower, current->name);
        toLowerCase(currentNameLower);
        if (strcmp(currentNameLower, searchNameLower) == 0) {
            printf("\nPatient Found:\n");
            printf("Patient ID: %d\n", current->id);
            printf("Patient Name: %s\n", current->name);
            printf("Patient Age: %d\n", current->age);
            printf("Patient Diagnosis: %s\n", current->diagnosis);
            printf("Patient Room No.: %d\n", current->roomNumber);
            printf("Admission Date: %s\n", current->admissionDate);
            if (strlen(current->dischargeDate) == 0) {
                printf("Discharge Date: N/A\n\n");
            } else {
                printf("Discharge Date: %s\n\n", current->dischargeDate);
            }
            return;
        }
        current = current->next;
    }
    printf("Error: Patient with name '%s' not found!\n", searchName);
}

void searchPatient(void) {
    while (1) {
        printf("Search Options:\n");
        printf("1. Search by ID\n");
        printf("2. Search by Name\n");
        printf("3. Return to Main Menu\n");

        int choice = getValidInt(1, 3, "Enter your choice: ");

        if (choice == 1) {
            searchPatientByID();
        } else if (choice == 2) {
            searchPatientByName();
        } else if (choice == 3) {
            return;
        }
    }
}

void deletePatient(void) {
    int idToDelete = getValidInt(1, 9999, "Enter Patient ID to discharge: ");
    if (head == NULL) {
        printf("No patients to discharge.\n");
        return;
    }

    Patient *current = head;
    Patient *prev = NULL;

    // Handle if the first node is to be discharged
    if (current != NULL && current->id == idToDelete) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(current->dischargeDate, sizeof(current->dischargeDate),
                 "%Y-%m-%d %H:%M:%S", tm_info);

        head = current->next;
        current->next = dischargedHead;
        dischargedHead = current;

        printf("Patient with ID %d discharged successfully.\n", idToDelete);
        return;
    }

    // Search for the patient in the active list
    while (current != NULL) {
        if (current->id == idToDelete) {
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(current->dischargeDate, sizeof(current->dischargeDate),
                     "%Y-%m-%d %H:%M:%S", tm_info);

            // Remove from active list
            prev->next = current->next;
            // Add to discharged list at the beginning
            current->next = dischargedHead;
            dischargedHead = current;

            printf("Patient with ID %d discharged successfully.\n", idToDelete);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Error: Patient with ID %d not found.\n", idToDelete);
}

void savePatientsToFile(FILE *file) {
    // Overwrite entire file by moving the file pointer to the beginning.
    rewind(file);

    // First, count the number of patient records.
    int count = 0;
    Patient *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Write the count to the file.
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        perror("Error writing patient count to file");
        return;
    }

    // Iterate through the linked list and write each patient's data.
    current = head;
    while (current != NULL) {
        // Write each field one by one, skipping the "next" pointer.
        if (fwrite(&current->id, sizeof(int), 1, file) != 1 ||
            fwrite(current->name, sizeof(char), sizeof(current->name), file) != sizeof(current->name) ||
            fwrite(&current->age, sizeof(int), 1, file) != 1 ||
            fwrite(current->diagnosis, sizeof(char), sizeof(current->diagnosis), file) != sizeof(current->diagnosis) ||
            fwrite(&current->roomNumber, sizeof(int), 1, file) != 1 ||
            fwrite(current->admissionDate, sizeof(char), sizeof(current->admissionDate), file) != sizeof(current->admissionDate) ||
            fwrite(current->dischargeDate, sizeof(char), sizeof(current->dischargeDate), file) != sizeof(current->dischargeDate)) {
            perror("Error writing patient data to file");
            return;
            }

        current = current->next;
    }
    fflush(file);
    printf("Patients saved successfully.\n");
}