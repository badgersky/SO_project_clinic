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

sem_t *door, *reg;

int main() {
	int reg_fd[2];
    if (pipe(reg_fd) < 0) perror("pipe");

    door = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (door == MAP_FAILED) perror("mmap");
    reg = (sem_t*) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg == MAP_FAILED) perror("mmap");
    int* p_cnt = (int*) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_cnt == MAP_FAILED) perror("mmap");

    if (sem_init(door, 1, 1) < 0) perror("sem_init");
    if (sem_init(reg, 1, 2) < 0) perror("sem_init");

    *p_cnt = 0;
    create_director(p_cnt);
    create_registers(reg_fd);
    create_patients(p_cnt, reg_fd);
    
    wait_registers();
    wait_director();

    if (sem_destroy(door) < 0) perror("sem_destroy");
    if (sem_destroy(reg) < 0) perror("sem_destroy");
    if (munmap(door, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(reg, sizeof(sem_t)) < 0) perror("munmap");
    if (munmap(p_cnt, sizeof(int)) < 0) perror("munmap");
    return 0;
}
