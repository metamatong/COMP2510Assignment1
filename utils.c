#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"
#include "globals.h"
#include "doctor.h"
#include "patient.h"

void loadDoctorSchedule(void) {
    if (scheduleFile == NULL) {
        perror("Schedule file not open");
        return;
    }

    // Set the file pointer to the beginning.
    fseek(scheduleFile, 0, SEEK_SET);
    scheduleCount = 0;  // Reset the counter

    Schedule temp;
    while (fread(&temp, sizeof(Schedule), 1, scheduleFile) == 1) {
        if (temp.day >= 0 && temp.day < DAY_COUNT &&
            temp.shift >= 0 && temp.shift < SHIFT_COUNT) {
            if (scheduleCount < MAX_SCHEDULES) {
                scheduleList[scheduleCount] = temp;
                scheduleCount++;
            } else {
                fprintf(stderr, "Warning: Maximum schedule capacity reached. Some records may not be loaded.\n");
                break;
            }
        }
    }
}


/**
 * loadPatients
 *
 * Loads patients from the file pointed to by patientsFile and inserts them
 * into the active patients list (using the new Patient/PatientNode structure).
 */
void loadPatients(void) {
    int count;
    if (fread(&count, sizeof(int), 1, patientsFile) != 1) {
        perror("Error reading patient count");
        return;
    }
    for (int i = 0; i < count; i++) {
        Patient temp;
        if (fread(&temp.id, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.name, sizeof(char), sizeof(temp.name), patientsFile) != sizeof(temp.name) ||
            fread(&temp.age, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.diagnosis, sizeof(char), sizeof(temp.diagnosis), patientsFile) != sizeof(temp.diagnosis) ||
            fread(&temp.roomNumber, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.admissionDate, sizeof(char), sizeof(temp.admissionDate), patientsFile) != sizeof(temp.admissionDate) ||
            fread(temp.dischargeDate, sizeof(char), sizeof(temp.dischargeDate), patientsFile) != sizeof(temp.dischargeDate)) {
            perror("Error reading patient data");
            return;
            }
        Patient *newPatient = malloc(sizeof(Patient));
        if (newPatient == NULL) {
            perror("Error allocating memory for loaded patient");
            exit(1);
        }
        *newPatient = temp;  // Copy data into new allocated Patient

        PatientNode *newNode = malloc(sizeof(PatientNode));
        if (newNode == NULL) {
            perror("Error allocating memory for patient node");
            free(newPatient);
            exit(1);
        }
        newNode->patient = newPatient;
        newNode->next = head;
        head = newNode;
    }
}

/**
 * backupRoutine
 *
 * Writes all active patients to a backup file.
 */
