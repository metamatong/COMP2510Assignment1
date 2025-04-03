#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "patient.h"
#include "doctor.h"
#include "utils.h"
#include "report.h"

#include <time.h>

#include "hospital.h"

/**
 * Generates a text file summarizing the total number of current patients,
 * and optionally lists them. Reads data from your in-memory linked list 'head'.
 */
void generatePatientSummaryReport(void) {
    FILE *fp = fopen("patient_summary_report.txt", "w");
    if (!fp) {
        perror("Error creating patient summary report");
        return;
    }

    int activeCount = 0, dischargedCount = 0;
    Patient *current = head;
    while (current != NULL) {
        activeCount++;
        current = current->next;
    }
    current = dischargedHead;
    while (current != NULL) {
        dischargedCount++;
        current = current->next;
    }

    int dayCount, weekCount, monthCount;
    countAdmissionPeriods(&dayCount, &weekCount, &monthCount);

    fprintf(fp, "Patient Summary Report\n");
    fprintf(fp, "----------------------\n");
    fprintf(fp, "Active patients: %d\n", activeCount);
    fprintf(fp, "Discharged patients: %d\n", dischargedCount);
    fprintf(fp, "Total number of patients: %d\n\n", activeCount + dischargedCount);

    fprintf(fp, "List of Active Patients:\n");
    current = head;
    while (current != NULL) {
        fprintf(fp, "ID: %d, Name: %s, Room: %d\n", current->id, current->name, current->roomNumber);
        fprintf(fp, "Admission Date: %s\n", current->admissionDate);
        fprintf(fp, "Discharge Date: N/A\n\n");
        current = current->next;
    }

    fprintf(fp, "List of Discharged Patients:\n");
    current = dischargedHead;
    while (current != NULL) {
        fprintf(fp, "ID: %d, Name: %s, Room: %d\n", current->id, current->name, current->roomNumber);
        fprintf(fp, "Admission Date: %s\n", current->admissionDate);
        fprintf(fp, "Discharge Date: %s\n\n", current->dischargeDate);
        current = current->next;
    }

    fprintf(fp, "Admissions in the Last Time Periods:\n");
    fprintf(fp, "------------------------------------\n");
    fprintf(fp, "Last 24 hours: %d\n", dayCount);
    fprintf(fp, "Last 7 days:   %d\n", weekCount);
    fprintf(fp, "Last 30 days:  %d\n", monthCount);

    fclose(fp);
    printf("Patient summary report generated: 'patient_summary_report.txt'\n");
}

/**
 * updateCountsForPatient
 *
 * Helper function that updates the admission counters for a given patient.
 *
 * @param p Pointer to a Patient structure.
 * @param now The current time (as time_t).
 * @param dayCount Pointer to the counter for the last 24 hours.
 * @param weekCount Pointer to the counter for the last 7 days.
 * @param monthCount Pointer to the counter for the last 30 days.
 */
void updateCountsForPatient(Patient *p, time_t now, int *dayCount, int *weekCount, int *monthCount) {
    int year, month, day;
    if (sscanf(p->admissionDate, "%d-%d-%d", &year, &month, &day) == 3) {
        struct tm admission_tm = {0};
        admission_tm.tm_year = year - 1900;
        admission_tm.tm_mon  = month - 1;
        admission_tm.tm_mday = day;
        admission_tm.tm_hour = 0;
        admission_tm.tm_min  = 0;
        admission_tm.tm_sec  = 0;
        admission_tm.tm_isdst = -1;
        time_t admission_time = mktime(&admission_tm);
        if (admission_time != -1) {
            double diff = difftime(now, admission_time);
            if (diff <= SECONDS_IN_DAY)
                (*dayCount)++;
            if (diff <= SECONDS_IN_WEEK)
                (*weekCount)++;
            if (diff <= SECONDS_IN_MONTH)
                (*monthCount)++;
        }
    }
}

/**
 * countAdmissionPeriods
 *
 * Counts the number of patients admitted within the last day, week, and month.
 * It processes both active patients (from 'head') and discharged patients (from 'dischargedHead').
 *
 * @param dayCount Pointer to an integer where the count for the last 24 hours will be stored.
 * @param weekCount Pointer to an integer where the count for the last 7 days will be stored.
 * @param monthCount Pointer to an integer where the count for the last 30 days will be stored.
 */
