#include "director.h"
#include "sem.h"

void director_routine(int* p_cnt) {
    do {
        sem_wait(door);
        printf("director, number of patients: %d\n", *p_cnt);
        sem_post(door);
        sleep(3);
    } while(1);

    exit(0);
}

void create_director(int* p_cnt) {
    pid_t d = fork();

    if (d < 0) {perror("fork"); exit(4);}
    if (d == 0) {
        director_routine(p_cnt);
    }
}

void wait_director() {
    if (wait(0) < 0) {perror("wait"); exit(4);}
}