#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"
//Author @Curtis Alves
#define MAX_PROC 512

int main(int argc, char *argv[]) {

    // Fork bomb prevention block

    struct rlimit limit;
    limit.rlim_cur = MAX_PROC;
    limit.rlim_max = MAX_PROC;
    setrlimit(RLIMIT_NPROC, &limit);

    char *cmdline = (char*) malloc(MAXBUF); // Utilizes MAXBUF from dsh.h

    // Here, ideally, you would call loop() or other shell initialization functions
    // Since modifications to main.c are off-limits, ensure loop() or its equivalent is triggered elsewhere

    free(cmdline); // Ensure allocated memory is freed before exiting
    return 0;
}
