#include "register.h"

void register_routine(int num, int* p_cnt, struct p_queue* p_q) {
    do {
        sem_wait(reg_q_f);
        sem_wait(reg);
        p_q->i -= 1;
        int pid = p_q->ids[p_q->i];
        printf("register %d processing patient %d\n", num, pid);
        p_q->ids[p_q->i] = 0;
        sem_post(reg);
        sem_post(reg_q_e);

        sleep(1);
    } while(1);

    exit(0);
}

void create_registers(int* p_cnt, struct p_queue* p_q) {
    pid_t reg[2];

    for (int i = 0; i < REG_NUM; i++) {
        reg[i] = fork();

        if (reg[i] < 0) {
            perror("fork");
        } 
        if (reg[i] == 0) {
            register_routine(i + 1, p_cnt, p_q);
        }
    }
}

void wait_registers() {
    for (int i = 0; i < REG_NUM; i++) {
        if (wait(0) < 0) {
            perror("wait");
        }
    }
}