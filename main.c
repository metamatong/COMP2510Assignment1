#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_PATIENTS 50

typedef enum {
  SUNDAY,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  DAY_COUNT
} Day;

typedef enum {
    MORNING,
    AFTERNOON,
    EVENING,
    SHIFT_COUNT
} Shift;

int generatePatientID();
void menu();
void addPatient();
void displayPatient();
void searchPatient();
void deletePatient();
void manageDoctSched();
void assignDoctorShift();
void viewWeeklySchedule();
int getValidInt(int minVal, int maxVal, const char *promptMsg);
void getValidString(char *dest, size_t maxLen, const char *promptMsg);

// Structure to store patient information
typedef struct {
    int id;
    char name[100];
    int age;
    char diagnosis[200];
    int roomNumber;
} Patient;

typedef struct {
  char docName[50];
} Doctor;

int patientCount = 0;
Patient patients[MAX_PATIENTS];
Doctor doctors[DAY_COUNT][SHIFT_COUNT];

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while (1) {
        printf("\n=== Hospital Management System ===\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Exit\n");

        choice = getValidInt(1, 6, "\nEnter your choice (1-6): \n");

        switch (choice)
        {
            case 1:
                addPatient();
            break;
            case 2:
                displayPatient();
            break;
            case 3:
                searchPatient();
            break;
            case 4:
                deletePatient();
            break;
            case 5:
                manageDoctSched();
            break;
            case 6:
                return;
        }
    }
}

void addPatient()
{
    if (patientCount >= MAX_PATIENTS) {
        printf("Error: Too many patients!\n");
        return;
    }
    Patient patient;
    patient.id = generatePatientID();

    // We are using this helper function that does not return string because we cannot dynamically assign
    // arrays to fixed size array in a struct in the runtime.
    getValidString(patient.name, 99, "Enter patient name: ");

    // However, we can do that with int. So we make this helper function return an integer and assign it here.
    patient.age = getValidInt(0, 200, "\nEnter patient age (0-200): \n");

    getValidString(patient.diagnosis, 199, "Enter patient diagnosis: ");

    patient.roomNumber = getValidInt(0, 9999, "\nEnter patient room number (0-9999): \n");

    patients[patientCount] = patient;
    patientCount++;

    printf("Patient added successfully ID: %d\n", patient.id);
}

int generatePatientID()
{
    int candidate = 1;
    while (1) {
        int found = 0;
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].id == candidate) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return candidate;
        }
        candidate++;
    }
}

void displayPatient()
{
    printf("=== Every Patient currently checked in ===\n");
    if (patientCount == 0) {
        printf("Error: No patients right now!\n");
        return;
    }

    for (int i = 0; i < patientCount; i++) {
        printf("Patient ID: %d\n", patients[i].id);
        printf("Patient Name: %s\n", patients[i].name);
        printf("Patient Age: %d\n", patients[i].age);
        printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
        printf("Patient Room No.: %d\n", patients[i].roomNumber);
        printf("\n");
    }
}

