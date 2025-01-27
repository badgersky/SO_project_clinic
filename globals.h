#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef GLOBALS_H
#define GLOBALS_H

#define DR_NUM 6
#define MAX_P 500

#define TP 0
#define TK 20

#define BUFFER 30

#define REG_NUM 2
#define MAX_QUEUE 15
#define MAX_CAPACITY 30

#define X5 50
#define X4 50
#define X3 50
#define X2 50
#define X1 50

extern int protection;
extern int visibility;

// doctors and registers pids
extern pid_t* r_pid;
extern pid_t* dr_pids;
extern sem_t* p_lock;

// entering clinic, register control, queues
extern sem_t* reg_queue[2];
extern sem_t* rq_lock;
extern sem_t* clinic_capacity;
extern sem_t* p_cnt_lock;
extern sem_t* rq_capacity;

extern int *rq_cnt;
extern int *p_cnt;
extern int *desks_open;

// clinic worktime variables
extern sem_t* cs_lock;

extern int *t;
extern int *clinic_state;

// communication between register and patients
extern int patient_register[2][2];
extern int register_patient[2][2];

extern sem_t* reg_pipe_lock[2];

// doctor queue control
extern sem_t* dr_queue[6];
extern sem_t* drq_lock[6];
extern sem_t* drq_cnt_lock[6];

extern int* dr_limits;
extern int* dr_p_cnt;
extern int* drq_cnt;

// communication between doctors and patients
extern int patient_doctor[6][2];
extern int doctor_patient[6][2];

extern sem_t* dr_pipe_lock[6];

// daily report
extern FILE *report;
extern sem_t* report_lock;

// patients pids
typedef struct {
    pid_t pids[MAX_P];
    int count;
    sem_t* pid_lock;
} pid_tracker;

extern pid_tracker* pids;

// doctors queues pid tracker
extern pid_t* pdrq_pids[6];
extern sem_t* pdrq_pids_lock[6];

// emergency
extern int* emergency;
extern sem_t* emergency_lock;

#endif

void initialize_sem();
void destroy_sem();
void share_variables();
void free_variables();
void init_variables();
void open_report();
void close_report();
void write_report(char* msg);
void initialize_pids();
void cleanup_pids();
void insert_pid(int dr_id, pid_t pid);
void remove_pid(int dr_id, pid_t pid);
void print_sem_value(sem_t* s);
int get_sem_value(sem_t* s);
void* wait_for_processes(void* arg);