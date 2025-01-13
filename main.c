#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "globals.h"
#include "patient.h"
#include "doctor.h"
#include "director.h"

int main() {
    initialize_sem();
    share_variables();
    init_variables();

    create_director();
    create_registers();
    create_doctors();
    create_patients();

    wait_patients();
    wait_doctors();
    wait_registers();
    wait_director();

    free_variables();
    destroy_sem();
    return 0;
}