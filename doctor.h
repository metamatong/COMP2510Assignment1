#ifndef DOCTOR_H
#define DOCTOR_H

#define DAY_COUNT    7
#define SHIFT_COUNT  3

typedef struct {
    char docName[50];
} Doctor;

// reference to the doctors 2D array
extern Doctor doctors[DAY_COUNT][SHIFT_COUNT];

// prototypes for doctor-related functions
void manageDoctSched(void);
void assignDoctorShift(void);
void viewWeeklySchedule(void);

#endif