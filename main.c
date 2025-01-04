#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

# define P_NUM 10
# define REG_NUM 2



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

void creeate_registers() {
    pid_t* reg = malloc(sizeof(pid_t) * 2);

    for (int i = 0; i < REG_NUM; i++) {
        reg[i] = fork();
        printf("%d\n", reg[i]);

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
    creeate_registers();
    director_routine();
    wait_registers();

    return 0;
}