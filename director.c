#include "director.h"

void director_routine() {
    while(1) {
        sleep(1);
        printf("director\n");
    }

    exit(0);
}

void create_director() {
    pid_t d = fork();

    if (d < 0) {
        perror("fork");
    }
    if (d == 0) {
        director_routine();
    }
}

void wait_director() {
    if (wait(0) < 0) {
        perror("wait");
    }
}