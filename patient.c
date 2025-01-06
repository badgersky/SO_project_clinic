#include "patient.h"
#include "utils.h"
#include "sem.h"

void patient_routine(int* p_cnt, struct p_queue* p_q) {
    sem_wait(door);
    *p_cnt += 1;
    sem_post(door);

    sem_wait(reg_q_e);
    sem_wait(reg);

    int pid = getpid();
    p_q->ids[p_q->i] = pid;
    p_q->i += 1;

    sem_post(reg);
    sem_post(reg_q_f);

    sem_wait(door);
    *p_cnt -= 1;
    sem_post(door);
    sleep(1);
}

void create_patients(int* p_cnt, struct p_queue* p_q) {
    for (int i = 0; i < MAX_P; i++) {
        sleep(1);
        create_patient(p_cnt, p_q);
    }
}

void create_patient(int* p_cnt, struct p_queue* p_q) {
    pid_t p = fork();

    if (p < 0) {
        perror("fork");
    }
    if (p == 0) {
        patient_routine(p_cnt, p_q);
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {
            perror("wait");
        }
    }
}