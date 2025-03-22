#include <stdio.h>
#include <string.h>
#include "patient.h"
#include "utils.h"


int patientCount = 0;
Patient patients[MAX_PATIENTS];

int generatePatientID(void) {
    int candidate = 1;
    while (1) {
        int found = 0;
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].id == candidate) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return candidate;
        }
        candidate++;
    }
}

void addPatient(void) {
    if (patientCount >= MAX_PATIENTS) {
        printf("Error: Too many patients!\n");
        return;
    }
    Patient patient;
    patient.id = generatePatientID();

    getValidString(patient.name, 99, "Enter patient name: ");
    patient.age = getValidInt(0, 200, "\nEnter patient age (0-200): \n");
    getValidString(patient.diagnosis, 199, "Enter patient diagnosis: ");
    patient.roomNumber = getValidInt(0, 9999, "\nEnter patient room number (0-9999): \n");

    patients[patientCount] = patient;
    patientCount++;

    printf("Patient added successfully! ID: %d\n", patient.id);
}

void displayPatient(void) {
    printf("=== All Patients Currently Checked In ===\n");
    if (patientCount == 0) {
        printf("No patients to display.\n");
        return;
    }

    for (int i = 0; i < patientCount; i++) {
        printf("Patient ID: %d\n", patients[i].id);
        printf("Patient Name: %s\n", patients[i].name);
        printf("Patient Age: %d\n", patients[i].age);
        printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
        printf("Patient Room No.: %d\n\n", patients[i].roomNumber);
    }
}

void searchPatient(void) {
    while (1) {
        printf("\n-- Search for a Patient --\n");
        printf("1. By ID\n");
        printf("2. By Name\n");
        printf("3. Return to Main Menu\n");

        int choice = getValidInt(1, 3, "\nEnter choice (1-3): \n");
        if (choice == 3) {
            return; // back to main menu
        }

        switch (choice) {
            case 1: {
                while (1) {
                    int inputId = getValidInt(-1, 9999,
                        "\nEnter the ID of the patient (or -1 to cancel): \n");

                    if (inputId == -1) {
                        break;
                    }

                    int found = 0;
                    for (int i = 0; i < patientCount; i++) {
                        if (patients[i].id == inputId) {
                            printf("\nPatient Found:\n");
                            printf("Patient ID: %d\n", patients[i].id);
                            printf("Patient Name: %s\n", patients[i].name);
                            printf("Patient Age: %d\n", patients[i].age);
                            printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                            printf("Patient Room No.: %d\n\n", patients[i].roomNumber);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Error: Patient Not Found! Please try again.\n");
                    } else {
                        break;
                    }
                }
                break;
            }

            case 2: {
                while (1) {
                    char inputName[100];
                    getValidString(inputName, 99,
                        "\nEnter the patient's name (or 'cancel' to go back): ");

                    if (strcmp(inputName, "cancel") == 0) {
                        break;
                    }

                    // Convert inputName to lowercase
                    for (size_t k = 0; k < strlen(inputName); k++) {
                        if (inputName[k] >= 'A' && inputName[k] <= 'Z') {
                            inputName[k] += ('a' - 'A');
                        }
                    }

                    int found = 0;
                    for (int i = 0; i < patientCount; i++) {
                        char tempName[100];
                        strcpy(tempName, patients[i].name);
                        // Convert to lowercase
                        for (size_t j = 0; j < strlen(tempName); j++) {
                            if (tempName[j] >= 'A' && tempName[j] <= 'Z') {
                                tempName[j] += ('a' - 'A');
                            }
                        }
                        if (strcmp(inputName, tempName) == 0) {
                            printf("\nPatient Found:\n");
                            printf("Patient ID: %d\n", patients[i].id);
                            printf("Patient Name: %s\n", patients[i].name);
                            printf("Patient Age: %d\n", patients[i].age);
                            printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                            printf("Patient Room No.: %d\n\n", patients[i].roomNumber);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Error: Patient Not Found! Please try again.\n");
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    }
}

void deletePatient(void) {
    if (patientCount == 0) {
        printf("No patients to delete.\n");
        return;
    }
    while (1) {
        int id = getValidInt(1, 9999,
            "Please enter Patient ID to delete (or 0 to cancel): ");
        if (id == 0) {
            return;
        }

        int index = -1;
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].id == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            printf("Error: Patient with ID %d not found. Please try again.\n", id);
        } else {
            // Shift array to remove the entry
            for (int i = index; i < patientCount - 1; i++) {
                patients[i] = patients[i + 1];
            }
            patientCount--;
            printf("Patient with ID %d deleted successfully.\n", id);
            break;
        }
    }
}