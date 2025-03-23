#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"
#include "globals.h"

int main(void) {

    // Read doctor schedule file to use it globally
    scheduleFile = fopen("../schedule.bin", "a+b");

    // We need this because we want to create the file if the file doesn't exist, but want to read from the file if
    // it exists. So we use "a+b" but since it reads from the end, we set the cursor back to front.
    fseek(scheduleFile, 0, SEEK_SET);
    if (scheduleFile == NULL) {
        perror("Error opening doctor schedule file. The program will terminate.");
        return 0;
    }

    scheduleCount = 0;  // Reset the counter before loading.
    Schedule tempSchedule;

    // Read each schedule record from the file.
    while (fread(&tempSchedule, sizeof(Schedule), 1, scheduleFile) == 1) {
        if (tempSchedule.day >= 0 && tempSchedule.day < DAY_COUNT &&
            tempSchedule.shift >= 0 && tempSchedule.shift < SHIFT_COUNT) {
            if (scheduleCount < MAX_SCHEDULES) {
                scheduleList[scheduleCount] = tempSchedule;
                scheduleCount++;
            } else {
                fprintf(stderr, "Warning: Maximum schedule capacity reached. Some records may not be loaded.\n");
                break;
            }
        }
    }

    // Read patients file to use it globally
    patientsFile = fopen("../patients.bin", "a+b");

    // We need this because we want to create the file if the file doesn't exist, but want to read from the file if
    // it exists. So we use "a+b" but since it reads from the end, we set the cursor back to front.
    fseek(patientsFile, 0, SEEK_SET);
    if (patientsFile == NULL) {
        perror("Error opening patients file. The program will terminate.");
        return 0;
    }

    int count = 0;
    // Read the count of patient records
    if (fread(&count, sizeof(int), 1, patientsFile) != 1) {
        // File might be empty; no records to load.
    }

    for (int i = 0; i < count; i++) {
        // Create a temporary storage for patient fields
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

    menu();

    // Close files when session ends
    fclose(scheduleFile);
    fclose(patientsFile);
    return 0;
}
