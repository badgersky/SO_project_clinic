#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define P_NUM 20
# define MAX_P 100
# define REG_NUM 2

void register_routine(int num);
void director_routine();
void patient_routine();

void create_patients();
void create_patient();
void create_registers();
void create_director();

void wait_registers();
void wait_patients();
void wait_director();

void print_sem_val(sem_t* sem);

void print_sem_val(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("%d\n", sem_val);
}

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
    for (int i = 0; i < MAX_P; i++) {
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

void wait_director() {
    if (wait(0) < 0) {
        perror("wait");
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
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
    wait_director();
    wait_patients();
    return 0;
}
