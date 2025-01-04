#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define P_NUM 10
# define REG_NUM 2

void register_routine(int num);
void director_routine();
void patient_routine();

void create_patients();
void create_patient();
void create_registers();
void create_director();

void wait_registers();

void register_routine(int num) {
    while(1) {
        sleep(1);
        printf("register %d\n", num);
    }

    exit(0);
}

void director_routine() {
    while(1) {
        sleep(1);
        printf("director\n");
    }

    exit(0);
}

void patient_routine() {
    for (int i = 0; i < 5; i++) {
        sleep(1);
        printf("patient %d\n", getpid());
    }

    exit(0);
}

void create_patients() {
    while(1) {
        sleep(2);
        create_patient();
    }
}

void create_patient() {
    pid_t p = fork();

    if (p < 0) {
        perror("fork");
    }
    if (p == 0) {
        patient_routine();
    }
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

int main() {
    create_director();
    create_registers();
    create_patients();
    wait_registers();

    return 0;
}
