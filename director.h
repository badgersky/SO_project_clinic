#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void director_routine(int* p_cnt, int* reg_arr);

void create_director(int* p_cnt, int* reg_arr);

void wait_director();