#include "globals.h"

sem_t *reg_queue, *clinic_capacity, *rq_lock;

int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANONYMOUS;

int *rq_cnt;
int *t;
int *clinic_state;

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

    sem_init(reg_queue, 1, 1);
    sem_init(clinic_capacity, 1, 30);
    sem_init(rq_lock, 1, 1);
}

void destroy_sem() {
    sem_destroy(reg_queue);
    sem_destroy(clinic_capacity);
    sem_destroy(rq_lock);

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
}