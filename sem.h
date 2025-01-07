#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SEM_H
#define SEM_H

extern sem_t *door;
extern sem_t *reg;

#endif

void initialize_sem();

void destroy_sem();

void print_sem_values(sem_t* sem);