#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SEM_H
#define SEM_H

extern sem_t* reg_queue;

#endif

void initialize_sem();