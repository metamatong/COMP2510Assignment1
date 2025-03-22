#ifndef HOSPITAL_H
#define HOSPITAL_H

#define INITIAL_PATIENT_CAPACITY 50

typedef enum {
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} Day;

typedef enum {
    MORNING,
    AFTERNOON,
    EVENING
} Shift;

void menu(void);

#endif