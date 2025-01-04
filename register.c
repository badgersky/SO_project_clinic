#include "register.h"

void register_routine(int num) {
    while(1) {
        sleep(1);
        printf("register %d\n", num);
    }

    exit(0);
}

void create_registers() {
    pid_t reg[2];

    for (int i = 0; i < REG_NUM; i++) {
        reg[i] = fork();

        if (reg[i] < 0) {
            perror("fork");
        } 
        if (reg[i] == 0) {
            register_routine(i + 1);
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