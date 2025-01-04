#include "patient.h"
#include "utils.h"

void patient_routine() {
    for (int i = 0; i < 5; i++) {
        sleep(1);
        printf("patient %d\n", getpid());
    }

    exit(0);
}

void create_patients() {
    for (int i = 0; i < MAX_P; i++) {
        sleep(1);
        create_patient();
    }
}

void create_patient() {
    pid_t p = fork();

    if (p < 0) {
        perror("fork");
    }
    if (p == 0) {
        patient_routine();
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {
            perror("wait");
        }
    }
}