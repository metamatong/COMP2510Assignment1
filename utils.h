#ifndef UTILS_H
#define UTILS_H

#include <stddef.h> // for size_t

int  getValidInt(int minVal, int maxVal, const char *promptMsg);
void getValidString(char *dest, size_t maxLen, const char *promptMsg);

#endif