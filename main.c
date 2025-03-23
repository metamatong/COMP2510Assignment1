#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"
#include "globals.h"
#include "utils.h"

int main(void) {

    // Read doctor schedule file to use it globally
    scheduleFile = fopen("../schedule.bin", "a+b");
    if (scheduleFile == NULL) {
        perror("Error opening doctor schedule file. The program will terminate.");
        return 0;
    }
    // We need this because we want to create the file if the file doesn't exist, but want to read from the file if
    // it exists. So we use "a+b" but since it reads from the end, we set the cursor back to front.
    fseek(scheduleFile, 0, SEEK_SET);

    // Load schedule data into memory.
    loadDoctorSchedule();

    // Read patients file to use it globally
    patientsFile = fopen("../patients.bin", "a+b");
    if (patientsFile == NULL) {
        perror("Error opening patients file. The program will terminate.");
        return 0;
    }
    // We need this because we want to create the file if the file doesn't exist, but want to read from the file if
    // it exists. So we use "a+b" but since it reads from the end, we set the cursor back to front.
    fseek(patientsFile, 0, SEEK_SET);

    // Load patients data into the linked list.
    loadPatients();

    menu();

    // Close files when session ends
    fclose(scheduleFile);
    fclose(patientsFile);
    return 0;
}
