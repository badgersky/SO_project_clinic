#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "patient.h"
#include "director.h"
#include "register.h"

# define P_NUM 10

void print_sem_val(sem_t* sem);

void print_sem_val(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("%d\n", sem_val);
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
