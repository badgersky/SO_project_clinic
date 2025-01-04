#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define MAX_P 20

void patient_routine();

void create_patients();
void create_patient();

void wait_patients();