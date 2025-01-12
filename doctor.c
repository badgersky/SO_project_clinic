#include "doctor.h"

void doctor_routine(int i) {
    do {
        printf("doctor %d\n", i);
        sleep(3);
    } while(1);

    exit(0);
}

void create_doctors() {
    pid_t pid;
    for (int i = 0; i < DR_NUM; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(5);
        }
        if (pid == 0) {
            srand(time(0) + i);
            doctor_routine(i);
        }
    }
}

void wait_doctors() {
    for (int i = 0; i < DR_NUM; i++) {
        if (wait(0) < 0) {
            perror("wait");
            exit(5);
        }
    }
}

int get_rand_id() {
    int r = rand() % 10;
    int dr_id;
    if (r < 6) {
        dr_id = 4 + (rand() % 2);
    }
    else {
        dr_id = rand() % (DR_NUM - 2);
    }
    
    return dr_id;
}