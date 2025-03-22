#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"
#include "patient.h"

int main(void) {

    patients = malloc(INITIAL_PATIENT_CAPACITY * sizeof(Patient));
    if (patients == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    menu();

    free(patients);
    return 0;
}
