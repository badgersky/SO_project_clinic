#include "sem.h"

sem_t *door, *reg[2], *doctors[6], *reg_q;

void initialize_sem() {
    for (int i = 0; i < 6; i++) {
        doctors[i] = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (doctors[i] == MAP_FAILED) {perror("mmap"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        reg[i] = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (reg == MAP_FAILED) {perror("mmap"); exit(2);}
    }
    door = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (door == MAP_FAILED) {perror("mmap"); exit(2);}
	reg_q = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_q == MAP_FAILED) {perror("mmap"); exit(2);}

    for (int i = 0; i < 6; i++) {
        if (sem_init(doctors[i], 1, 1) < 0) {perror("sem_init"); exit(2);}
    }
    if (sem_init(reg[0], 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg[1], 1, 0) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(door, 1, 1) < 0) {perror("sem_init"); exit(2);}
    if (sem_init(reg_q, 1, 1) < 0) {perror("sem_init"); exit(2);}

}

void destroy_sem() {
    for (int i = 0; i < 6; i++) {
        if (sem_destroy(doctors[i]) < 0) {perror("sem_destroy"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        if (sem_destroy(reg[i]) < 0) {perror("sem_destroy"); exit(2);}
    }
    if (sem_destroy(door) < 0) {perror("sem_destroy"); exit(2);}
    if (sem_destroy(reg_q) < 0) {perror("sem_destroy"); exit(2);}

    for (int i = 0; i < 6; i++) {
        if (munmap(doctors[i], sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    }
    for (int i = 0; i < 2; i++) {
        if (munmap(reg[i], sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    }
    if (munmap(door, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
    if (munmap(reg_q, sizeof(sem_t)) < 0) {perror("munmap"); exit(2);}
}

void print_sem_val(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("%d\n", sem_val);
}