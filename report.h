#ifndef REPORT_H
#define REPORT_H

void generatePatientSummaryReport(void);
void updateCountsForPatient(PatientNode *p, time_t now, int *dayCount, int *weekCount, int *monthCount);
void countAdmissionPeriods(int *dayCount, int *weekCount, int *monthCount);
void generateDoctorUtilizationReport(void);
void generateRoomUsageReport(void);
void reportingMenu(void);

#define SECONDS_IN_DAY   86400
#define SECONDS_IN_WEEK  (7 * SECONDS_IN_DAY)
#define SECONDS_IN_MONTH (30 * SECONDS_IN_DAY)

#endif
