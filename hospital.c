#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"
#include "patient.h"
#include "doctor.h"
#include "globals.h"
#include "utils.h"
#include "report.h"

void menu(void) {

    int choice;
    while (1) {
        printf("\n=== Hospital Management System ===\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Restore Backup\n");
        printf("7. Reporting & Analytics\n");
        printf("8. Exit\n");

        choice = getValidInt(1, 8, "\nEnter your choice (1-8): \n");

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
                restoreBackup();
            break;
            case 7:
                reportingMenu();
            break;
            case 8:
                while (head != NULL) {
                    PatientNode *temp = head;
                    head = head->next;
                    free(temp->patient);
                    free(temp);
                }
            return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}