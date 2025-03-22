#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"
#include "patient.h"
#include "doctor.h"
#include "utils.h"

void menu(void) {

    patients = malloc(INITIAL_PATIENT_CAPACITY * sizeof(Patient));
    if (patients == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    int choice;
    while (1) {
        printf("\n=== Hospital Management System ===\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Exit\n");

        choice = getValidInt(1, 6, "\nEnter your choice (1-6): \n");

        switch (choice) {
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
                free(patients);
                return;
        }
    }
}