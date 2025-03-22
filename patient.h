#ifndef PATIENT_H
#define PATIENT_H

#include "hospital.h"

typedef struct {
    int id;
    char name[100];
    int age;
    char diagnosis[200];
    int roomNumber;
} Patient;

// Expose extern variables (defined in patient.c)
extern int patientCount;
extern int allocatedPatients;
extern Patient *patients;

// Prototypes for patient functions
int  generatePatientID(void);
void addPatient(void);
void displayPatient(void);
void searchPatient(void);
void deletePatient(void);

#endif