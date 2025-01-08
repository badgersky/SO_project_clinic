#include "patient.h"
#include "sem.h"

void patient_routine(int* p_cnt, int reg_fd[2]) {
    srand(getpid());

    printf("Patient %d entered, total patients: %d\n", getpid(), *p_cnt);

    pid_t pid = getpid();
    printf("patient %d registering\n", pid);
    write(reg_fd[1], &pid, sizeof(pid_t));
	sleep(12);

    sem_wait(door);
    *p_cnt -= 1;
    sem_post(door);
    exit(0);
}

void create_patients(int* p_cnt, int reg_fd[2]) {
	do {
          sem_wait(door);
          if (*p_cnt < MAX_P) {create_patient(p_cnt, reg_fd), *p_cnt += 1;};
          sem_post(door);
          sleep(1);
	} while(1);
}

void create_patient(int* p_cnt, int reg_fd[2]) {
    pid_t p = fork();

    if (p < 0) {perror("fork"); exit(3);}
    if (p == 0) {
        close(reg_fd[0]);
        patient_routine(p_cnt, reg_fd);
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {perror("wait"); exit(3);}
    }
}