#include <stdio.h>
#include <string.h>
#include "doctor.h"
#include "utils.h"

Doctor doctors[DAY_COUNT][SHIFT_COUNT];

void manageDoctSched(void) {
    int choice;
    do {
        printf("\n=== Doctor Schedule Management ===\n");
        printf("1. Assign Doctor to a Shift\n");
        printf("2. View Weekly Schedule\n");
        printf("3. Return to Previous Menu\n");

        choice = getValidInt(1, 3, "\nEnter your choice (1-3): \n");

        switch (choice) {
            case 1:
                assignDoctorShift();
                break;
            case 2:
                viewWeeklySchedule();
                break;
            case 3:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid Choice! Please try again.\n");
        }
    } while (choice != 3);
}

void assignDoctorShift(void) {
    const char *const DAY_NAMES[DAY_COUNT] = {
        "Sunday", "Monday", "Tuesday",
        "Wednesday", "Thursday", "Friday", "Saturday"
    };

    const char *const SHIFT_NAMES[SHIFT_COUNT] = {
        "Morning", "Afternoon", "Evening"
    };

    char docName[50];
    Schedule schedule;

    while (1) {
        printf("\n-- Assign Doctor to a Shift --\n");

        int day = getValidInt(-1, DAY_COUNT - 1,
            "Enter day of the week [0=Sunday..6=Saturday] or -1 to cancel: ");
        if (day == -1) {
            printf("Assignment cancelled.\n");
            return;
        }

        int shift = getValidInt(0, SHIFT_COUNT - 1,
            "Enter shift [0=Morning, 1=Afternoon, 2=Evening]: ");

        getValidString(docName, 49, "Please enter the Doctor's name: ");

        if (strlen(doctors[day][shift].docName) != 0) {
            printf("Error: A doctor is already assigned for %s (%s).\n",
                   DAY_NAMES[day], SHIFT_NAMES[shift]);
            printf("Please try again.\n\n");
        } else {
            strcpy(doctors[day][shift].docName, docName);
            printf("Doctor '%s' assigned to %s (%s) successfully!\n",
                   docName, DAY_NAMES[day], SHIFT_NAMES[shift]);

            // prepare to save the schedule into a structure
            strncpy(schedule.docName, docName, MAX_DOC_NAME);
            schedule.day = day;
            schedule.shift = shift;

            // call saveSchedule() function to write it in a file.
            saveSchedule(schedule);
            break;
        }
    }
}

void viewWeeklySchedule(void) {
    const char *dayNames[DAY_COUNT] = {
        "Sunday", "Monday", "Tuesday",
        "Wednesday", "Thursday", "Friday", "Saturday"
    };
    const char *shiftNames[SHIFT_COUNT] = {
        "Morning", "Afternoon", "Evening"
    };

    // Create a temporary 2D array to store assignments.
    char assignments[DAY_COUNT][SHIFT_COUNT][MAX_DOC_NAME];

    // Initialize all assignments to an empty string.
    for (int i = 0; i < DAY_COUNT; i++) {
        for (int j = 0; j < SHIFT_COUNT; j++) {
            assignments[i][j][0] = '\0';
        }
    }

    // Open the file in binary read mode.
    FILE *file = fopen("../schedule.bin", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    Schedule schedule;
    // Read each schedule record and update the appropriate slot.
    while (fread(&schedule, sizeof(Schedule), 1, file) == 1) {
        if (schedule.day >= 0 && schedule.day < DAY_COUNT &&
            schedule.shift >= 0 && schedule.shift < SHIFT_COUNT) {
            // If multiple records exist for the same day/shift,
            // the latest record will overwrite previous ones.
            strcpy(assignments[schedule.day][schedule.shift], schedule.docName);
            }
    }
    fclose(file);

    // Print out the weekly schedule.
    printf("\n=== Weekly Doctor Schedule ===\n");
    for (int i = 0; i < DAY_COUNT; i++) {
        for (int j = 0; j < SHIFT_COUNT; j++) {
            if (assignments[i][j][0] == '\0') {
                printf("%-9s (%-9s): [No Doctor Assigned]\n",
                       dayNames[i], shiftNames[j]);
            } else {
                printf("%-9s (%-9s): Dr. %s\n",
                       dayNames[i], shiftNames[j],
                       assignments[i][j]);
            }
        }
        printf("\n");
    }
}

void saveSchedule(Schedule schedule) {
    FILE *file = fopen("../schedule.bin", "ab");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    if (fwrite(&schedule, sizeof(Schedule), 1, file) != 1) {
        perror("Error writing schedule to file");
    }
    fflush(file);
    fclose(file);
}
