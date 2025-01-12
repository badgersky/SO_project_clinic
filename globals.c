#include "globals.h"

sem_t *reg_queue, *clinic_capacity, *rq_lock, *reg_pipe_lock, *drq_lock[6], *dr_queue[6];

int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANONYMOUS;

int *rq_cnt;
int *t;
int *clinic_state;

int patient_register[2];
int register_patient[2];

int* dr_limits;
int* dr_p_cnt;

void initialize_sem() {
    reg_queue = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (reg_queue == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    clinic_capacity = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (clinic_capacity == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    rq_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (rq_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    reg_pipe_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (reg_pipe_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }

    for (int i = 0; i < DR_NUM; i++) {
        drq_lock[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (drq_lock[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(drq_lock[i], 1, 1);
        
        dr_queue[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (dr_queue[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(dr_queue[i], 1, 1);
    }

    sem_init(reg_queue, 1, 1);
    sem_init(clinic_capacity, 1, 30);
    sem_init(rq_lock, 1, 1);
    sem_init(reg_pipe_lock, 1, 1);
}

void destroy_sem() {
    sem_destroy(reg_queue);
    sem_destroy(clinic_capacity);
    sem_destroy(rq_lock);
    sem_destroy(reg_pipe_lock);

    for (int i = 0; i < DR_NUM; i++) {
        sem_destroy(drq_lock[i]);
        sem_destroy(dr_queue[i]);
        if (munmap(drq_lock[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
        
        if (munmap(dr_queue[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
    }

    if (munmap(reg_queue, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(clinic_capacity, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(rq_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(reg_pipe_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
}

void share_variables() {
    rq_cnt = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (rq_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    t = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (t == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    clinic_state = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (clinic_state == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }

    dr_limits = mmap(NULL, sizeof(int) * DR_NUM, protection, visibility, -1, 0);
    dr_p_cnt = mmap(NULL, sizeof(int) * DR_NUM, protection, visibility, -1, 0);
}

void init_variables() {
    *rq_cnt = 0;
    *t = 0;
    *clinic_state = 1;

    dr_limits[0] = 3;
    dr_limits[1] = 3;
    dr_limits[2] = 3;
    dr_limits[3] = 3;
    dr_limits[4] = 8;
    dr_limits[5] = 8;

    for (int i = 0; i < DR_NUM; i++) {
        dr_p_cnt[i] = 0;
    }

    pipe(patient_register);
    pipe(register_patient);
}

void free_variables() {
    if (munmap(rq_cnt, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(t, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(4);
    } 
    if (munmap(clinic_state, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(4);
    }

    close(patient_register[1]);
    close(register_patient[1]);
    close(patient_register[0]);
    close(register_patient[0]);
}