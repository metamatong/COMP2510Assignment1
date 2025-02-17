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

// Structure to store doctor information for schedule
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

/**
 * Displays the main menu and handles user choices.
 * The menu allows operations like adding, displaying, searching,
 * deleting patients, managing doctor schedules, and exiting.
 */
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

/**
 * Adds a new patient to the system.
 * Prompts the user for patient details and stores the patient
 * in the patients array.
 */
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

/**
 * Generates a unique patient ID.
 * Iterates through possible candidate IDs starting at 1
 * until it finds an ID not already assigned to a patient.
 *
 * @return A unique patient ID.
 */
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

/**
 * Displays all patients currently checked in.
 * Iterates over the patients array and prints out each patient's details.
 */
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

/**
 * Searches for a patient either by ID or by name.
 * Provides a sub-menu to choose the search method, and displays
 * patient information if found.
 */
void searchPatient()
{
    while (1) {
        printf("\n-- Search for a Patient --\n");
        printf("1. By ID\n");
        printf("2. By Name\n");
        printf("3. Return to Main Menu\n");

        // Allow user to pick 1, 2, or 3
        int choice = getValidInt(1, 3, "\nEnter choice (1-3): \n");

        // If user picks '3', just go back to main menu
        if (choice == 3) {
            return;
        }

        switch (choice)
        {
            case 1: {
                // Keep asking for an ID until found or user cancels
                while (1) {
                    int inputId = getValidInt(-1, 9999,
                        "\nEnter the ID of the patient (or -1 to cancel): \n");

                    // A sentinel value (-1) to quit the loop
                    if (inputId == -1) {
                        break; // go back to choice menu
                    }

                    int found = 0;
                    for (int i = 0; i < patientCount; i++) {
                        if (patients[i].id == inputId) {
                            printf("\nPatient Found:\n");
                            printf("Patient ID: %d\n", patients[i].id);
                            printf("Patient Name: %s\n", patients[i].name);
                            printf("Patient Age: %d\n", patients[i].age);
                            printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                            printf("Patient Room No.: %d\n\n", patients[i].roomNumber);

                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        printf("Error: Patient Not Found! Please try again.\n");
                    } else {
                        // Once found, we can break out of the inner loop
                        break;
                    }
                }
                break;
            }

            case 2: {
                // Keep asking for a name until found or user types 'cancel'
                while (1) {
                    char inputName[100];
                    getValidString(inputName, 99,
                        "\nEnter the patient's name (or type 'cancel' to go back): ");

                    // If user wants to cancel name search
                    if (strcmp(inputName, "cancel") == 0) {
                        break; // go back to choice menu
                    }

                    // Convert inputName to lowercase in-place
                    for (size_t k = 0; k < strlen(inputName); k++) {
                        if (inputName[k] >= 'A' && inputName[k] <= 'Z') {
                            inputName[k] = inputName[k] + ('a' - 'A');
                        }
                    }

                    int found = 0;
                    for (int i = 0; i < patientCount; i++) {
                        // Make a temp copy of this patient's name
                        char tempName[100];
                        strcpy(tempName, patients[i].name);

                        // Convert tempName to lowercase
                        for (size_t j = 0; j < strlen(tempName); j++) {
                            if (tempName[j] >= 'A' && tempName[j] <= 'Z') {
                                tempName[j] = tempName[j] + ('a' - 'A');
                            }
                        }

                        // Compare the lowercase versions
                        if (strcmp(inputName, tempName) == 0) {
                            printf("\nPatient Found:\n");
                            printf("Patient ID: %d\n", patients[i].id);
                            printf("Patient Name: %s\n", patients[i].name);
                            printf("Patient Age: %d\n", patients[i].age);
                            printf("Patient Diagnosis: %s\n", patients[i].diagnosis);
                            printf("Patient Room No.: %d\n\n", patients[i].roomNumber);

                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        printf("Error: Patient Not Found! Please try again.\n");
                    } else {
                        // Once found, break out of the inner loop
                        break;
                    }
                }
                break;
            }
        } // end switch
    } // end while(1)
}

/**
 * Deletes a patient from the system.
 * Prompts the user for a patient ID and removes the corresponding record
 * from the patients array if it exists.
 */
void deletePatient()
{
    if (patientCount == 0) {
        printf("No patients to delete.\n");
        return;
    }
    while (1) {
        int id = getValidInt(1, 9999, "Please enter Patient ID to delete (or 0 to cancel): ");
        if (id == 0) {
            // user canceled
            return;
        }

        int index = -1;
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].id == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            printf("Error: Patient with ID %d not found. Please try again.\n", id);
        }
        else {
            // shift patients array to “delete” the record
            for (int i = index; i < patientCount - 1; i++) {
                patients[i] = patients[i + 1];
            }
            patientCount--;
            printf("Patient with ID %d deleted successfully.\n", id);
            break; // exit loop
        }
    }
}

