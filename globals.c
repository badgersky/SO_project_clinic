#include "globals.h"

sem_t *reg_queue, *clinic_capacity, *rq_lock, *reg_pipe_lock, *drq_lock[6], *dr_queue[6], *dr_pipe_lock[6], *report_lock, *p_cnt_lock, *cs_lock, *rq_capacity, *emergency_lock, *drq_cnt_lock[6];

int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANONYMOUS;

int* rq_cnt;
int* t;
int* clinic_state;
int* p_cnt;

int patient_register[2];
int register_patient[2];

int* dr_limits;
int* dr_p_cnt;
int* drq_cnt;

int patient_doctor[6][2];
int doctor_patient[6][2];

int* emergency;

pid_tracker* pids;
pdrq_tracker* pdrq_pids[6];

FILE* report;

void initialize_pids() {
    pids = (pid_tracker*)mmap(NULL, sizeof(pid_tracker), protection, visibility, -1, 0);
    if (pids == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    pids->pid_lock = (sem_t*)mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (pids->pid_lock == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    sem_init(pids->pid_lock, 1, 1);
    pids->count = 0;
}

void cleanup_pids() {
    sem_destroy(pids->pid_lock);
    if (munmap(pids->pid_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(1);
    }
    if (munmap(pids, sizeof(pid_tracker)) < 0) {
        perror("munmap");
        exit(1);
    };
}

void initialize_pdrq_pids() {
    for (int i = 0; i < DR_NUM; i++) {
        pdrq_pids[i] = (pdrq_tracker*)mmap(NULL, sizeof(pdrq_tracker) * dr_limits[i], protection, visibility, -1, 0);
        if (pdrq_pids == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }

        pdrq_pids[i]->pid_lock = (sem_t*)mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (pdrq_pids[i]->pid_lock == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }

        sem_init(pdrq_pids[i]->pid_lock, 1, 1);
    }
}

void cleanup_pdrq_pids() {
    for (int i = 0; i < DR_NUM; i++) {
        sem_destroy(pdrq_pids[i]->pid_lock);
        if (munmap(pdrq_pids[i]->pid_lock, sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(1);
        }
        if (munmap(pdrq_pids[i], sizeof(pdrq_tracker)) < 0) {
            perror("munmap");
            exit(1);
        };
    }
}

void initialize_sem() {
    reg_queue = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (reg_queue == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    clinic_capacity = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (clinic_capacity == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    rq_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (rq_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    reg_pipe_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (reg_pipe_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    report_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (report_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    p_cnt_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (p_cnt_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    cs_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (cs_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    rq_capacity = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (rq_capacity == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }
    emergency_lock = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    if (emergency_lock == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }

    for (int i = 0; i < DR_NUM; i++) {
        drq_lock[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (drq_lock[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(drq_lock[i], 1, 1);
        
        dr_queue[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (dr_queue[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(dr_queue[i], 1, 1);

        dr_pipe_lock[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (dr_pipe_lock[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(dr_pipe_lock[i], 1, 1);

        drq_cnt_lock[i] = (sem_t*) mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        if (drq_cnt_lock[i] == MAP_FAILED) {
            perror("mmap");
            exit(2);
        }
        sem_init(drq_cnt_lock[i], 1, 1);
    }

    sem_init(reg_queue, 1, 1);
    sem_init(clinic_capacity, 1, MAX_CAPACITY);
    sem_init(rq_lock, 1, 1);
    sem_init(reg_pipe_lock, 1, 1);
    sem_init(report_lock, 1, 1);
    sem_init(p_cnt_lock, 1, 1);
    sem_init(cs_lock, 1, 1);
    sem_init(rq_capacity, 1, MAX_QUEUE);
    sem_init(emergency_lock, 1, 1);
}

void destroy_sem() {
    sem_destroy(reg_queue);
    sem_destroy(clinic_capacity);
    sem_destroy(rq_lock);
    sem_destroy(reg_pipe_lock);
    sem_destroy(report_lock);
    sem_destroy(p_cnt_lock);
    sem_destroy(cs_lock);
    sem_destroy(rq_capacity);
    sem_destroy(emergency_lock);

    for (int i = 0; i < DR_NUM; i++) {
        sem_destroy(drq_lock[i]);
        sem_destroy(dr_queue[i]);
        sem_destroy(dr_pipe_lock[i]);
        if (munmap(drq_lock[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
        if (munmap(dr_queue[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
        if (munmap(dr_pipe_lock[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
        if (munmap(drq_cnt_lock[i], sizeof(sem_t)) < 0) {
            perror("munmap");
            exit(2);
        } 
    }

    if (munmap(reg_queue, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(clinic_capacity, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(rq_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(reg_pipe_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(report_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(p_cnt_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(cs_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(rq_capacity, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
    if (munmap(emergency_lock, sizeof(sem_t)) < 0) {
        perror("munmap");
        exit(2);
    } 
}

void share_variables() {
    rq_cnt = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (rq_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    t = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (t == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    clinic_state = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (clinic_state == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    p_cnt = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (p_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }

    dr_limits = mmap(NULL, sizeof(int) * DR_NUM, protection, visibility, -1, 0);
    if (dr_limits == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    dr_p_cnt = mmap(NULL, sizeof(int) * DR_NUM, protection, visibility, -1, 0);
    if (dr_p_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    emergency = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (emergency == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
    drq_cnt = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    if (drq_cnt == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }
}

void open_report() {
    report = fopen("daily_report", "a");
    if (report == NULL) {
        perror("fopen");
        exit(4);
    }
}

void write_report(char* msg) {
    size_t saved_b;
    open_report();
    saved_b = fwrite(msg, sizeof(char), strlen(msg), report);
    close_report();
}

void close_report() {
    if (fclose(report) != 0) {
        perror("fclose");
        exit(4);
    }

    report = NULL;
}

void init_variables() {
    *rq_cnt = 0;
    *t = 0;
    *clinic_state = 1;
    *p_cnt = 0;
    *emergency = 0;

    dr_limits[0] = X5;
    dr_limits[1] = X4;
    dr_limits[2] = X3;
    dr_limits[3] = X2;
    dr_limits[4] = X1;
    dr_limits[5] = X1;

    for (int i = 0; i < DR_NUM; i++) {
        dr_p_cnt[i] = 0;

        if (pipe(patient_doctor[i]) < 0) {
            perror("pipe");
            exit(4);
        }
        if (pipe(doctor_patient[i]) < 0) {
            perror("pipe");
            exit(4);
        }

        drq_cnt[i] = 0;
    }

    if (pipe(patient_register) < 0) {
        perror("pipe");
        exit(4);
    }
    if (pipe(register_patient) < 0) {
        perror("pipe");
        exit(4);
    }
}

void free_variables() {
    if (munmap(rq_cnt, sizeof(int)) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(t, sizeof(int)) < 0) {
        perror("munmap");
        exit(4);
    } 
    if (munmap(clinic_state, sizeof(int)) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(p_cnt, sizeof(int)) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(dr_limits, sizeof(int) * DR_NUM) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(dr_p_cnt, sizeof(int) * DR_NUM) < 0) {
        perror("munmap");
        exit(4);
    }
    if (munmap(emergency, sizeof(int) * DR_NUM) < 0) {
        perror("munmap");
        exit(4);
    }

    for (int i = 0; i < DR_NUM; i++) {
        close(patient_doctor[i][0]);
        close(patient_doctor[i][1]);
        close(doctor_patient[i][0]);
        close(doctor_patient[i][1]);
    }

    close(patient_register[1]);
    close(register_patient[1]);
    close(patient_register[0]);
    close(register_patient[0]);
}
