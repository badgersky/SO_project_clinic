#include "doctor.h"

int stop_treating = 0;

void sigusr1_handler(int sig) {
    stop_treating = 1;
}

void doctor_routine(int i) {
    int done = 0;
    signal(SIGUSR1, sigusr1_handler);
    for (int j = 0; j < DR_NUM; j++) {
        if (j != i) {
            close(patient_doctor[j][0]);
            close(patient_doctor[j][1]);
            close(doctor_patient[j][0]);
            close(doctor_patient[j][1]);
        }
    }

    close(patient_register[1]);
    close(register_patient[1]);
    close(patient_register[0]);
    close(register_patient[0]);

    close(patient_doctor[i][1]);
    close(doctor_patient[i][0]);

    do {
        // printf("Doctor %d\n", getpid());
        sem_wait(drq_cnt_lock[i]);
        if (drq_cnt[i] > 0) {
            sem_post(drq_cnt_lock[i]);
            examine_patient(i);
        } else {
            sem_post(drq_cnt_lock[i]);
        }

        if (stop_treating) {
            sem_wait(drq_lock[i]);
            dr_p_cnt[i] = dr_limits[i];
            sem_post(drq_lock[i]);
            done = 1;
            printf("Doctor %d is leaving\n", i);
        }
        
        sem_wait(cs_lock);
        sem_wait(rq_lock);
        sem_wait(p_cnt_lock);
        if (*clinic_state == 0 && *p_cnt == 0 && *rq_cnt == 0) {
            printf("Doctors %d leaving\n", i);
            done = 1;
        }
        sem_post(p_cnt_lock);
        sem_post(rq_lock);
        sem_post(cs_lock);
        sleep(1);
    } while(!done);

    exit(0);
}

void examine_patient(int dr_id) {
    pid_t p_pid;
    int r = rand() % 10;
    int spec_id, doc_resp = -1;

    sem_wait(dr_pipe_lock[dr_id]);

    // printf("Doctor %d before reading from patient\n", dr_id);
    if (read(patient_doctor[dr_id][0], &p_pid, sizeof(pid_t)) < 0) {
        perror("read doctor");
        exit(5);
    }
    if (read(patient_doctor[dr_id][0], &spec_id, sizeof(int)) < 0) {
        perror("read doctor");
        exit(5);
    }
    // printf("Doctor received spec id %d from patient %d\n", spec_id, p_pid);
    printf("Doctor %d %d examining patient %d\n", dr_id, getpid(), p_pid);

    if (stop_treating) {
        char* msg = (char*) malloc(sizeof(char) * BUFFER);
        sprintf(msg, "%d - skierowanie do %d - koniec pracy doktora\n", p_pid, dr_id);
        sem_wait(report_lock);
        write_report(msg);
        sem_post(report_lock);
        free(msg);
    } else {
        if (spec_id >= 0 && spec_id <= 3) {
            sem_wait(drq_lock[dr_id]);
            if (dr_p_cnt[dr_id] < dr_limits[dr_id]) {
                dr_p_cnt[dr_id] += 1;
                doc_resp = spec_id;
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
    }

    if (write(doctor_patient[dr_id][1], &doc_resp, sizeof(int)) < 0) {
        perror("write doctor");
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
        sem_wait(drq_lock[i]);
        if (dr_p_cnt[i] < dr_limits[i]) {
            full = 0;
        }
        sem_post(drq_lock[i]);
    }

    return full;
}