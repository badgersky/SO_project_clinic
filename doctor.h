#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "sem.h"

#define DR_NUM 6

void create_doctors();
void wait_doctors();
void doctor_routine(int i);