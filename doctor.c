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
    int r = rand() % 10;
    int spec_id = -1;

    if (r < 2) {
        if (dr_id == 4 || dr_id == 5) {
            spec_id = rand() % DR_NUM - 2;
        }
    }
    
    close(patient_doctor[dr_id][1]);
    close(doctor_patient[dr_id][0]);

    sem_wait(dr_pipe_lock[dr_id]);

    if (read(patient_doctor[dr_id][0], &p_pid, sizeof(pid_t)) < 0) {
        perror("read");
        exit(5);
    }

    printf("Doctor %d examining patient %d\n", dr_id, p_pid);
    if (spec_id >= 0 && spec_id <= 3) {
        sem_wait(drq_lock[dr_id]);
        if (dr_p_cnt[dr_id] < dr_limits[dr_id]) {
            dr_p_cnt[dr_id] += 1;
            printf("Doctor %d registering patient %d to doctor %d\n", dr_id, p_pid, spec_id);
        } else {
            char* msg = (char*) malloc(sizeof(char) * BUFFER);
            sprintf(msg, "%d - skierowanie do %d - wystawił doktor %d\n", p_pid, spec_id, dr_id);
            sem_wait(report_lock);
            write_report(msg);
            sem_post(report_lock);
            free(msg);
        }
        sem_post(drq_lock[dr_id]);
    } 
    
    if (write(doctor_patient[dr_id][1], &spec_id, sizeof(int)) < 0) {
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

int check_limits() {
    int full = 1;
    for (int i = 0; i < DR_NUM; i++) {
        if (dr_p_cnt[i] < dr_limits[i]) {
            full = 0;
        }
    }

    return full;
}