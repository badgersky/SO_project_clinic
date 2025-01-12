#include "patient.h"

void patient_routine(int i) {
    srand(getpid());
    int reg_resp = 0, dr_id;
    dr_id = get_rand_id();
    enter_clinic();

    sem_wait(reg_queue);
    reg_resp = patient_registration(dr_id);
    sem_post(reg_queue);
    
    leave_queue();

    if (reg_resp == 0) {
        leave_clinic();
    } else {
        sem_wait(dr_queue[dr_id]);
        go_to_doc(dr_id);
        sem_post(dr_queue[dr_id]);
    }

    leave_clinic();
    exit(0);
}

void go_to_doc(int dr_id) {
    printf("patient %d going to doctor %d\n", getpid(), dr_id);
}

int patient_registration(int dr_id) {
    printf("patient %d registering\n", getpid());
    int reg_resp;

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
    // printf("patient %d, registers response: %d\n", getpid(), reg_resp);

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
    printf("patient %d leaving clinic\n", getpid());
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