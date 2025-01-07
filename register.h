#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "sem.h"
#include "patient.h"

# define REG_NUM 2

void register_routine(int num, int* p_cnt, int reg_fd[2]);

void create_registers(int* p_cnt, int reg_fd[2]);

void wait_registers();