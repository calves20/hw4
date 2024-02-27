#ifndef DSH_H_
#define DSH_H_
//Author @Curtis Alves
#define MAX_LINE 1024
#define MAXBUF 1024    // Maximum buffer size for command line input
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

// Function prototypes that need to be implemented in dsh.c
void initialize_shell(void); // Placeholder for any initialization if needed
void loop(void); // Main loop for processing commands
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int launch(char **args);

// Built-in shell command functions
int cmd_cd(char **args);
int cmd_help(char **args);
int cmd_exit(char **args);

// Global variables for built-in commands
extern char *builtin_str[];
extern int (*builtin_func[])(char **);

int num_builtins(void);

#endif /* DSH_H_ */
