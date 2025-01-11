#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SEM_H
#define SEM_H

extern sem_t* reg_queue;
extern sem_t* rq_lock;
extern sem_t* clinic_capacity;

extern int protection;
extern int visibility;

#endif

void initialize_sem();
void destroy_sem();