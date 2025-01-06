#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>

#include "patient.h"
#include "director.h"
#include "register.h"
#include "sem.h"

sem_t *door, *reg, *reg_q_e, *reg_q_f;

int main() {
    struct p_queue* p_q;
    p_q = (struct p_queue*) mmap(0, sizeof(struct p_queue), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_q == MAP_FAILED) perror("mmap");
    door = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (door == MAP_FAILED) perror("mmap");
    reg = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg == MAP_FAILED) perror("mmap");
    reg_q_e = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_q_e == MAP_FAILED) perror("mmap");
    reg_q_f = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_q_f == MAP_FAILED) perror("mmap");

    int* p_cnt = (int*) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_cnt == MAP_FAILED) perror("mmap");

    *p_cnt = 0;
    if (sem_init(door, 1, 1) < 0) perror("sem_init");
    if (sem_init(reg, 1, 1) < 0) perror("sem_init");
    if (sem_init(reg_q_e, 1, MAX_P) < 0) perror("sem_init");
    if (sem_init(reg_q_f, 1, 0) < 0) perror("sem_init");

    create_director();
    create_registers(p_cnt, p_q);
    create_patients(p_cnt, p_q);
    
    wait_registers();
    wait_director();

    if (sem_destroy(door) < 0) perror("sem_destroy");
    if (sem_destroy(reg) < 0) perror("sem_destroy");
    if (sem_destroy(reg_q_e) < 0) perror("sem_destroy");
    if (sem_destroy(reg_q_f) < 0) perror("sem_destroy");
    if (munmap(door, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(reg, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(reg_q_e, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(reg_q_f, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(p_cnt, sizeof(int)) < 0) perror("munmap");
    if (munmap(p_q, sizeof(struct p_queue)) < 0) perror("munmap");
    return 0;
}
