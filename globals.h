#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#ifndef GLOBALS_H
#define GLOBALS_H

#define DR_NUM 6
#define MAX_P 100

#define TP 0
#define TK 100

#define BUFFER 100

#define REG_NUM 2
#define MAX_QUEUE 30
#define MAX_CAPACITY 50

extern int protection;
extern int visibility;

// entering clinic, register control, queues
extern sem_t* reg_queue;
extern sem_t* rq_lock;
extern sem_t* clinic_capacity;
extern sem_t* p_cnt_lock;
extern sem_t* rq_capacity;

extern int *rq_cnt;
extern int *p_cnt;

// clinic worktime variables
extern sem_t* cs_lock;

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

// communication between doctors and patients
extern int patient_doctor[6][2];
extern int doctor_patient[6][2];

extern sem_t* dr_pipe_lock[6];

// daily report
extern FILE *report;
extern sem_t* report_lock;

#endif

void initialize_sem();
void destroy_sem();
void share_variables();
void free_variables();
void init_variables();
void open_report();
void close_report();
void write_report(char* msg);