#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SEM_H
#define SEM_H

extern sem_t* reg_queue;
extern sem_t* rq_lock;
extern sem_t* clinic_capacity;

extern int protection;
extern int visibility;

extern int *rq_cnt;

// clinic closing variables
extern int *t;
extern int *clinic_state;

// communication between register and patients
extern int patient_register[2];
extern int register_patient[2];

extern sem_t* reg_pipe_lock;

#endif

void initialize_sem();
void destroy_sem();
void share_variables();
void free_variables();