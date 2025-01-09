#include "doctor.h"

void doctor_routine(int type, char* spec, int dr_fd[6][2]) {
    if (type > 3) poz_doctor_routine(spec, dr_fd[type]);
    else specialist_routine(spec, dr_fd[type]);

    exit(0);
}

void create_doctor(int type, char* spec, int dr_fd[6][2]) {
    pid_t pid = fork();

    if (pid < 0) {perror("fork"); exit(1);}
    if (pid == 0) {
        doctor_routine(type, spec, dr_fd);
    }
}

void poz_doctor_routine(char* spec, int dr_fd[2]) {
	close(dr_fd[1]);
    do {
		pid_t pid;
        read(dr_fd[0], &pid, sizeof(pid_t));
        printf("%s examining patient %d\n", spec, pid);
        sleep(2);
    } while(1);
}

void specialist_routine(char* spec, int dr_fd[2]) {
    do {
//        printf("%s\n", spec);
        sleep(2);
    } while(1);
}

char** get_specializations() {
    char** specs = (char**) mmap(NULL, sizeof(char*) * 6, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (specs == MAP_FAILED) {perror("mmap"); exit(1);}

    for (int i = 0; i < 6; i++) {
        specs[i] = (char*) mmap(NULL, MAX_SPEC_CHAR, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (specs[i] == MAP_FAILED) {perror("mmap"); exit(1);}
    }

    strncpy(specs[0], "cardiologist", MAX_SPEC_CHAR);
    strncpy(specs[1], "oculist", MAX_SPEC_CHAR);
    strncpy(specs[2], "pediatrist", MAX_SPEC_CHAR);
    strncpy(specs[3], "job medicine doctor", MAX_SPEC_CHAR);
    strncpy(specs[4], "poz doctor 1", MAX_SPEC_CHAR);
    strncpy(specs[5], "poz doctor 2", MAX_SPEC_CHAR);

    return specs;
}

void free_specializations(char** specs) {
    for (int i = 0; i < 4; i++) {
        if (munmap(specs[i], MAX_SPEC_CHAR) < 0) {perror("munmap"); exit(1);}
    }

    if (munmap(specs, sizeof(char*) * 6) < 0) {perror("munmap"); exit(1);}
}

void wait_doctors() {
    for (int i = 0; i < 6; i++) {
        if (wait(0) < 0) {perror("wait"); exit(1);}
    }
}