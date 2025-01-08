#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define X1 10
#define X2 8
#define X3 8
#define X4 8
#define X5 8

void director_routine(int* reg_q_cnt, int reg_fd[2], int* p_cnt, int* reg_arr, char** specs);

void create_director(int* reg_q_cnt, int reg_fd[2], int* p_cnt, int* reg_arr, char** specs);

void wait_director();