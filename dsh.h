/*
 * dsh.h
 *
 * Header file for David Shell (DSH).
 * Defines necessary includes, macros, and function prototypes.
 */

#ifndef DSH_H_
#define DSH_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXBUF 256  // Max number of characters allowed on command line.
#define MAXARGS 64  // Max number of arguments allowed.

// Global variables (if any)
extern int thisIsGlobal;  // Example global variable, delete or replace as needed.

// Function declarations

/**
 * Reads a line of input from the user.
 * @return Dynamically allocated string with the input.
 */
char* read_line(void);

/**
 * Splits the given command line into tokens/arguments.
 * @param line The command line input.
 * @return Dynamically allocated array of tokens/arguments.
 */
char** split_line(char* line);

/**
 * Executes the given command.
 * @param args Null-terminated list of arguments (including the command).
 * @return Status code of the execution.
 */
int execute(char** args);

/**
 * Launches a program and waits for it to terminate.
 * @param args Null-terminated list of arguments (including the program).
 * @return Status code of the launched program.
 */
int launch(char** args);

// Built-in shell command functions

/**
 * Implementation for the 'cd' shell command.
 * @param args List of arguments. args[0] is "cd". args[1] is the directory.
 * @return Always returns 1, to continue executing.
 */
int dsh_cd(char** args);

/**
 * Implementation for the 'help' shell command.
 * @param args List of arguments. Not used.
 * @return Always returns 1, to continue executing.
 */
int dsh_help(char** args);

/**
 * Implementation for the 'exit' shell command.
 * @param args List of arguments. Not used.
 * @return Always returns 0, to terminate execution.
 */
int dsh_exit(char** args);

#endif /* DSH_H_ */
