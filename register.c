#include "register.h"

pid_t open_reg(int* reg_q_cnt, int i, int reg_fd[2], int *reg_arr, char** specs) {
    printf("opening register\n");
    pid_t pid = create_register(reg_q_cnt, 1, reg_fd, reg_arr, specs);
    sem_post(reg[1]);
    return pid;
}

void close_reg(int *reg_arr) {
	if (reg_arr[1] == 1) reg_arr[1] = 0;
    printf("closing register\n");
    sem_wait(reg[1]);
}

int is_open_reg(int *reg_arr) {
	if (reg_arr[1] == 1) return 1;
    return 0;
}

void register_routine(int* reg_q_cnt, int i, int reg_fd[2], int* reg_arr, char** specs) {
    srand(getpid());
    close(reg_fd[1]);
    do {
      	sem_wait(reg_q);
        printf("patients in register queue: %d\n", *reg_q_cnt);
        sem_post(reg_q);

        sem_wait(reg[i]);
		pid_t pid;
        read(reg_fd[0], &pid, sizeof(pid_t));
        printf("register %d registering patient %d\n", i + 1, pid);
        sem_post(reg[i]);
    } while(1);

    exit(0);
}

pid_t create_register(int* reg_q_cnt, int i, int reg_fd[2], int* reg_arr, char** specs) {
    pid_t reg;

    reg = fork();

    if (reg < 0) {perror("fork"); exit(5);}
    if (reg == 0) {
        register_routine(reg_q_cnt, i, reg_fd, reg_arr, specs);
    }
    return reg;
}

void wait_registers() {
    for (int i = 0; i < REG_NUM; i++) {
        if (wait(0) < 0) {
            perror("wait"); exit(5);
        }
    }
}