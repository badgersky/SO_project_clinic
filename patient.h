#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "doctor.h"

#define MAX_P 100

void patient_routine(int i);
void create_patients();
void wait_patients();
void leave_clinic();
void enter_clinic();
void leave_queue();
int patient_registration(int dr_id);
void go_to_doc(int dr_id);