#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#ifndef PATIENT_H
#define PATIENT_H

# define MAX_P 20

struct p_queue{
  int ids[MAX_P];
  int i;
};

void patient_routine(int* p_cnt, struct p_queue* p_q);

void create_patients(int* p_cnt, struct p_queue* p_q);
void create_patient(int* p_cnt, struct p_queue* p_q);

void wait_patients();

#endif