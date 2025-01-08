#include "director.h"
#include "sem.h"
#include "register.h"

void director_routine(int* reg_q_cnt, int reg_fd[2], int* p_cnt, int* reg_arr, char** specs) {
    pid_t pid;
  	do {
        sem_wait(reg_q);
        printf("director, number of patients: %d\n", *p_cnt);
        if (!is_open_reg(reg_arr) && *reg_q_cnt >= MAX_P / 2) {
        	pid = open_reg(reg_q_cnt, 1, reg_fd, reg_arr, specs);
        }
        if (is_open_reg(reg_arr) && *reg_q_cnt < MAX_P / 3) {
			kill(pid, SIGKILL);
        }
        sem_post(reg_q);
        sleep(3);
    } while(1);

    exit(0);
}

void create_director(int* reg_q_cnt, int reg_fd[2], int* p_cnt, int* reg_arr, char** specs) {
    pid_t d = fork();

    if (d < 0) {perror("fork"); exit(4);}
    if (d == 0) {
        director_routine(reg_q_cnt, reg_fd, p_cnt, reg_arr, specs);
    }
}

void wait_director() {
    if (wait(0) < 0) {perror("wait"); exit(4);}
}