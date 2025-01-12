#include "register.h"

void register_routine() {
    int* desks_open = (int*) malloc(sizeof(int));
    *desks_open = 1;

    do {
        open_close_register(desks_open);
        process_patient();
        sleep(1);
    } while(1);

    free(desks_open);
    exit(0);
}

void process_patient() {
    int dr_id, reg_resp = 0;
    close(register_patient[0]);
    close(patient_register[1]);

    sem_wait(reg_pipe_lock);
    if (read(patient_register[0], &dr_id, sizeof(int)) < 0) {
        perror("read");
        exit(3);
    }

    sem_wait(drq_lock[dr_id]);
    if (dr_p_cnt[dr_id] < dr_limits[dr_id]) {
        dr_p_cnt[dr_id] += 1;
        reg_resp = 1;
        printf("registering patient to doctor %d\n", dr_id);
    } else {
        printf("no free visit hours to doctor %d\n", dr_id);
    }
    sem_post(drq_lock[dr_id]);

    if (write(register_patient[1], &reg_resp, sizeof(int)) < 0) {
        perror("write");
        exit(3);
    }
    sem_post(reg_pipe_lock);
}

void open_close_register(int* desks_open) {
    sem_wait(rq_lock);
    if (*desks_open == 1 && *rq_cnt > MAX_QUEUE / 2) {
        printf("second register open\n");
        sem_post(reg_queue);
        *desks_open += 1;
    }
    if (*desks_open == 2 && *rq_cnt < MAX_QUEUE / 3) {
        printf("second register closed\n");
        sem_wait(reg_queue);
        *desks_open -= 1;
    }
    printf("Number of patients in register queue: %d\n", *rq_cnt);
    sem_post(rq_lock);
}

void create_registers() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        return perror("fork");
        exit(1);
    }
    if (pid == 0) {
        register_routine();
    }
}

void wait_registers() { 
    if (wait(0) < 0) {
        perror("wait");
        exit(1);
    }
}