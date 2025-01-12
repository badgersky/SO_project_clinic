#include "director.h"

void director_routine() {
    do {
        // printf("director\n");
        *t += 1;
        if (*t >= TK) {
            close_clinic();
            break;
        }
        sleep(3);
    } while(1);

    exit(0);
}

void close_clinic() {
    if (*clinic_state == 1) {
        printf("closing clinic\n");
        *clinic_state = 0;
    }
}

void create_director() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(6);
    }
    if (pid == 0) {
        director_routine();
    }
}

void wait_director() {
    if (wait(0) < 0) {
        perror("wait");
        exit(6);
    }
}