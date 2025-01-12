#include "doctor.h"

void doctor_routine(int i) {
    do {
        // printf("doctor %d\n", i);
        examine_patient(i);
        sleep(3);
    } while(1);

    exit(0);
}

void examine_patient(int dr_id) {
    pid_t p_pid;
    int doc_resp = -1;

    close(patient_doctor[dr_id][1]);
    close(doctor_patient[dr_id][0]);

    sem_wait(dr_pipe_lock[dr_id]);

    if (read(patient_doctor[dr_id][0], &p_pid, sizeof(pid_t)) < 0) {
        perror("read");
        exit(5);
    }
    printf("Doctor %d examining patient %d\n", dr_id, p_pid);
    if (write(doctor_patient[dr_id][1], &doc_resp, sizeof(int)) < 0) {
        perror("write");
        exit(5);
    }

    sem_post(dr_pipe_lock[dr_id]);
}

void create_doctors() {
    pid_t pid;
    for (int i = 0; i < DR_NUM; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(5);
        }
        if (pid == 0) {
            srand(time(0) + i);
            doctor_routine(i);
        }
    }
}

void wait_doctors() {
    for (int i = 0; i < DR_NUM; i++) {
        if (wait(0) < 0) {
            perror("wait");
            exit(5);
        }
    }
}

int get_rand_id() {
    int r = rand() % 10;
    int dr_id;
    if (r < 6) {
        dr_id = 4 + (rand() % 2);
    }
    else {
        dr_id = rand() % (DR_NUM - 2);
    }
    
    return dr_id;
}