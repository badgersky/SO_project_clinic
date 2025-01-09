#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "sem.h"
#include "patient.h"

# define REG_NUM 2

int is_open_reg(int *reg_arr);

pid_t open_reg(int* reg_q_cnt, int i, int reg_fd[2], int *reg_arr, char** specs, int* visits_cnt, int* dr_limits);

void close_reg(int *reg_arr);

void register_routine(int* reg_q_cnt, int i, int reg_fd[2], int* reg_arr, char** specs, int* visits_cnt, int* dr_limits);

pid_t create_register(int* reg_q_cnt, int i, int reg_fd[2], int* reg_arr, char** specs, int* visits_cnt, int* dr_limits);

void wait_registers();