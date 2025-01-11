#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void director_routine(int* reg_q_cnt, int reg_fd[2], int resp_fd[2], int* p_cnt, int* reg_arr, char** specs, int* visits_cnt, int* dr_limits);

void create_director(int* reg_q_cnt, int reg_fd[2], int resp_fd[2], int* p_cnt, int* reg_arr, char** specs, int* visits_cnt, int* dr_limits);

void wait_director();