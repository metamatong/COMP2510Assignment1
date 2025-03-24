#ifndef PATIENT_H
#define PATIENT_H

typedef struct Patient {
    int id;
    char name[100];
    int age;
    char diagnosis[200];
    int roomNumber;
    char admissionDate[20];
    char dischargeDate[20];
    struct Patient *next;
} Patient;

extern Patient *head;

// Prototypes for patient functions
int  generatePatientID(void);
void addPatient(void);
void displayPatient(void);
void searchPatient(void);
void deletePatient(void);
void searchPatientByName(void);
void searchPatientByID(void);
void savePatientsToFile(FILE*);

#endif