void searchPatient()
{
    int choice;
    printf("Enter how you would like to search patient: \n");
    printf("1. by ID\n");
    printf("2. by name\n");
    choice = getValidInt(1, 2, "");

    switch (choice)
    {
        case 1: {
            int inputId;
            inputId = getValidInt(0, 9999, "Enter the ID of the patient you are looking for: ");

            int found = 0;
            for (int i = 0; i < patientCount; i++) {
                if (inputId == patients[i].id) {
                    printf("\nPatient Found:\n");
                    printf("Patient ID: %d\n", patients[i].id);
                    printf("Patient Name: %s\n", patients[i].name);
                    printf("Patient Age: %d\n", patients[i].age);
                    printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                    printf("Patient Room No.: %d\n", patients[i].roomNumber);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Error: Patient Not Found!\n");
            }
            break;
        }

        case 2: {
            char inputName[100];
            getValidString(inputName, 99, "Enter the name of the patient you are looking for: ");
            int found = 0;
            for (int i = 0; i < patientCount; i++) {

                // We first convert both string to lowercase [a-z]
                for (size_t i = 0; i < strlen(inputName); i++) {
                    if (inputName[i] >= 'A' && inputName[i] <= 'Z') {
                        // Move from 'A'..'Z' to 'a'..'z'
                        inputName[i] = inputName[i] + ('a' - 'A');
                    }
                }

                char tempName[100];
                strcpy(tempName, patients[i].name);

                for (size_t i = 0; i < strlen(tempName); i++) {
                    if (tempName[i] >= 'A' && tempName[i] <= 'Z') {
                        // Move from 'A'..'Z' to 'a'..'z'
                        tempName[i] = tempName[i] + ('a' - 'A');
                    }
                }

                if (strcmp(inputName, tempName) == 0) {
                    printf("\nPatient Found:\n");
                    printf("Patient ID: %d\n", patients[i].id);
                    printf("Patient Name: %s\n", patients[i].name);
                    printf("Patient Age: %d\n", patients[i].age);
                    printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                    printf("Patient Room No.: %d\n", patients[i].roomNumber);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Error: Patient Not Found!\n");
            }
            break;
        }
        default:
            printf("Invalid Choice!\n");
    }
}

void deletePatient()
{
    if (patientCount == 0) {
        printf("No patients to delete.\n");
    }

    int id;
    int index = -1;

    printf("Please enter Patient ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1)
        {
        printf("Error: Patient with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < patientCount - 1; i++)
        {
        patients[i] = patients[i + 1];
    }

    patientCount--;
    printf("Patient with ID %d deleted successfully.\n", id);

}

void manageDoctSched() {

  int choice;
  do {
    printf("\n=== Doctor Schedule Management ===\n");
    printf("1. Assign Doctor to a Shift\n");
    printf("2. View Weekly Schedule\n");
    printf("3. Return to Previous Menu\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            assignDoctorShift();
            break;
        case 2:
            viewWeeklySchedule();
            break;
        case 3:
          printf("Returning to main menu...\n");
          break;
        default:
          printf("Invalid Choice! Please try again.\n");

    }
  } while (choice != 3);
}

void assignDoctorShift() {

    const char* dayNames[DAY_COUNT] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    const char* shiftNames[SHIFT_COUNT] = {
        "Morning", "Afternoon", "Evening"
    };

    int day, shift;
    char docName[50];

    printf("\nEnter day of the week [0=Sunday, 6=Saturday]: ");
    scanf("%d", &day);
    if(day < 0 || day >= DAY_COUNT) {
      printf("Error: Invalid day!\n");
      return;
    }

    printf("Enter shift [0=Morning, 1=Afternoon, 2=Evening] ");
    scanf("%d", &shift);
    if(shift < 0 || shift >= SHIFT_COUNT) {
      printf("Error: Invalid shift!\n");
      return;
    }

    printf("Please enter the Doctor's name: ");
    scanf(" %[^\n]", docName);

    if(strlen(doctors[day][shift].docName) != 0) {
      printf("Error: A doctor is already assigned for %s (%s).\n", dayNames[day], shiftNames[shift]);
    } else {
        strcpy(doctors[day][shift].docName, docName);
        printf( "Doctor '%s' assigned to %s (%s) successfully!\n", docName, dayNames[day], shiftNames[shift]);
    }

}

void viewWeeklySchedule() {
  const char* dayNames[DAY_COUNT] = {
    "Sunday","Monday","Tuesday","Wednesday", "Thursday","Friday","Saturday"
     };

  const char* shiftNames[SHIFT_COUNT] = {
    "Morning","Afternoon","Evening"
  };

  printf("\n=== Weekly Doctor Schedule ===\n");
  for (int i = 0; i < DAY_COUNT; i++) {
    for(int j = 0; j < SHIFT_COUNT; j++) {
      if(strlen(doctors[i][j].docName) == 0) {
        printf("%-9s (%-9s): [No Doctor Assigned]\n", dayNames[i], shiftNames[j]);
      } else {
        printf("%-9s (%-9s) : Dr. %s\n", dayNames[i], shiftNames[j], doctors[i][j].docName);
      }
    }
    printf("\n");
  }
}


/**
 * Prompts the user with `promptMsg`, reads input from stdin,
 * and returns a valid integer in the range [minVal..maxVal].
 * On invalid input (non-integer, out-of-range, etc.), it re-prompts
 * until the user enters a valid integer.
 */
int getValidInt(int minVal, int maxVal, const char *promptMsg) {
    char buffer[100];

    while (1) {
        // Show the prompt
        printf("%s", promptMsg);

        // 1) Read a line of input using fgets
        // This is to deal with float inputs wrongly parsed as integers
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            // If reading fails (EOF or error), prompt again
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // 2) Check if the input line was longer than our buffer
        //    If there's no '\n' in buffer, we didn't get the entire line
        //    and must flush the remainder.
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            // Flush until we see a newline or reach EOF
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* discard */
            }
        } else {
            // Remove the trailing newline for convenience
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        // 3) Convert the string to a long
        char *endptr;
        long num = strtol(buffer, &endptr, 10);

        // 4) If endptr didn't reach the end, we have leftover non-numeric characters
        if (*endptr != '\0') {
            printf("Invalid input. Please enter an integer.\n");
            continue;  // prompt again
        }

        // 5) Check the range
        if (num < minVal || num > maxVal) {
            printf("Out of range. Enter a number between %d and %d.\n",
                   minVal, maxVal);
            continue;
        }

        // 6) If valid, return the integer
        return (int)num;
    }
}

/**
 * Reads a line of input from the user and stores it into 'dest', ensuring:
 *  - The final string is at most 'maxLen' characters long (excluding '\0').
 *  - No leftover characters remain in stdin if the user typed a very long line.
 *  - Trailing newline is removed.
 *  - Rejects empty or whitespace-only input (spaces, tabs, carriage returns).
 *  - Reprompts if the user input exceeds 'maxLen' characters or is invalid.
 */
void getValidString(char *dest, size_t maxLen, const char *promptMsg)
{
    char buffer[512]; // temp buffer to detect overflow & flush excess

    while (1) {
        printf("%s", promptMsg);

        // Attempt to read a line (including the newline if it fits)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // If EOF or an error occurs, prompt again.
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // If we do not find '\n' in buffer, we must flush leftover chars.
        if (!strchr(buffer, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* discard */
            }
        } else {
            // Replace the newline with a null terminator
            buffer[strcspn(buffer, "\n")] = '\0';
        }

        // Check length of what we read
        size_t length = strlen(buffer);

        // 1. Reject empty input
        if (length == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        // 2. Check if the string is all whitespace (spaces, tabs, carriage returns, etc.)
        int allWhitespace = 1;
        for (size_t i = 0; i < length; i++) {
            char c = buffer[i];
            // Only treat ' ', '\t', '\r' as whitespace here
            // (You can extend this list as needed, e.g. '\v', '\f' if desired)
            if (c != ' ' && c != '\t' && c != '\r') {
                allWhitespace = 0;
                break;
            }
        }
        if (allWhitespace) {
            printf("Input cannot be whitespace only.\n");
            continue;
        }

        // 3. Check if user typed more than allowed length
        if (length > maxLen) {
            printf("Input too long! Maximum allowed is %zu characters.\n", maxLen);
            continue;
        }

        // If we get here, it's valid length and not empty/whitespace only
        strncpy(dest, buffer, maxLen + 1);
        dest[maxLen] = '\0'; // ensure null termination

        break;  // success, exit loop
    }
}