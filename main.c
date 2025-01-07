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
#include "doctor.h"

int main() {
	int reg_fd[2];
    if (pipe(reg_fd) < 0) perror("pipe"); exit(6);

    initialize_sem();

    int* p_cnt = (int*) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_cnt == MAP_FAILED) perror("mmap"); exit(6);

    *p_cnt = 0;
    create_director(p_cnt);
    create_registers(reg_fd);
    for (int i = 0; i < 6; i++) {
        if (i == 0) i+= 1;
        create_doctor(i);
    }
    create_patients(p_cnt, reg_fd);

    wait_registers();
    wait_director();
    wait_doctors();

    destroy_sem();

    if (munmap(p_cnt, sizeof(int)) < 0) perror("munmap"); exit(6);
    return 0;
}
