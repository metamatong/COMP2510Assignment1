#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include "doctor.h"
#include "patient.h"

extern FILE *scheduleFile; // Global file pointer for doctor schedule file
extern FILE *patientsFile; // Global file pointer for patient file

#define MAX_SCHEDULES (DAY_COUNT * SHIFT_COUNT)
extern Schedule scheduleList[MAX_SCHEDULES];
extern int scheduleCount;

extern Patient *head;


#endif
