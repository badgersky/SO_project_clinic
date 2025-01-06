#include "patient.h"
#include "utils.h"
#include "sem.h"

void patient_routine(int* p_cnt) {
    do {
        sem_wait(door);
        *p_cnt += 1;
        sem_post(door);

        printf("patients count %d\n", *p_cnt);
        sleep(1);

        sem_wait(door);
        *p_cnt -= 1;
        sem_post(door);
    } while (1);
}

void create_patients(int* p_cnt) {
    for (int i = 0; i < MAX_P; i++) {
        sleep(1);
        create_patient(p_cnt);
    }
}

void create_patient(int* p_cnt) {
    pid_t p = fork();

    if (p < 0) {
        perror("fork");
    }
    if (p == 0) {
        patient_routine(p_cnt);
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {
            perror("wait");
        }
    }
}