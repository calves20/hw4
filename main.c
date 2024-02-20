/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "dsh.h"

#define MAX_PROC 400

int main(int argc, char *argv[]) {
    char *cmdline;
    char **args;
    int status;

    // Fork bomb prevention
    struct rlimit limit;
    limit.rlim_cur = MAX_PROC;
    limit.rlim_max = MAX_PROC;
    setrlimit(RLIMIT_NPROC, &limit);

    do {
        printf("dsh> ");  // Print command prompt
        cmdline = (char*) malloc(MAXBUF); // Allocate memory for the command line input

        if (!cmdline) {
            fprintf(stderr, "Allocation error\n");
            exit(EXIT_FAILURE);
        }\

        // Read input from command line
        if (fgets(cmdline, MAXBUF, stdin) == NULL) {
            // Handle EOF or read error
            free(cmdline);
            continue; // Skip the rest of the loop iteration
        }

        // Parse input into commands and parameters
        args = split_line(cmdline);
        
        // Execute parsed command
        status = execute(args);

        // Free allocated memory
        free(cmdline);
        free(args);
    } while (status); // Continue until "exit" command is executed

    return EXIT_SUCCESS;
}

