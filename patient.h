#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#ifndef PATIENT_H
#define PATIENT_H

# define MAX_P 20

void go_to_doc(int dr_fd[2], int dr_id);

int get_dr_id();

void patient_routine(int* reg_q_cnt, int* p_cnt, int reg_fd[2], int resp_fd[2], char** specs, int dr_fd[6][2]);

void create_patients(int* reg_q_cnt, int* p_cnt, int reg_fd[2], int resp_fd[2], char** specs, int dr_fd[6][2]);

void create_patient(int* reg_q_cnt, int* p_cnt, int reg_fd[2], int resp_fd[2], char** specs, int dr_fd[6][2]);

void wait_patients();

int patient_register(int* reg_q_cnt, int reg_fd[2], int resp_fd[2], int dr_id);

#endif