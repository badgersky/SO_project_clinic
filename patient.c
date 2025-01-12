#include "patient.h"

void patient_routine(int i) {
    int reg_resp;
    enter_clinic();

    sem_wait(reg_queue);
    reg_resp = patient_registration();
    sem_post(reg_queue);
    
    leave_queue();
    leave_clinic();
    exit(0);
}

int patient_registration() {
    srand(getpid());
    printf("patient %d registering\n", getpid());
    int reg_resp, dr_id;
    dr_id = get_rand_id();

    close(patient_register[0]);
    close(register_patient[1]);

    if (write(patient_register[1], &dr_id, sizeof(int)) < 0) {
        perror("write");
        exit(3);
    }
    if (read(register_patient[0], &reg_resp, sizeof(int)) < 0) {
        perror("read");
        exit(3);
    }
    printf("patient %d, registers response: %d\n", getpid(), reg_resp);

    sleep(1);
    return reg_resp;
}

void enter_clinic() {
    sem_wait(clinic_capacity);

    sem_wait(rq_lock);
    *rq_cnt += 1;
    sem_post(rq_lock);
}

void leave_queue() {
    sem_wait(rq_lock);
    *rq_cnt -= 1;
    sem_post(rq_lock);
}

void leave_clinic() {
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