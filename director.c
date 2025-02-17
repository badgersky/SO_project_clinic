#include "director.h"

int done = 0;

void d_sigusr2_handler(int sig) {
    printf("Director %d received SIGUSR2, forwarding to all patients...\n", getpid());

    sem_wait(emergency_lock);
    *emergency = 1;
    close_clinic();
    sem_post(emergency_lock);

    sem_wait(pids->pid_lock);
    for (int i = 0; i < pids->count; i++) {
        kill(pids->pids[i], SIGUSR2);
    }
    sem_post(pids->pid_lock);

    done = 1;
}

void director_routine() {
    signal(SIGUSR2, d_sigusr2_handler);
    int full;
    for (int j = 0; j < DR_NUM; j++) {
        close(patient_doctor[j][0]);
        close(patient_doctor[j][1]);
        close(doctor_patient[j][0]);
        close(doctor_patient[j][1]);
    }
    for (int j = 0; j < REG_NUM; j++) {
        close(patient_register[j][1]);
        close(register_patient[j][1]);
        close(patient_register[j][0]);
        close(register_patient[j][0]);
    }
    do {
        printf("Director %d, Number of patients inside: %d\n", getpid(), MAX_CAPACITY - get_sem_value(clinic_capacity));
        full = check_limits();
        if (full == 1) {
            printf("No free spots to any doctors\n");
            close_clinic();
            done = 1;
        }

        *t += 1;
        if (*t >= TK) {
            printf("End of clinics worktime\n");
            close_clinic();
            done = 1;
        }
        sleep(1); // ten sleep jest po to zeby logi wygladaly dobrze i przychodnia nie konczyla odrazu pracy
    } while(!done);

    exit(0);
}

void close_clinic() {
    sem_wait(cs_lock);
    if (*clinic_state == 1) {
        printf("closing clinic\n");
        *clinic_state = 0;
        sem_post(cs_lock);
        while (1) {
            if (MAX_CAPACITY - get_sem_value(clinic_capacity) == 0) {
                sem_wait(p_lock);
                if (r_pid[0] != 0) kill(r_pid[0], SIGUSR2);
                if (r_pid[1] != 0) kill(r_pid[1], SIGUSR2);
                for (int i = 0; i < DR_NUM; i++) {
                    if (dr_pids[i] != 0) kill(dr_pids[i], SIGUSR2);
                }
                sem_post(p_lock);
                break;
            }
        }
    } else {
        sem_post(cs_lock);
    }
}

void create_director() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(6);
    }
    if (pid == 0) {
        director_routine();
    }
}

void wait_director() {
    if (wait(0) < 0) {
        perror("wait");
        exit(6);
    }
}
