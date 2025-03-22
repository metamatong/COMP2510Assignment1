#ifndef DOCTOR_H
#define DOCTOR_H

#define DAY_COUNT    7
#define SHIFT_COUNT  3
#define MAX_DOC_NAME 50

typedef struct {
    char docName[50];
} Doctor;

typedef struct {
    char docName[MAX_DOC_NAME];
    int day;    // 0=Sunday, …, 6=Saturday
    int shift;  // 0=Morning, 1=Afternoon, 2=Evening
} Schedule;

// prototypes for doctor-related functions
void manageDoctSched(void);
void assignDoctorShift(void);
void viewWeeklySchedule(void);
void saveSchedule(Schedule);

#endif