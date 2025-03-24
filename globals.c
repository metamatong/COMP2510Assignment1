#include "globals.h"

// Definitions for schedule file
FILE *scheduleFile = NULL;
Schedule scheduleList[MAX_SCHEDULES];
int scheduleCount = 0;

// Definitions for doctor file
FILE *patientsFile = NULL;
Patient *head = NULL;
Patient *dischargedHead = NULL;
