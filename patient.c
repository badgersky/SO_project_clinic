#include "patient.h"
#include "utils.h"
#include "sem.h"

void patient_routine() {
    for (int i = 0; i < 5; i++) {
        sleep(1);
//        printf("patient %d\n", getpid());
    }

    exit(0);
}

void create_patients(int* p_cnt) {
    for (int i = 0; i < MAX_P; i++) {
        sleep(1);
        sem_wait(door);
        create_patient();
        *p_cnt += 1;
        sem_post(door);
        printf("patients count %d\n", *p_cnt);
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