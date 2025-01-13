#include "director.h"

void director_routine() {
    int full;
    
    do {
        // printf("director\n");
        full = check_limits();
        if (full == 1) {
            close_clinic();
        }

        *t += 1;
        if (*t >= TK) {
            close_clinic();
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
