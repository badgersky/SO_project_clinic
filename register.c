#include "register.h"

pid_t open_reg(int* reg_q_cnt, int i, int reg_fd[2], int resp_fd[2], int *reg_arr, char** specs, int* visits_cnt, int* dr_limits) {
    printf("opening register\n");
    pid_t pid = create_register(reg_q_cnt, 1, reg_fd, resp_fd, reg_arr, specs, visits_cnt, dr_limits);
    reg_arr[1] = 1;
    sem_post(reg[1]);
    return pid;
}

void close_reg(int *reg_arr) {
	if (reg_arr[1] == 1) reg_arr[1] = 0;
//    printf("closing register\n");
    sem_wait(reg[1]);
}

int is_open_reg(int *reg_arr) {
	if (reg_arr[1] == 1) return 1;
    return 0;
}

void register_routine(int* reg_q_cnt, int i, int reg_fd[2], int resp_fd[2], int* reg_arr, char** specs, int* visits_cnt, int* dr_limits) {
    srand(getpid());
    int reg_res;
    do {
        printf("register %d\n", i + 1);

        sem_wait(reg[i]);
        printf("after reg[i]\n");
        int dr_id;
        sem_wait(reg_r_p);
        read(reg_fd[0], &dr_id, sizeof(int));
        sem_post(reg_r_p);
        printf("after reg_r_p\n");

        sem_wait(dr_q[dr_id]);
        printf("lol\n");
        if (visits_cnt[dr_id] < dr_limits[dr_id]) {
            visits_cnt[dr_id] += 1;
            reg_res = 1;
        } else {
            reg_res = 0;
        }
        sem_post(dr_q[dr_id]);
        printf("after dr_q\n");

        sem_wait(reg_w_p);
        write(resp_fd[1], &reg_res, sizeof(int));
        sem_post(reg_w_p);
        printf("after reg_w_p\n");
        
        sem_post(reg[i]);
        sleep(1);
    } while(1);

    exit(0);
}

pid_t create_register(int* reg_q_cnt, int i, int reg_fd[2], int resp_fd[2], int* reg_arr, char** specs, int* visits_cnt, int* dr_limits) {
    pid_t reg;

    reg = fork();

    if (reg < 0) {perror("fork"); exit(5);}
    if (reg == 0) {
        register_routine(reg_q_cnt, i, reg_fd, resp_fd, reg_arr, specs, visits_cnt, dr_limits);
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