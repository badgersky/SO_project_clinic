#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "sem.h"
#include "patient.h"

# define REG_NUM 2

void register_routine(int num, int* p_cnt, struct p_queue* p_q);

void create_registers(int* p_cnt, struct p_queue* p_q);

void wait_registers();