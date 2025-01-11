#include "director.h"

void director_routine() {
    do {
        printf("director\n");
        sleep(3);
    } while(1);

    exit(0);
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