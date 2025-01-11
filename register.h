#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "sem.h"

#define REG_NUM 2
#define MAX_QUEUE 10

void create_registers();
void wait_registers();
void register_routine();