#include "register.h"

void register_routine() {
    int* desks_open = (int*) malloc(sizeof(int));
    *desks_open = 1;

    do {
        open_close_register(desks_open);
        sleep(1);
    } while(1);

    free(desks_open);
    exit(0);
}

void open_close_register(int* desks_open) {
    sem_wait(rq_lock);
    if (*desks_open == 1 && *rq_cnt > MAX_QUEUE / 2) {
        printf("second register open\n");
        sem_post(reg_queue);
        *desks_open += 1;
    }
    if (*desks_open == 2 && *rq_cnt < MAX_QUEUE / 3) {
        printf("second register closed\n");
        sem_wait(reg_queue);
        *desks_open -= 1;
    }
    printf("Number of patients in register queue: %d\n", *rq_cnt);
    sem_post(rq_lock);
}

void create_registers() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        return perror("fork");
        exit(1);
    }
    if (pid == 0) {
        register_routine();
    }
}

void wait_registers() { 
    if (wait(0) < 0) {
        perror("wait");
        exit(1);
    }
}