/**
 * Manages the doctor schedule by displaying a sub-menu.
 * Options include assigning a doctor to a shift or viewing the weekly schedule.
 */
void manageDoctSched() {
  int choice;
  do {
    printf("\n=== Doctor Schedule Management ===\n");
    printf("1. Assign Doctor to a Shift\n");
    printf("2. View Weekly Schedule\n");
    printf("3. Return to Previous Menu\n");

    choice = getValidInt(1, 3, "\nEnter your choice (1-3): \n");

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


/**
 * Assigns a doctor to a specific shift on a specific day.
 * Checks for any existing assignment before assigning a new doctor.
 */
void assignDoctorShift() {
    const char* dayNames[DAY_COUNT] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    const char* shiftNames[SHIFT_COUNT] = {
        "Morning", "Afternoon", "Evening"
    };

    char docName[50];

    // We'll loop until the user successfully assigns a doctor
    // or decides to cancel.
    while (1) {
        printf("\n-- Assign Doctor to a Shift --\n");

        // If you want to allow user to "cancel,"
        // you can broaden getValidInt's minimum to -1 and check for that:
        int day = getValidInt(-1, DAY_COUNT - 1,
            "Enter day of the week [0=Sunday..6=Saturday] or -1 to cancel: ");
        if (day == -1) {
            printf("Assignment cancelled.\n");
            return; // go back to previous menu
        }

        int shift = getValidInt(0, SHIFT_COUNT - 1,
            "Enter shift [0=Morning, 1=Afternoon, 2=Evening]: ");

        getValidString(docName, 49, "Please enter the Doctor's name: ");

        // Check if there's already a doctor assigned
        if (strlen(doctors[day][shift].docName) != 0) {
            printf("Error: A doctor is already assigned for %s (%s).\n",
                dayNames[day], shiftNames[shift]);
            printf("Please try again.\n\n");
            // Loop continues, re-prompting for day/shift
        } else {
            // Assign the new doctor to that day/shift
            strcpy(doctors[day][shift].docName, docName);
            printf("Doctor '%s' assigned to %s (%s) successfully!\n",
                docName, dayNames[day], shiftNames[shift]);
            break; // we are done, so break out of the while loop
        }
    }
}

/**
 * Displays the weekly doctor schedule.
 * Iterates through each day and shift, printing the assigned doctor's name
 * or a message if no doctor is assigned.
 */
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
 * Prompts the user with a message and reads an integer from standard input.
 * The integer must be within the specified range [minVal, maxVal]. It will
 * re-prompt on invalid input until a valid integer is entered.
 *
 * @param minVal The minimum allowed integer value.
 * @param maxVal The maximum allowed integer value.
 * @param promptMsg The message to display when prompting the user.
 * @return A valid integer within the specified range.
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
 * Reads a line of input from the user into the provided destination buffer.
 * Ensures that the final string is at most maxLen characters long (excluding the '\0'),
 * flushes any excess characters, removes the trailing newline, and ensures that the
 * input is not empty or composed solely of whitespace.
 *
 * @param dest The destination buffer where the input string will be stored.
 * @param maxLen The maximum allowed length for the input (excluding '\0').
 * @param promptMsg The message to display when prompting the user.
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