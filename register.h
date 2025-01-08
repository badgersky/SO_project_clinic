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

void open_reg(int *reg_arr);

void close_reg(int *reg_arr);

void register_routine(int reg_fd[2], int* reg_arr);

void create_registers(int reg_fd[2], int* reg_arr);

void wait_registers();