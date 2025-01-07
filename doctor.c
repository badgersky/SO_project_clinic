#include "doctor.h"

void doctor_routine(int type) {
    switch (type) {
        case 1:
            poz_doctor_routine();
        case 2:
            oculist_routine();
        case 3:
            cardiologist_routine();
        case 4:
            pediatrician_routine();
        case 5:
            occup_med_doctor_routine();
        break;
    }
}

void create_doctor(int type) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
    }
    if (pid == 0) {
        doctor_routine(type);
    }
}

void poz_doctor_routine() {
    printf("poz doctor\n");
}

void oculist_routine() {
    printf("oculist\n");
}

void cardiologist_routine() {
    printf("cardiologist\n");
}

void pediatrician_routine() {
    printf("pediatrician\n");
}

void occup_med_doctor_routine() {
    printf("occupational medicine doctor\n");
}