#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "register.h"
#include "globals.h"
#include "patient.h"
#include "doctor.h"
#include "director.h"

int main() {
    initialize_sem();
    share_variables();
    init_variables();
    initialize_pids();

    create_director();
    create_registers();
    create_doctors();
    create_patients();

    pthread_t wait_t;
    pthread_create(&wait_t, NULL, *wait_for_processes, NULL);
    pthread_join(wait_t, NULL);

    cleanup_pids();
    free_variables();
    destroy_sem();
    return 0;
}