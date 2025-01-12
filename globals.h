#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef GLOBALS_H
#define GLOBALS_H

#define DR_NUM 6

extern int protection;
extern int visibility;

// entering clinic, register control, queues
extern sem_t* reg_queue;
extern sem_t* rq_lock;
extern sem_t* clinic_capacity;

extern int *rq_cnt;

// clinic worktime variables
extern int *t;
extern int *clinic_state;

// communication between register and patients
extern int patient_register[2];
extern int register_patient[2];

extern sem_t* reg_pipe_lock;

// doctor queue control
extern sem_t* dr_queue[6];
extern sem_t* drq_lock[6];

extern int* dr_limits;
extern int* dr_p_cnt;

#endif

void initialize_sem();
void destroy_sem();
void share_variables();
void free_variables();
void init_variables();