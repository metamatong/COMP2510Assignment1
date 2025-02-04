#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 50

int generatePatientID();
void addPatient();
void displayPatient();
void searchPatient();
void deletePatient();
void manageDoctSched();

// Structure to store patient information
typedef struct {
    int id;
    char name[100];
    int age;
    char diagnosis[200];
    int roomNumber;
} Patient;

int patientCount = 0;
Patient patients[MAX_PATIENTS];

int main(void) {

    int choice;
    while (1) {
        printf("\nHospital Management System\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addPatient();
                break;
            case 2:
                displayPatient();
                break;
            case 3:
                searchPatient();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                manageDoctSched();
                break;
            case 6:
                return 0;
        }
    }
}

void addPatient()
{
    if (patientCount >= MAX_PATIENTS) {
        printf("Error: Too many patients!\n");
        return;
    }
    Patient patient;
    patient.id = generatePatientID();

    printf("Enter patient name: ");
    scanf(" %[^\n]", &patient.name);

    printf("Enter patient age: ");
    scanf("%d", &patient.age);

    printf("Enter patient diagnosis: ");
    scanf(" %[^\n]", &patient.diagnosis);

    printf("Enter patient room number: ");
    scanf("%d", &patient.roomNumber);

    patients[patientCount] = patient;
    patientCount++;

    printf("Patient added successfully ID: %d\n", patient.id);
}

int generatePatientID() {
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

void displayPatient()
{
    if (patientCount == 0) {
        printf("Error: No patients right now!\n");
        return;
    }

    for (int i = 0; i < patientCount; i++) {
        printf("Patient ID: %d\n", patients[i].id);
        printf("Patient Name: %s\n", patients[i].name);
        printf("Patient Age: %d\n", patients[i].age);
        printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
        printf("Patient Room No.: %d\n", patients[i].roomNumber);
        printf("\n");
    }

    printf("Displayed Every Patients Successfully.\n");
}

void searchPatient()
{
    int choice;
    printf("Enter how you would like to search patient: \n");
    printf("1. by ID\n");
    printf("2. by name\n");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1: {
            int inputId;
            printf("Enter the ID of the patient you are looking for: \n");
            scanf("%d", &inputId);
            if (inputId > patientCount || inputId < 0) {
                printf("Error: Invalid ID!\n", inputId);
            } else {
                int found = 0;
                for (int i = 0; i < patientCount; i++) {
                    if (inputId == patients[i].id) {
                        printf("\nPatient Found:\n");
                        printf("Patient ID: %d\n", patients[i].id);
                        printf("Patient Name: %s\n", patients[i].name);
                        printf("Patient Age: %d\n", patients[i].age);
                        printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                        printf("Patient Room No.: %d\n", patients[i].roomNumber);
                        printf("\n");
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Error: Patient Not Found!\n");
                }
                break;
            }
        }
        case 2: {
            char inputName[100];
            printf("Enter the ID of the patient you are looking for: ");
            scanf("%s", inputName);
            if (inputName[0] == '\0') {
                printf("Error: Empty Name!\n");
            }
            int found = 0;
            for (int i = 0; i < patientCount; i++) {
                if (strcmp(inputName, patients[i].name) == 0) {
                    printf("\nPatient Found:\n");
                    printf("Patient ID: %d\n", patients[i].id);
                    printf("Patient Name: %s\n", patients[i].name);
                    printf("Patient Age: %d\n", patients[i].age);
                    printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                    printf("Patient Room No.: %d\n", patients[i].roomNumber);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Error: Patient Not Found!\n");
            }
            break;
        }
        default:
            printf("Invalid Choice!\n");
    }
}

void deletePatient()
{
    if (patientCount == 0) {
        printf("No patients to delete.\n");
    }

    int id;
    int index = -1;

    printf("Please enter Patient ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1)
        {
        printf("Error: Patient with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < patientCount - 1; i++)
        {
        patients[i] = patients[i + 1];
    }

    patientCount--;
    printf("Patient with ID %d deleted successfully.\n", id);

}

void manageDoctSched() {
    printf("Test");
}