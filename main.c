#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "sem.h"
#include "patient.h"

int main() {
    initialize_sem();

    create_registers();
    create_patients();

    wait_registers();
    wait_patients();

    destroy_sem();
    return 0;
}