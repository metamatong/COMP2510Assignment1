#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "patient.h"
#include "utils.h"


PatientNode *head = NULL;
PatientNode *dischargedHead = NULL;

/* Global variable for generating unique IDs */
int nextPatientID = 1;

int generatePatientID(void) {
    return nextPatientID++;
}

/**
 * addPatient
 *
 * Allocates a new Patient and its associated PatientNode.
 * Inserts the new node at the head of the active patient list.
 * On any allocation error, frees the allocated memory and exits.
 */
void addPatient(void) {
    /* Allocate memory for a new Patient */
    Patient *newPatient = malloc(sizeof(Patient));
    if (newPatient == NULL) {
        fprintf(stderr, "Error allocating memory for new patient\n");
        exit(1);
    }
    newPatient->id = generatePatientID();
    newPatient->dischargeDate[0] = '\0';

    /* Set the admission date */
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(newPatient->admissionDate, sizeof(newPatient->admissionDate),
             "%Y-%m-%d %H:%M:%S", tm_info);

    /* Get user input for patient details */
    getValidString(newPatient->name, 99, "Enter patient name: \n");
    newPatient->age = getValidInt(0, 200, "Enter patient age (0-200): \n");
    getValidString(newPatient->diagnosis, 199, "Enter patient diagnosis: \n");
    newPatient->roomNumber = getValidInt(0, 9999, "Enter patient room number (0-9999): \n");

    /* Allocate a new PatientNode to hold the patient */
    PatientNode *newNode = malloc(sizeof(PatientNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error allocating memory for new patient node\n");
        free(newPatient);  // Free the patient memory if node allocation fails
        exit(1);
    }
    newNode->patient = newPatient;

    /* Insert new node at the head of the active list */
    newNode->next = head;
    head = newNode;

    /* Save active patients to file (patientsFile is assumed to be defined in globals) */
    savePatientsToFile(patientsFile);
    printf("Patient added successfully! ID: %d\n", newPatient->id);
    free(newPatient);
}

/**
 * displayPatient
 *
 * Displays all active patients currently in the active list.
 */
void displayPatient(void) {
    printf("=== All Patients Currently Checked In ===\n");
    if (head == NULL) {
        printf("No patients to display.\n");
        return;
    }
    PatientNode *current = head;
    while (current != NULL) {
        Patient *p = current->patient;
        printf("\nPatient ID: %d\n", p->id);
        printf("Patient Name: %s\n", p->name);
        printf("Patient Age: %d\n", p->age);
        printf("Patient Diagnosis: %s\n", p->diagnosis);
        printf("Patient Room No.: %d\n", p->roomNumber);
        printf("Admission Date: %s\n", p->admissionDate);
        if (strlen(p->dischargeDate) == 0) {
            printf("Discharge Date: N/A\n");
        } else {
            printf("Discharge Date: %s\n", p->dischargeDate);
        }
        current = current->next;
    }
}

/**
 * searchPatientByID
 *
 * Searches for a patient by their ID in the active list.
 */
void searchPatientByID(void) {
    int searchId = getValidInt(-1, 9999, "\nEnter the Patient ID to search: ");
    if (searchId == -1) {
        return;
    }
    PatientNode *current = head;
    while (current != NULL) {
        if (current->patient->id == searchId) {
            Patient *p = current->patient;
            printf("\nPatient Found:\n");
            printf("Patient ID: %d\n", p->id);
            printf("Patient Name: %s\n", p->name);
            printf("Patient Age: %d\n", p->age);
            printf("Patient Diagnosis: %s\n", p->diagnosis);
            printf("Patient Room No.: %d\n", p->roomNumber);
            printf("Admission Date: %s\n", p->admissionDate);
            if (strlen(p->dischargeDate) == 0) {
                printf("Discharge Date: N/A\n\n");
            } else {
                printf("Discharge Date: %s\n\n", p->dischargeDate);
            }
            return;
        }
        current = current->next;
    }
    printf("Error: Patient with ID %d not found!\n", searchId);
}

/**
 * searchPatientByName
 *
 * Searches for a patient by name in the active list.
 */
void searchPatientByName(void) {
    char searchName[100];
    getValidString(searchName, 99, "\nEnter the Patient Name to search: ");
    if (strcmp(searchName, "-1") == 0) {
        return;
    }

    char searchNameLower[100];
    strcpy(searchNameLower, searchName);
    toLowerCase(searchNameLower);

    PatientNode *current = head;
    while (current != NULL) {
        char currentNameLower[100];
        strcpy(currentNameLower, current->patient->name);
        toLowerCase(currentNameLower);
        if (strcmp(currentNameLower, searchNameLower) == 0) {
            Patient *p = current->patient;
            printf("\nPatient Found:\n");
            printf("Patient ID: %d\n", p->id);
            printf("Patient Name: %s\n", p->name);
            printf("Patient Age: %d\n", p->age);
            printf("Patient Diagnosis: %s\n", p->diagnosis);
            printf("Patient Room No.: %d\n", p->roomNumber);
            printf("Admission Date: %s\n", p->admissionDate);
            if (strlen(p->dischargeDate) == 0) {
                printf("Discharge Date: N/A\n\n");
            } else {
                printf("Discharge Date: %s\n\n", p->dischargeDate);
            }
            return;
        }
        current = current->next;
    }
    printf("Error: Patient with name '%s' not found!\n", searchName);
}

/**
 * searchPatient
 *
 * Presents a menu to search for a patient either by ID or by Name.
 */
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

/**
 * deletePatient
 *
 * "Discharges" a patient by moving their node from the active list to the discharged list.
 * It updates the discharge date and rearranges the pointers.
 */
void deletePatient(void) {
    int idToDelete = getValidInt(1, 9999, "Enter Patient ID to discharge: ");
    if (head == NULL) {
        printf("No patients to discharge.\n");
        return;
    }

    PatientNode *current = head;
    PatientNode *prev = NULL;

    /* Check if the head node is the one to be discharged */
    if (current != NULL && current->patient->id == idToDelete) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(current->patient->dischargeDate, sizeof(current->patient->dischargeDate),
                 "%Y-%m-%d %H:%M:%S", tm_info);

        head = current->next;
        current->next = dischargedHead;
        dischargedHead = current;

        printf("Patient with ID %d discharged successfully.\n", idToDelete);
        return;
    }

    /* Traverse the active list to find the patient */
    while (current != NULL) {
        if (current->patient->id == idToDelete) {
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(current->patient->dischargeDate, sizeof(current->patient->dischargeDate),
                     "%Y-%m-%d %H:%M:%S", tm_info);

            /* Remove from active list */
            prev->next = current->next;
            /* Add to discharged list at the beginning */
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

/**
 * savePatientsToFile
 *
 * Writes all active patients to the provided file.
 * Adjust this function if discharged patients also need to be saved.
 */
void savePatientsToFile(FILE *file) {
    /* Rewind the file to start overwriting */
    rewind(file);
    int count = 0;
    PatientNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    /* Write the count of patients */
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        perror("Error writing patient count to file");
        return;
    }

    /* Write each patient's data, one field at a time */
    current = head;
    while (current != NULL) {
        Patient *p = current->patient;
        if (fwrite(&p->id, sizeof(int), 1, file) != 1 ||
            fwrite(p->name, sizeof(char), sizeof(p->name), file) != sizeof(p->name) ||
            fwrite(&p->age, sizeof(int), 1, file) != 1 ||
            fwrite(p->diagnosis, sizeof(char), sizeof(p->diagnosis), file) != sizeof(p->diagnosis) ||
            fwrite(&p->roomNumber, sizeof(int), 1, file) != 1 ||
            fwrite(p->admissionDate, sizeof(char), sizeof(p->admissionDate), file) != sizeof(p->admissionDate) ||
            fwrite(p->dischargeDate, sizeof(char), sizeof(p->dischargeDate), file) != sizeof(p->dischargeDate)) {
            perror("Error writing patient data to file");
            return;
        }
        current = current->next;
    }
    fflush(file);
    printf("Patients saved successfully.\n");
}

/**
 * freeAllPatients
 *
 * Frees all memory allocated for both the active and discharged patient lists.
 */
void freeAllPatients(void) {
    PatientNode *current = head;
    while (current != NULL) {
        PatientNode *temp = current;
        current = current->next;
        free(temp->patient);
        free(temp);
    }
    head = NULL;

    current = dischargedHead;
    while (current != NULL) {
        PatientNode *temp = current;
        current = current->next;
        free(temp->patient);
        free(temp);
    }
    dischargedHead = NULL;
}

