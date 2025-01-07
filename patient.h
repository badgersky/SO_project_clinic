#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#ifndef PATIENT_H
#define PATIENT_H

# define MAX_P 20

void patient_routine(int* p_cnt, int reg_fd[2]);

void create_patients(int* p_cnt, int reg_fd[2]);
void create_patient(int* p_cnt, int reg_fd[2]);

void wait_patients();

#endif