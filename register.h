#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

void create_registers();
void wait_registers();
void register_routine(int i);
void open_close_register();
void process_patient(int i);