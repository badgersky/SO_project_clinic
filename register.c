#include "register.h"

void register_routine(int i) {
    do {
        printf("register %d\n", i);
    } while(1);
}

void create_registers() {
    pid_t pid;

    for (int i = 1; i <= REG_NUM; i++) {
        pid = fork();
        if (pid < 0) {
            return perror("fork");
            exit(1);
        }
        if (pid == 0) {
            register_routine(i);
        }
    }
}