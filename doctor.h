#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void doctor_routine(int type);

void poz_doctor_routine();

void oculist_routine();

void cardiologist_routine();

void pediatrician_routine();

void occup_med_doctor_routine();

void create_doctor(int type);

void wait_doctors();