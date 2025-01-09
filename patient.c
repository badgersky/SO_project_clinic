#include "patient.h"
#include "sem.h"

void patient_routine(int* reg_q_cnt, int* p_cnt, int reg_fd[2], char** specs, int dr_fd[6][2]) {
	int dr_id = get_dr_id();
  	sem_wait(reg_q);
    *reg_q_cnt += 1;
    sem_post(reg_q);

//    printf("patient %d registering to %s\n", getpid(), specs[dr_id]);
	patient_register(reg_q_cnt, reg_fd);
	go_to_poz(dr_fd);

    sem_wait(door);
    *p_cnt -= 1;
    sem_post(door);
    exit(0);
}

void go_to_poz(int dr_fd[6][2]) {
	pid_t pid = getpid();
	int dr_id = 4 + rand() % 2;

    sem_wait(poz);
    write(dr_fd[dr_id][1], &pid, sizeof(pid_t));
    sem_post(poz);
}

int get_dr_id() {
	srand(getpid());
	int dr_id, i;
    int poz_ids[2] = {4, 5};
  	int rand_int = rand() % 10 + 1;
    if (rand_int < 6) {
    	i = rand() % 2;
        dr_id = poz_ids[i];
    }
    if (rand_int == 6) dr_id = 0;
    if (rand_int == 7) dr_id = 1;
    if (rand_int == 8) dr_id = 2;
    if (rand_int == 9) dr_id = 3;

    return dr_id;
}

void patient_register(int* reg_q_cnt, int reg_fd[2]) {
	srand(getpid());
    pid_t pid = getpid();
    write(reg_fd[1], &pid, sizeof(pid_t));
    sleep(10);
    sem_wait(reg_q);
    *reg_q_cnt -= 1;
    sem_post(reg_q);
}

void create_patients(int* reg_q_cnt, int* p_cnt, int reg_fd[2], char** specs, int dr_fd[6][2]) {
	do {
        sem_wait(door);
        if (*p_cnt < MAX_P) {
    		create_patient(reg_q_cnt, p_cnt, reg_fd, specs, dr_fd),
            *p_cnt += 1;
        };
        sem_post(door);
        sleep(1);
	} while(1);
}

void create_patient(int* reg_q_cnt, int* p_cnt, int reg_fd[2], char** specs, int dr_fd[6][2]) {
    pid_t p = fork();

    if (p < 0) {perror("fork"); exit(3);}
    if (p == 0) {
        close(reg_fd[0]);
        for (int i = 0; i < 6; i++) {
        	close(dr_fd[i][0]);
        }
        patient_routine(reg_q_cnt, p_cnt, reg_fd, specs, dr_fd);
    }
}

void wait_patients() {
    for (int i = 0; i < MAX_P; i++) {
        if (wait(0) < 0) {perror("wait"); exit(3);}
    }
}