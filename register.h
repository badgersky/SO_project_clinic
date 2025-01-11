#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

#define REG_NUM 2
#define MAX_QUEUE 10

void create_registers();
void wait_registers();
void register_routine();
void open_close_register(int* desks_open);