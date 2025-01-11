#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#define REG_NUM 2

void create_registers();
void wait_registers();
void register_routine(int i);