#ifndef DOCTOR_H
#define DOCTOR_H

#define DAY_COUNT    7
#define SHIFT_COUNT  3
#define MAX_DOC_NAME 50
#define MAX_DAY_NAME 10
#define MAX_SHIFT_NAME 10

typedef struct {
    char docName[50];
} Doctor;

typedef struct {
    char docName[MAX_DOC_NAME];
    char dayNames[DAY_COUNT][MAX_DAY_NAME];
    char shiftNames[SHIFT_COUNT][MAX_SHIFT_NAME];
} Schedule;

// prototypes for doctor-related functions
void manageDoctSched(void);
void assignDoctorShift(void);
void viewWeeklySchedule(void);

#endif