#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define REG_NUM 2

void register_routine(int num);

void create_registers();

void wait_registers();