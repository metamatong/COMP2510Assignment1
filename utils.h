#ifndef UTILS_H
#define UTILS_H
#include "patient.h"

int  getValidInt(int minVal, int maxVal, const char *promptMsg);
void getValidString(char *dest, size_t maxLen, const char *promptMsg);
void toLowerCase(char *str);

// New functions for loading file data into memory.
void loadDoctorSchedule(void);
void loadPatients(void);

// Backup functions
void backupRoutine(void);
void *backupThreadFunction(void *arg);

// Restore functions
void restoreDoctorSchedule(void);
void restorePatients(void);
void restoreBackup(void);

// Read and Write Patients
int readPatientFromFile(FILE *fp, Patient *p);
int writePatientToFile(FILE *fp, const Patient *p);


#endif