#include <stdio.h>
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
        perror("Error opening doctor schedule file. The program is terminated.");
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

    menu();

    // Close schedule file
    fclose(scheduleFile);
    return 0;
}
