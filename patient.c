#include "patient.h"

void patient_routine(int i) {
    enter_clinic();

    sem_wait(reg_queue);
    printf("patient %d registering\n", getpid());
    sleep(2);
    sem_post(reg_queue);
    
    leave_clinic();
    exit(0);
}

void enter_clinic() {
    sem_wait(clinic_capacity);

    sem_wait(rq_lock);
    *rq_cnt += 1;
    sem_post(rq_lock);
}

void leave_clinic() {
    sem_wait(rq_lock);
    *rq_cnt -= 1;
    sem_post(rq_lock);

    sem_post(clinic_capacity);
}

void create_patients() {
    pid_t pid;
    for (int i = 0; i < MAX_P; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(3);
        }
        if (pid == 0) {
            patient_routine(i);
        }
        sleep(1);
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {
            perror("wait");
            exit(3);
        }
    }
}