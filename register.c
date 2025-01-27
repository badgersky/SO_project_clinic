#include "register.h"

void sigusr2_reg_handler(int sig) {
    exit(0);
}

void register_routine(int i) {
    signal(SIGUSR2, sigusr2_reg_handler);
    int done = 0;
    int data, ret;

    close(register_patient[i][0]);
    close(patient_register[i][1]);

    for (int j = 0; j < DR_NUM; j++) {
        close(patient_doctor[j][0]);
        close(patient_doctor[j][1]);
        close(doctor_patient[j][0]);
        close(doctor_patient[j][1]);
    }

    do {
        open_close_register();
        process_patient(i);
        // sleep(1);
    } while(!done);
    exit(0);
}

void process_patient(int i) {
    int dr_id, reg_resp = 0;
    pid_t p_pid;

    // printf("before reg_pipe_lock\n");
    sem_wait(reg_pipe_lock[i]);
    // printf("register reading p pid\n");
    if (read(patient_register[i][0], &p_pid, sizeof(int)) < 0) {
        perror("read reg");
        exit(3);
    }
    // printf("register got p pid %d\n", p_pid);
    // printf("register reading dr id\n");
    if (read(patient_register[i][0], &dr_id, sizeof(int)) < 0) {
        perror("read reg");
        exit(3);
    }
    // printf("register got dr id\n");
    sem_wait(cs_lock);
    if (*clinic_state == 0) {
        sem_post(cs_lock);
        char* msg = (char*) malloc(sizeof(char) * BUFFER);
        sprintf(msg, "%d - skierowanie do %d - wystawiła rejestracja\n", p_pid, dr_id);
        sem_wait(report_lock);
        write_report(msg);
        sem_post(report_lock);
        free(msg);
    } else {
        sem_post(cs_lock);
        sem_wait(drq_lock[dr_id]);
        if (dr_p_cnt[dr_id] < dr_limits[dr_id]) {
            dr_p_cnt[dr_id] += 1;
            reg_resp = 1;
            printf("register %d %d registering patient %d to doctor %d\n", i + 1, getpid(), p_pid, dr_id);
        } else {
            printf("no free visit hours to doctor %d\n", dr_id);
        }
        sem_post(drq_lock[dr_id]);
    }
    // printf("register writing to patient %d\n", p_pid);
    if (write(register_patient[i][1], &reg_resp, sizeof(int)) < 0) {
        perror("write reg");
        exit(3);
    }
    // printf("register finished writing to patient %d\n", p_pid);
    sem_post(reg_pipe_lock[i]);
}

void open_close_register() {
    sem_wait(rq_lock);
    if (*desks_open == 1 && get_sem_value(rq_capacity) < MAX_QUEUE / 2) {
        printf("second register open\n");
        *desks_open += 1;
    }
    if (*desks_open == 2 && get_sem_value(rq_capacity) > MAX_QUEUE / 3) {
        int sem_val;
        printf("second register closed\n");
        *desks_open -= 1;
    }
    sem_post(rq_lock);
}

void create_registers() {
    pid_t pid;

    for (int i = 0; i < REG_NUM; i++) {
        pid = fork();
        if (pid < 0) {
            return perror("fork");
            exit(1);
        }
        if (pid == 0) {
            register_routine(i);
        } else {
            sem_wait(p_lock);
            *r_pid = pid;
            sem_post(p_lock);
        }
    }
}

void wait_registers() { 
    if (wait(0) < 0) {
        perror("wait");
        exit(1);
    }
}