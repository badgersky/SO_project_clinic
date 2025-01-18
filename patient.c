#include "patient.h"

void* child_routine(void* arg) {
    printf("child patient, parent's id: %d\n", getpid());
    pthread_exit(0);
}

int is_minor() {
    int r = rand() % 10;

    if (r < 2) {
        return 1;
    } else {
        return 0;
    }
}

void p_sigusr2_handler(int sig) {
    printf("patient %d received signal %d\n", getpid(), sig);
    leave_queue();
    leave_clinic();
}

void patient_routine(int i) {
    pthread_t ch_id;
    signal(SIGUSR2, p_sigusr2_handler);
    sem_wait(pids->pid_lock);
    pids->pids[pids->count] = getpid();
    pids->count += 1; 
    // printf("updated pids\n");
    sem_post(pids->pid_lock);

    int m = is_minor();

    if (m) {
        if (pthread_create(&ch_id, NULL, &child_routine, NULL) != 0) {
            perror("pthread_create");
            exit(3);
        }
        pthread_detach(ch_id);
    }

    int reg_resp = 0, dr_id, doc_resp1 = -1, doc_resp2 = -1;
    dr_id = get_rand_id();

    sem_wait(cs_lock);
    if (*clinic_state == 1) {
        sem_post(cs_lock);
        enter_clinic();

        sem_wait(reg_queue);
        printf("patient %d goind to registration\n", getpid());
        reg_resp = patient_registration(dr_id);
        sem_post(reg_queue);

        leave_queue();
            
        if (reg_resp > 0) {
            sem_wait(dr_queue[dr_id]);
            printf("patient %d going to doc %d\n", getpid(), dr_id);
            doc_resp1 = go_to_doc(dr_id);
            sem_post(dr_queue[dr_id]);
        } else if (reg_resp == 0) {
            leave_clinic();
        }

        if (doc_resp1 >= 0) {
            sem_wait(dr_queue[doc_resp1]);
            printf("patient %d going to doc %d\n", getpid(), doc_resp1);
            doc_resp2 = go_to_doc(dr_id);
            sem_post(dr_queue[doc_resp1]);
        } else if (doc_resp1 == -1) {
            leave_clinic();
        }

        if (doc_resp2 == -1) {
            leave_clinic();
        }
    } else {
        sem_post(cs_lock);
        exit(0);
    }
}

int go_to_doc(int dr_id) {
    sem_wait(drq_cnt_lock[dr_id]);
    drq_cnt[dr_id] += 1;
    sem_post(drq_cnt_lock[dr_id]);
    int doc_resp;
    pid_t pid = getpid();

    close(patient_doctor[dr_id][0]);
    close(doctor_patient[dr_id][1]);

    printf("patient %d writing to doctor %d\n", getpid(), dr_id);
    if (write(patient_doctor[dr_id][1], &pid, sizeof(pid_t)) < 0) {
        perror("write patient doc");
        exit(3);
    }

    printf("patient %d reading from doctor %d\n", getpid(), dr_id);
    if (read(doctor_patient[dr_id][0], &doc_resp, sizeof(int)) < 0) {
        perror("read patient doc");
        exit(3);
    }
    printf("Patient %d received response %d from doctor %d\n", pid, doc_resp, dr_id);

    sem_wait(drq_cnt_lock[dr_id]);
    drq_cnt[dr_id] -= 1;
    sem_post(drq_cnt_lock[dr_id]);
    
    if (doc_resp < 0) {
        close(patient_doctor[dr_id][1]);
        close(doctor_patient[dr_id][0]);
    }
    return doc_resp;
}


int patient_registration(int dr_id) {
    printf("patient %d registering to doctor %d\n", getpid(), dr_id);
    int reg_resp;
    pid_t pid = getpid();

    close(patient_register[0]);
    close(register_patient[1]);

    printf("patient %d writing pid to register\n", getpid());
    if (write(patient_register[1], &pid, sizeof(pid_t)) < 0) {
        perror("write patient reg");
        exit(3);
    }
    printf("patient %d writing dr id %d to register\n", getpid(), dr_id);
    if (write(patient_register[1], &dr_id, sizeof(int)) < 0) {
        perror("write patient reg");
        exit(3);
    }
    
    printf("patient %d reading register response\n", getpid());
    if (read(register_patient[0], &reg_resp, sizeof(int)) < 0) {
        perror("read patient reg");
        exit(3);
    }
    printf("patient %d, registers response: %d\n", getpid(), reg_resp);
    return reg_resp;
}

void enter_clinic() {
    sem_wait(clinic_capacity);
    sem_wait(rq_capacity);
    sem_wait(rq_lock);
    *rq_cnt += 1;
    sem_post(rq_lock);
    sem_wait(p_cnt_lock);
    *p_cnt += 1;
    sem_post(p_cnt_lock);
}

void leave_queue() {
    sem_wait(rq_lock);
    if (*rq_cnt > 0) *rq_cnt -= 1;
    sem_post(rq_lock);
    sem_post(rq_capacity);
}

void leave_clinic() {
    printf("patient %d leaving clinic\n", getpid());
    for (int j = 0; j < DR_NUM; j++) {
        close(patient_doctor[j][0]);
        close(patient_doctor[j][1]);
        close(doctor_patient[j][0]);
        close(doctor_patient[j][1]);
    }
    sem_wait(p_cnt_lock);
    if (*p_cnt > 0) *p_cnt -= 1;
    sem_post(p_cnt_lock);
    sem_post(clinic_capacity);
    exit(0);
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
            srand(getpid());
            sem_wait(emergency_lock);
            if (!*emergency) {
                sem_post(emergency_lock);
                patient_routine(i);
            } else {
                sem_post(emergency_lock);
            }
            
        }
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