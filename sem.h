#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SEM_H
#define SEM_H

extern sem_t *door;
extern sem_t *reg_q;
extern sem_t *reg[2];
extern sem_t* doctors[4];
extern sem_t* poz;
extern sem_t* dr_q[6];
extern sem_t* reg_r_p;
extern sem_t* reg_w_p;

#endif

void initialize_sem();

void destroy_sem();

void print_sem_values(sem_t* sem);