#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

void create_registers();
void wait_registers();
void register_routine();
void open_close_register(int* desks_open);
void process_patient();