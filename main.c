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
    initialize_sem();

    int* reg_arr = (int*) mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (reg_arr == MAP_FAILED) {perror("mmap"); exit(6);}
    reg_arr[0] = 1; reg_arr[1] = 0;

    int reg_fd[2];
    if (pipe(reg_fd) < 0) {perror("pipe"); exit(6);}

    int* p_cnt = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_cnt == MAP_FAILED) {perror("mmap"); exit(6);}

    char** specs;
    specs = get_specializations();

    *p_cnt = 0;
    create_director(p_cnt, reg_arr);
    create_registers(reg_fd, reg_arr);
    for (int i = 0; i < 6; i++) {
        create_doctor(i, specs[i]);
    }
    create_patients(p_cnt, reg_fd);

    wait_registers();
    wait_director();
    wait_doctors();

    destroy_sem();
    free_specializations(specs);

    if (munmap(p_cnt, sizeof(int)) < 0) {perror("munmap"); exit(6);}
    return 0;
}
