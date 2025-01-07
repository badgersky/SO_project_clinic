#include "doctor.h"

void doctor_routine() {
    printf("doctor\n");
}

void create_doctor() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
    }
    if (pid == 0) {
        doctor_routine();
    }
}