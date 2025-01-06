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

# define P_NUM 10

sem_t* door;

int main() {
    door = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* p_cnt = (int*) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *p_cnt = 0;
    sem_init(door, 1, 1);

    create_director();
    create_registers();
    create_patients(p_cnt);
    
    wait_registers();
    wait_director();

    sem_destroy(door);
    munmap(door, sizeof(sem_t));
    munmap(p_cnt, sizeof(int));
    return 0;
}
