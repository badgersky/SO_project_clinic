#include "sem.h"

void initialize_sem() {
    door = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (door == MAP_FAILED) perror("mmap");
    reg = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg == MAP_FAILED) perror("mmap");

    if (sem_init(door, 1, 1) < 0) perror("sem_init");
    if (sem_init(reg, 1, 2) < 0) perror("sem_init");
}

void destroy_sem() {
    if (sem_destroy(door) < 0) perror("sem_destroy");
    if (sem_destroy(reg) < 0) perror("sem_destroy");
    if (munmap(door, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(reg, sizeof(sem_t)) < 0) perror("munmap");
}

void print_sem_val(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("%d\n", sem_val);
}