#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "sem.h"
#include "patient.h"

extern int *rq_cnt;

void share_variables();
void free_variables();

int main() {
    initialize_sem();

    share_variables();

    create_registers();
    create_patients();

    wait_registers();
    wait_patients();

    destroy_sem();
    return 0;
}

void share_variables() {
    rq_cnt = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (rq_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
}

void free_variables() {
    if (munmap(rq_cnt, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(4);
    } 
}