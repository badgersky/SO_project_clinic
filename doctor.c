#include "doctor.h"

void doctor_routine(int i) {
    do {
        printf("doctor %d\n", i);
        sleep(1);
    } while(1);

    exit(0);
}

void create_doctors() {
    pid_t pid;
    for (int i = 0; i < DR_NUM - 1; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(5);
        }
        if (pid == 0) {
            doctor_routine(i);
        }
    }
}

void wait_doctors() {
    for (int i = 0; i < DR_NUM - 1; i++) {
        if (wait(0) < 0) {
            perror("wait");
            exit(5);
        }
    }
}