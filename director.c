#include "director.h"

void director_routine() {
    int full, done = 0;
    
    do {
        full = check_limits();
        if (full == 1) {
            printf("No free spots to any doctors\n");
            close_clinic();
            done = 1;
        }

        *t += 1;
        if (*t >= TK) {
            printf("End of clinics worktime\n");
            close_clinic();
            done = 1;
        }
        sleep(1);
    } while(!done);

    exit(0);
}

void close_clinic() {
    sem_wait(cs_lock);
    if (*clinic_state == 1) {
        printf("closing clinic\n");
        *clinic_state = 0;
    }
    sem_post(cs_lock);
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
