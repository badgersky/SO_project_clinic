#include "register.h"

void open_reg(int *reg_arr[2]) {
    if (*reg_arr[1] == 0) *reg_arr[1] = 1;
}

void close_reg(int *reg_arr[2]) {
	if (*reg_arr[1] == 1) *reg_arr[1] = 0;
}

void register_routine(int i, int reg_fd[2]) {
    srand(getpid());
    do {
        sem_wait(reg[i]);

        sleep(4);
        close(reg_fd[1]);
        pid_t pid;
        read(reg_fd[0], &pid, sizeof(pid_t));
        printf("register %d processing patient %d\n", i, pid);

        sem_post(reg[i]);
    } while(1);

    exit(0);
}

void create_registers(int reg_fd[2]) {
    pid_t reg[2];

    for (int i = 0; i < REG_NUM; i++) {
        reg[i] = fork();

        if (reg[i] < 0) {perror("fork"); exit(5);}
        if (reg[i] == 0) {
            register_routine(i + 1, reg_fd);
        }
    }
}

void wait_registers() {
    for (int i = 0; i < REG_NUM; i++) {
        if (wait(0) < 0) {
            perror("wait"); exit(5);
        }
    }
}