void countAdmissionPeriods(int *dayCount, int *weekCount, int *monthCount) {
    *dayCount = 0;
    *weekCount = 0;
    *monthCount = 0;
    time_t now = time(nullptr);

    // Process active patients.
    Patient *current = head;
    while (current != NULL) {
        updateCountsForPatient(current, now, dayCount, weekCount, monthCount);
        current = current->next;
    }

    // Process discharged patients.
    current = dischargedHead;
    while (current != NULL) {
        updateCountsForPatient(current, now, dayCount, weekCount, monthCount);
        current = current->next;
    }
}


/**
 * Generates a text file showing how many shifts each doctor covers in a week.
 * Uses your global 'scheduleList' array and 'scheduleCount' to see who is assigned.
 */
void generateDoctorUtilizationReport(void) {
    FILE *fp = fopen("doctor_utilization_report.txt", "w");
    if (!fp) {
        perror("Error creating doctor utilization report");
        return;
    }

    // We’ll keep a simple tally in memory.
    // For a bigger system, you might use a dynamic structure or a map.
    // Let's assume at most 100 different doctor names for demonstration.
    char uniqueDocs[100][MAX_DOC_NAME];
    int docShiftCount[100];
    int uniqueCount = 0;

    // Initialize the arrays
    for (int i = 0; i < 100; i++) {
        uniqueDocs[i][0] = '\0';
        docShiftCount[i] = 0;
    }

    // Loop over the schedule and count shifts per doctor
    for (int i = 0; i < scheduleCount; i++) {
        char *docName = scheduleList[i].docName;

        // Check if docName is already in uniqueDocs
        int foundIndex = -1;
        for (int k = 0; k < uniqueCount; k++) {
            if (strcmp(uniqueDocs[k], docName) == 0) {
                foundIndex = k;
                break;
            }
        }
        if (foundIndex == -1 && uniqueCount < 100) {
            // new doctor
            strncpy(uniqueDocs[uniqueCount], docName, MAX_DOC_NAME);
            docShiftCount[uniqueCount] = 1;
            uniqueCount++;
        } else if (foundIndex >= 0) {
            docShiftCount[foundIndex]++;
        }
    }

    fprintf(fp, "Doctor Utilization Report\n");
    fprintf(fp, "-------------------------\n");
    for (int i = 0; i < uniqueCount; i++) {
        fprintf(fp, "Doctor: %s, Shifts Covered: %d\n",
                uniqueDocs[i], docShiftCount[i]);
    }

    fclose(fp);
    printf("Doctor utilization report generated: 'doctor_utilization_report.txt'\n");
}

/**
 * Generates a text file showing how many patients occupy each room.
 * Uses the global linked list 'head' to gather data.
 */
void generateRoomUsageReport(void) {
    FILE *fp = fopen("room_usage_report.txt", "w");
    if (!fp) {
        perror("Error creating room usage report");
        return;
    }

    // If roomNumber can be up to 9999, you could do an array of size 10000,
    // or you could use a dynamic data structure. For simplicity, let's do a
    // smaller array and assume room numbers < 1000. Adjust as needed.
    const int MAX_ROOMS = 1000;
    int roomUsage[MAX_ROOMS];
    for (int i = 0; i < MAX_ROOMS; i++) {
        roomUsage[i] = 0;
    }

    Patient *current = head;
    while (current != NULL) {
        if (current->roomNumber >= 0 && current->roomNumber < MAX_ROOMS) {
            roomUsage[current->roomNumber]++;
        }
        current = current->next;
    }

    fprintf(fp, "Room Usage Report\n");
    fprintf(fp, "-----------------\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (roomUsage[i] > 0) {
            fprintf(fp, "Room %d: %d patient(s)\n", i, roomUsage[i]);
        }
    }

    fclose(fp);
    printf("Room usage report generated: 'room_usage_report.txt'\n");
}

/**
 * A simple menu for generating different reports.
 * Integrate it into your main menu or call it from 'menu()' in hospital.c.
 */
void reportingMenu(void) {
    while (1) {
        printf("\n=== Reporting and Analytics Menu ===\n");
        printf("1. Generate Patient Summary Report\n");
        printf("2. Generate Doctor Utilization Report\n");
        printf("3. Generate Room Usage Report\n");
        printf("4. Return to Main Menu\n");

        int choice = getValidInt(1, 4, "Enter your choice: ");

        switch (choice) {
            case 1:
                generatePatientSummaryReport();
                break;
            case 2:
                generateDoctorUtilizationReport();
                break;
            case 3:
                generateRoomUsageReport();
                break;
            case 4:
                menu();
        }
    }

}