void backupRoutine(void) {
    FILE *patientBackupFile = fopen("patient_backup.bin", "wb");
    if (!patientBackupFile) {
        perror("Error opening backup file");
        return;
    }
    // Count active patients
    int count = 0;
    PatientNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    if (fwrite(&count, sizeof(int), 1, patientBackupFile) != 1) {
        perror("Error writing patient count to backup file");
        fclose(patientBackupFile);
        return;
    }
    // Write each patient's data
    current = head;
    while (current != NULL) {
        Patient *p = current->patient;
        if (fwrite(&p->id, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(p->name, sizeof(char), sizeof(p->name), patientBackupFile) != sizeof(p->name) ||
            fwrite(&p->age, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(p->diagnosis, sizeof(char), sizeof(p->diagnosis), patientBackupFile) != sizeof(p->diagnosis) ||
            fwrite(&p->roomNumber, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(p->admissionDate, sizeof(char), sizeof(p->admissionDate), patientBackupFile) != sizeof(p->admissionDate) ||
            fwrite(p->dischargeDate, sizeof(char), sizeof(p->dischargeDate), patientBackupFile) != sizeof(p->dischargeDate)) {
            perror("Error writing patient data to backup file");
            fclose(patientBackupFile);
            return;
            }
        current = current->next;
    }
    fclose(patientBackupFile);
    printf("Backup routine completed successfully.\n");
}

// Backup thread function: sleeps for a fixed interval and then calls the backup routine.
void *backupThreadFunction(void *arg) {
    const int BACKUP_INTERVAL = 300; // backup every two minutes
    while (1) {
        sleep(BACKUP_INTERVAL);
        backupRoutine();
    }
    return NULL;
}


// Restore doctor schedule from a backup file.
void restoreDoctorSchedule(void) {
    char backupFilename[256];
    getValidString(backupFilename, sizeof(backupFilename) - 1,
                   "Enter backup file name for doctor schedule: ");
    FILE *fp = fopen(backupFilename, "rb");
    if (fp == NULL) {
        perror("Error opening schedule backup file");
        return;
    }
    // Determine the number of Schedule records in the backup file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);
    int count = fileSize / sizeof(Schedule);
    if (count > MAX_SCHEDULES) {
         fprintf(stderr, "Backup file has too many records; only the first %d will be restored.\n", MAX_SCHEDULES);
         count = MAX_SCHEDULES;
    }
    scheduleCount = fread(scheduleList, sizeof(Schedule), count, fp);
    if (scheduleCount != count) {
        perror("Error reading schedule backup file");
    } else {
        printf("Restored %d doctor schedule record(s) from backup.\n", scheduleCount);
    }
    fclose(fp);

    // Write the restored data back to the original schedule file.
    saveScheduleToFile(scheduleFile);
}

/**
 * restorePatients
 *
 * Reads patient data from a backup file and restores them into the active patients list.
 */
void restorePatients(void) {
    FILE *fp = fopen("patient_backup.bin", "rb");
    if (!fp) {
        perror("Error opening backup file for restore");
        return;
    }
    int count;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        perror("Error reading patient count from backup file");
        fclose(fp);
        return;
    }
    // (Optionally free existing patients before restoring)
    for (int i = 0; i < count; i++) {
        Patient temp;
        if (fread(&temp.id, sizeof(int), 1, fp) != 1 ||
            fread(temp.name, sizeof(char), sizeof(temp.name), fp) != sizeof(temp.name) ||
            fread(&temp.age, sizeof(int), 1, fp) != 1 ||
            fread(temp.diagnosis, sizeof(char), sizeof(temp.diagnosis), fp) != sizeof(temp.diagnosis) ||
            fread(&temp.roomNumber, sizeof(int), 1, fp) != 1 ||
            fread(temp.admissionDate, sizeof(char), sizeof(temp.admissionDate), fp) != sizeof(temp.admissionDate) ||
            fread(temp.dischargeDate, sizeof(char), sizeof(temp.dischargeDate), fp) != sizeof(temp.dischargeDate)) {
            perror("Error reading patient data from backup file");
            fclose(fp);
            return;
            }
        Patient *newPatient = malloc(sizeof(Patient));
        if (newPatient == NULL) {
            perror("Error allocating memory for restored patient");
            exit(1);
        }
        *newPatient = temp;
        PatientNode *newNode = malloc(sizeof(PatientNode));
        if (newNode == NULL) {
            perror("Error allocating memory for patient node during restore");
            free(newPatient);
            exit(1);
        }
        newNode->patient = newPatient;
        newNode->next = head;
        head = newNode;
    }
    fclose(fp);
    printf("Restore routine completed successfully.\n");
}

// Restore backup menu: let user choose what to restore.
void restoreBackup(void) {
    printf("\n=== Restore Backup Menu ===\n");
    printf("1. Restore Doctor Schedule\n");
    printf("2. Restore Patients\n");
    printf("3. Restore Both\n");
    printf("4. Cancel\n");
    int choice = getValidInt(1, 4, "Enter your choice: ");
    switch (choice) {
        case 1:
            restoreDoctorSchedule();
            break;
        case 2:
            restorePatients();
            break;
        case 3:
            restoreDoctorSchedule();
            restorePatients();
            break;
        case 4:
            printf("Restore cancelled.\n");
            break;
        default:
            printf("Invalid choice.\n");
    }
}

int getValidInt(int minVal, int maxVal, const char *promptMsg) {
    char buffer[100];

    while (1) {
        printf("%s", promptMsg);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // If no newline found, flush remainder
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        char *endptr;
        long num = strtol(buffer, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }

        if (num < minVal || num > maxVal) {
            printf("Out of range. Enter a number between %d and %d.\n",
                   minVal, maxVal);
            continue;
        }
        return (int)num;
    }
}

void getValidString(char *dest, size_t maxLen, const char *promptMsg) {
    char buffer[512];

    while (1) {
        printf("%s", promptMsg);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        if (!strchr(buffer, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        size_t length = strlen(buffer);

        if (length == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        int allWhitespace = 1;
        for (size_t i = 0; i < length; i++) {
            if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r') {
                allWhitespace = 0;
                break;
            }
        }
        if (allWhitespace) {
            printf("Input cannot be whitespace only.\n");
            continue;
        }

        if (length > maxLen) {
            printf("Input too long! Maximum allowed is %zu characters.\n", maxLen);
            continue;
        }

        strncpy(dest, buffer, maxLen + 1);
        dest[maxLen] = '\0';
        break;
    }
}

void toLowerCase(char *str) {
    for (; *str; ++str) {
        *str = tolower(*str);
    }
}