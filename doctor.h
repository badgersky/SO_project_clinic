#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>

#define MAX_SPEC_CHAR 25

void doctor_routine(int type, char* spec, int dr_fd[6][2]);

void poz_doctor_routine(char* spec, int dr_fd[2]);

void specialist_routine(char* spec, int dr_fd[2]);

void create_doctor(int type, char* spec, int dr_fd[6][2]);

char** get_specializations();

void free_specializations(char** specs);

void wait_doctors();