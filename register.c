#include "register.h"

void open_reg(int *reg_arr) {
    if (reg_arr[1] == 0) reg_arr[1] = 1;
}

void close_reg(int *reg_arr) {
	if (reg_arr[1] == 1) reg_arr[1] = 0;
}

int is_open_reg(int *reg_arr) {
	if (reg_arr[1] == 1) return 1;
    return 0;
}

void register_routine(int reg_fd[2], int* reg_arr) {
    srand(getpid());
    do {
      	int j = 0;
        sem_wait(reg_o);
      	if (is_open_reg(reg_arr)) {
        	j = rand() % 2;
      	}
        sem_post(reg_o);

        sem_wait(reg[j]);

        sleep(4);
        close(reg_fd[1]);
        pid_t pid;
        read(reg_fd[0], &pid, sizeof(pid_t));
        printf("register %d processing patient %d\n", j + 1, pid);

        sem_post(reg[j]);
    } while(1);

    exit(0);
}

void create_registers(int reg_fd[2], int* reg_arr) {
    pid_t reg[2];

    for (int i = 0; i < REG_NUM; i++) {
        reg[i] = fork();

        if (reg[i] < 0) {perror("fork"); exit(5);}
        if (reg[i] == 0) {
            register_routine(reg_fd, reg_arr);
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