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

void loadPatients(void) {
    if (patientsFile == NULL) {
        perror("Patients file not open");
        return;
    }

    // Set the file pointer to the beginning.
    fseek(patientsFile, 0, SEEK_SET);
    int count = 0;
    // Read the count of patient records
    if (fread(&count, sizeof(int), 1, patientsFile) != 1) {
        // File might be empty; no records to load.
        return;
    }

    for (int i = 0; i < count; i++) {
        // Create a temporary storage for patient fields.
        Patient temp;

        if (fread(&temp.id, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.name, sizeof(char), sizeof(temp.name), patientsFile) != sizeof(temp.name) ||
            fread(&temp.age, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.diagnosis, sizeof(char), sizeof(temp.diagnosis), patientsFile) != sizeof(temp.diagnosis) ||
            fread(&temp.roomNumber, sizeof(int), 1, patientsFile) != 1 ||
            fread(temp.admissionDate, sizeof(char), sizeof(temp.admissionDate), patientsFile) != sizeof(temp.admissionDate) ||
            fread(temp.dischargeDate, sizeof(char), sizeof(temp.dischargeDate), patientsFile) != sizeof(temp.dischargeDate)) {
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
        strcpy(newPatient->name, temp.name);
        newPatient->age = temp.age;
        strcpy(newPatient->diagnosis, temp.diagnosis);
        newPatient->roomNumber = temp.roomNumber;
        strcpy(newPatient->admissionDate, temp.admissionDate);
        strcpy(newPatient->dischargeDate, temp.dischargeDate);
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
}



void backupRoutine(void) {
    // Flush the schedule file to ensure all data is written.
    // This line forces any data still in the output buffer of the open schedule file to be written to disk.
    // This guarantees that the in-memory file state is current before creating the backup.
    fflush(scheduleFile);

    // Get the current time for timestamping.
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Backup docotr schedule
    char scheduleBackupFilename[256];
    strftime(scheduleBackupFilename, sizeof(scheduleBackupFilename), "../schedule_backup_%Y%m%d%H%M.bin", tm_info);
    FILE *scheduleBackupFile = fopen(scheduleBackupFilename, "wb");
    if (scheduleBackupFile == NULL) {
        perror("Error opening schedule backup file");
    } else {
        size_t written = fwrite(scheduleList, sizeof(Schedule), scheduleCount, scheduleBackupFile);
        if (written != scheduleCount) {
            perror("Error writing schedule backup file");
        } else {
            printf("Schedule backup created: %s\n", scheduleBackupFilename);
        }
        fclose(scheduleBackupFile);
    }

    // Backup patient schedule
    // Flush the patients file too.
    fflush(patientsFile);

    char patientBackupFilename[256];
    strftime(patientBackupFilename, sizeof(patientBackupFilename), "../patients_backup_%Y%m%d%H%M.bin", tm_info);
    FILE *patientBackupFile = fopen(patientBackupFilename, "wb");
    if (patientBackupFile == NULL) {
        perror("Error opening patient backup file");
    } else {
        // Count the number of patients in the linked list.
        int count = 0;
        Patient *current = head;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        // Write the count first.
        if (fwrite(&count, sizeof(int), 1, patientBackupFile) != 1) {
            perror("Error writing patient count to backup file");
        }
        // Now, write each patient's data (skipping the 'next' pointer).
        current = head;
        while (current != NULL) {
            if (fwrite(&current->id, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(current->name, sizeof(char), sizeof(current->name), patientBackupFile) != sizeof(current->name) ||
            fwrite(&current->age, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(current->diagnosis, sizeof(char), sizeof(current->diagnosis), patientBackupFile) != sizeof(current->diagnosis) ||
            fwrite(&current->roomNumber, sizeof(int), 1, patientBackupFile) != 1 ||
            fwrite(current->admissionDate, sizeof(char), sizeof(current->admissionDate), patientBackupFile) != sizeof(current->admissionDate) ||
            fwrite(current->dischargeDate, sizeof(char), sizeof(current->dischargeDate), patientBackupFile) != sizeof(current->dischargeDate)) {
                perror("Error writing patient data to backup file");
                break;
            }
            current = current->next;
        }
        fflush(patientBackupFile);
        printf("Patient backup created: %s\n", patientBackupFilename);
        fclose(patientBackupFile);
    }
}

// Backup thread function: sleeps for a fixed interval and then calls the backup routine.
void *backupThreadFunction(void *arg) {
    const int BACKUP_INTERVAL = 120; // backup every two minutes
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

// Restore patient data from a backup file
void restorePatients(void) {
    char backupFilename[256];
    getValidString(backupFilename, sizeof(backupFilename) - 1,
                   "Enter backup file name for patients: ");
    FILE *fp = fopen(backupFilename, "rb");
    if (fp == NULL) {
        perror("Error opening patient backup file");
        return;
    }
    int count = 0;
    // Read the count of patient records.
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        perror("Error reading patient count from backup file");
        fclose(fp);
        return;
    }
    // Free the current patient list.
    while (head != NULL) {
        Patient *temp = head;
        head = head->next;
        free(temp);
    }
    // Read each patient record and add to the linked list.
    for (int i = 0; i < count; i++) {
        Patient *newPatient = malloc(sizeof(Patient));
        if (newPatient == NULL) {
            fprintf(stderr, "Error allocating memory for patient\n");
            break;
        }
        if (fread(&newPatient->id, sizeof(int), 1, fp) != 1 ||
            fread(newPatient->name, sizeof(char), sizeof(newPatient->name), fp) != sizeof(newPatient->name) ||
            fread(&newPatient->age, sizeof(int), 1, fp) != 1 ||
            fread(newPatient->diagnosis, sizeof(char), sizeof(newPatient->diagnosis), fp) != sizeof(newPatient->diagnosis) ||
            fread(&newPatient->roomNumber, sizeof(int), 1, fp) != 1 ||
            fread(newPatient->admissionDate, sizeof(char), sizeof(newPatient->admissionDate), fp) != sizeof(newPatient->admissionDate) ||
            fread(newPatient->dischargeDate, sizeof(char), sizeof(newPatient->dischargeDate), fp) != sizeof(newPatient->dischargeDate)) {
            perror("Error reading patient record from backup file");
            free(newPatient);
            break;
            }
        newPatient->next = NULL;
        // Append to linked list.
        if (head == NULL) {
            head = newPatient;
        } else {
            Patient *cur = head;
            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = newPatient;
        }
    }
    fclose(fp);
    printf("Restored %d patient record(s) from backup.\n", count);

    // Write the restored data back to the original patients file.
    savePatientsToFile(patientsFile);
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