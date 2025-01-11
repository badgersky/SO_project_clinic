#include "register.h"

void register_routine() {
    do {
        printf("register\n");
        sleep(1);
    } while(1);
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