#include "dsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
//Author @Curtis Alves

// Array of strings representing the built-in command names.
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

// Array of function pointers for built-in functions.
// These functions correspond to the built-in commands.
int (*builtin_func[])(char **) = {
    &cmd_cd,
    &cmd_help,
    &cmd_exit
};

// Returns the number of built-in commands by dividing
// the total size of the `builtin_str` array by the size of one element.
int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Main loop that keeps running the shell.
void loop(void) {
    char *line; // For storing the input line.
    char **args; // For storing the tokenized commands and arguments.
    int status; // For storing the status of the executed command.

    do {
        printf("> "); // Shell prompt.
        line = read_line(); // Read a line of input.
        args = split_line(line); // Split the line into arguments.
        status = execute(args); // Execute the parsed command.

        // Free the allocated memory for the line and args.
        free(line);
        free(args);
    } while (status);
}

// Reads a line of input from the standard input.
char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; // `getline` will allocate a buffer.
    getline(&line, &bufsize, stdin);
    return line;
}

// Splits a line into tokens based on the defined delimiters.
char **split_line(char *line) {
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    // Check for allocation error.
    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Use `strtok` to split the line into tokens.
    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position++] = token;
        if (position >= bufsize) {
            // If we exceed the buffer, reallocate.
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL; // Null-terminate the list of tokens.
    return tokens;
}

// Executes the parsed command.
int execute(char **args) {
    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    // Check if the command is a built-in command.
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    // If not a built-in command, launch the program.
    return launch(args);
}

// Launches a program, possibly in the background.
int launch(char **args) {
    pid_t pid, wpid;
    int status;
    int background = 0; // Flag for background execution.

    // Check for '&' to determine if the command should run in the background.
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "&") == 0) {
            background = 1;
            args[i] = NULL; // Remove '&' so it doesn't get passed to execvp.
        }
    }

    pid = fork(); // Fork a child process.
    if (pid == 0) {
        // Child process.
        if (execvp(args[0], args) == -1) {
            perror("dsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking.
        perror("dsh");
    } else {
        // Parent process.
        if (!background) {
            // If not a background process, wait for the child to finish.
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else {
            // If a background process, print its PID and return immediately.
            printf("[PID %d running in background]\n", pid);
        }
    }

    return 1; // Return to prompt.
}

// Built-in function to change directories.
int cmd_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "dsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("dsh");
        }
    }
    return 1; // Always return 1 to continue execution.
}

// Built-in function to display help.
int cmd_help(char **args) {
    printf("David's Shell (DSH)\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following commands are built in:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1; // Always return 1 to continue execution.
}

// Built-in function to exit the shell.
int cmd_exit(char **args) {
    return 0; // Return 0 to terminate the loop in `loop` function.
}
