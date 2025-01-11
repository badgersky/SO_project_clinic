#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

#define TP 0
#define TK 100

void create_director();
void wait_director();
void director_routine();
void close_clinic();