#include <stdio.h>

#define MAX_PATIENTS 50

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

    patient.id = patientCount +1;

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
    printf("Test");
}

void deletePatient()
{
    printf("Test");
}

void manageDoctSched() {
    printf("Test");
}