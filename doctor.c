#include <stdio.h>
#include <string.h>
#include "doctor.h"

#include "globals.h"
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

            // Add the schedule record into the in-memory schedule list.
            if (scheduleCount < MAX_SCHEDULES) {
                strncpy(scheduleList[scheduleCount].docName, docName, MAX_DOC_NAME);
                scheduleList[scheduleCount].day = day;
                scheduleList[scheduleCount].shift = shift;
                scheduleCount++;
            } else {
                printf("Schedule list is full!\n");
                return;
            }

            // Write the doctor schedule data to the file so that we can "save".
            saveScheduleToFile(scheduleFile);

            printf("Doctor '%s' assigned to %s (%s) successfully!\n",
                   docName, DAY_NAMES[day], SHIFT_NAMES[shift]);
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

    // Populate the assignments array from the in-memory schedule list.
    for (int i = 0; i < scheduleCount; i++) {
        int day = scheduleList[i].day;
        int shift = scheduleList[i].shift;
        if (day >= 0 && day < DAY_COUNT && shift >= 0 && shift < SHIFT_COUNT) {
            strncpy(assignments[day][shift], scheduleList[i].docName, MAX_DOC_NAME);
        }
    }

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

void saveScheduleToFile(FILE *file) {
    // Overwrite entire file by moving the file pointer to the beginning.
    rewind(file);

    // Write all schedule records stored in the global scheduleList.
    const size_t written = fwrite(scheduleList, sizeof(Schedule), scheduleCount, file);
    if (written != scheduleCount) {
        perror("Error writing schedule list to file. Check if the file is open");
    }
    fflush(file);
    printf("Doctor schedules saved successfully.\n");
}
