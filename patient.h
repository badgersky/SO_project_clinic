#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define MAX_P 20

void patient_routine(int* p_cnt);

void create_patients(int* p_cnt);
void create_patient(int* p_cnt);

void wait_patients();