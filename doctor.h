#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "globals.h"

void create_doctors();
void wait_doctors();
void doctor_routine(int i);
int get_rand_id();
void examine_patient(int dr_id);
int check_limits();
