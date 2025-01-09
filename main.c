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
    int* dr_limits = (int*) mmap(NULL, sizeof(int) * 6, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (dr_limits == MAP_FAILED) {perror("mmap"); exit(6);}
    dr_limits[0] = X5;
    dr_limits[1] = X4;
    dr_limits[2] = X3;
    dr_limits[3] = X2;
    dr_limits[4] = X1;
    dr_limits[5] = X1;

    int* visits_cnt = (int*) mmap(NULL, sizeof(int) * 6, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (visits_cnt == MAP_FAILED) {perror("mmap"); exit(6);}
    for (int i = 0; i < 6; i++) {
    	visits_cnt[i] = 0;
    }

    int* reg_arr = (int*) mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (reg_arr == MAP_FAILED) {perror("mmap"); exit(6);}
    reg_arr[0] = 1; reg_arr[1] = 0;

    int reg_fd[2];
    if (pipe(reg_fd) < 0) {perror("pipe"); exit(6);}
	int dr_fd[6][2];
    for (int i = 0; i < 6; i++) {
    	if (pipe(dr_fd[i]) < 0) {perror("pipe"); exit(6);}
    }

    int* p_cnt = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_cnt == MAP_FAILED) {perror("mmap"); exit(6);}
    int* reg_q_cnt = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reg_q_cnt == MAP_FAILED) {perror("mmap"); exit(6);}

    char** specs;
    specs = get_specializations();

    *p_cnt = 0;
    *reg_q_cnt = 0;
    create_director(reg_q_cnt, reg_fd, p_cnt, reg_arr, specs);
    create_register(reg_q_cnt, 0, reg_fd, reg_arr, specs);
    for (int i = 0; i < 6; i++) {
        create_doctor(i, specs[i], dr_fd);
    }
    create_patients(reg_q_cnt, p_cnt, reg_fd, specs, dr_fd);

    wait_registers();
    wait_director();
    wait_doctors();

    destroy_sem();
    free_specializations(specs);

    if (munmap(p_cnt, sizeof(int)) < 0) {perror("munmap"); exit(6);}
    if (munmap(reg_q_cnt, sizeof(int)) < 0) {perror("munmap"); exit(6);}
    if (munmap(reg_arr, sizeof(int) * 2) < 0) {perror("munmap"); exit(6);}
    if (munmap(dr_limits, sizeof(int) * 6) < 0) {perror("munmap"); exit(6);}
    if (munmap(visits_cnt, sizeof(int) * 6) < 0) {perror("munmap"); exit(6);}
    return 0;
}
