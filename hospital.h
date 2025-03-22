#ifndef HOSPITAL_H
#define HOSPITAL_H

#define INITIAL_PATIENT_CAPACITY 50
#define DAY_COUNT    7
#define SHIFT_COUNT  3

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