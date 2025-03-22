#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int getValidInt(int minVal, int maxVal, const char *promptMsg) {
    char buffer[100];

    while (1) {
        printf("%s", promptMsg);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // If no newline found, flush remainder
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        char *endptr;
        long num = strtol(buffer, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }

        if (num < minVal || num > maxVal) {
            printf("Out of range. Enter a number between %d and %d.\n",
                   minVal, maxVal);
            continue;
        }
        return (int)num;
    }
}

void getValidString(char *dest, size_t maxLen, const char *promptMsg) {
    char buffer[512];

    while (1) {
        printf("%s", promptMsg);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        if (!strchr(buffer, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        size_t length = strlen(buffer);

        if (length == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        int allWhitespace = 1;
        for (size_t i = 0; i < length; i++) {
            if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r') {
                allWhitespace = 0;
                break;
            }
        }
        if (allWhitespace) {
            printf("Input cannot be whitespace only.\n");
            continue;
        }

        if (length > maxLen) {
            printf("Input too long! Maximum allowed is %zu characters.\n", maxLen);
            continue;
        }

        strncpy(dest, buffer, maxLen + 1);
        dest[maxLen] = '\0';
        break;
    }
}

void toLowerCase(char *str) {
    for (; *str; ++str) {
        *str = tolower(*str);
    }
}