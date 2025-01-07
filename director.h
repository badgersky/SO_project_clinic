#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void director_routine(int* p_cnt);

void create_director(int* p_cnt);

void wait_director();