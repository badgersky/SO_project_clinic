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

int main() {
    create_director();
    create_registers();
    create_patients();
    
    wait_registers();
    wait_director();
    return 0;
}
