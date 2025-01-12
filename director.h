#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

pid_t create_director();
void wait_director();
void director_routine();
void close_clinic();
void signal_handler();