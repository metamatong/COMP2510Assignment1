#ifndef UTILS_H
#define UTILS_H

int  getValidInt(int minVal, int maxVal, const char *promptMsg);
void getValidString(char *dest, size_t maxLen, const char *promptMsg);
void toLowerCase(char *str);

// New functions for loading file data into memory.
void loadDoctorSchedule(void);
void loadPatients(void);

#endif