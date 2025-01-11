#include "sem.h"
#include "patient.h"

sem_t *door, *reg[2], *doctors[4], *reg_q, *poz, *dr_q[6], *reg_r_p, *reg_w_p, *p_limit;

void initialize_sem() {
    for (int i = 0; i < 4; i++) {
        doctors[i] = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (doctors[i] == MAP_FAILED) {perror("mmap"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        reg[i] = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (reg == MAP_FAILED) {perror("mmap"); exit(2);}
    }
    for (int i = 0; i < 6; i++) {
        dr_q[i] = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (dr_q[i] == MAP_FAILED) {perror("mmap"); exit(2);}
    }
    door = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (door == MAP_FAILED) {perror("mmap"); exit(2);}
	reg_q = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_q == MAP_FAILED) {perror("mmap"); exit(2);}
    poz = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (poz == MAP_FAILED) {perror("mmap"); exit(2);}
    reg_w_p = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_w_p == MAP_FAILED) {perror("mmap"); exit(2);}
    reg_r_p = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_r_p == MAP_FAILED) {perror("mmap"); exit(2);}
    p_limit = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_limit == MAP_FAILED) {perror("mmap"); exit(2);}

    for (int i = 0; i < 4; i++) {
        if (sem_init(doctors[i], 1, 1) < 0) {perror("sem_init"); exit(2);}
    }
    for (int i = 0; i < 6; i++) {
        if (sem_init(dr_q[i], 1, 1) < 0) {perror("sem_init"); exit(2);}
    }
    if (sem_init(reg[0], 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg[1], 1, 0) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(door, 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg_q, 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(poz, 1, 2) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg_w_p, 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg_r_p, 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(p_limit, 1, MAX_P) < 0) {perror("sem_init"); exit(2);}
}

void destroy_sem() {
    for (int i = 0; i < 4; i++) {
        if (sem_destroy(doctors[i]) < 0) {perror("sem_destroy"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        if (sem_destroy(reg[i]) < 0) {perror("sem_destroy"); exit(2);}
    }
    for (int i = 0; i < 6; i++) {
        if (sem_destroy(dr_q[i]) < 0) {perror("sem_destroy"); exit(2);}
    }
    if (sem_destroy(door) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(reg_q) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(poz) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(reg_w_p) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(reg_r_p) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(p_limit) < 0) {perror("sem_destroy"); exit(2);}

    for (int i = 0; i < 4; i++) {
        if (munmap(doctors[i], sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        if (munmap(reg[i], sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    }
    for (int i = 0; i < 6; i++) {
        if (munmap(dr_q[i], sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    }
    if (munmap(door, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(reg_q, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(poz, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(reg_w_p, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(reg_r_p, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(p_limit, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
}

void print_sem_val(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("%d\n", sem_val);
}