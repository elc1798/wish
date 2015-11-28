#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "commander.h"
#include "dir_man.h"

int execute(char *path, char *arg_arr[]) {
    int pid;
    if (!(pid = fork())) {
        execvp(path, arg_arr);
        // This is only reached if the execvp function fails
        printf("Command [%s] not found.\n", arg_arr[0]);
        exit(-1);
    } else {
        int status;
        wait(&status);
        return status;
    }
}

// COMMAND > FILE
int stdout_to_file_overwrite(char *command[], char *file) {
    printf("PRINTING TO FILE %s", file);
    int stdout_cpy = dup(STDOUT_FD);
    int fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
    dup2(fd, STDOUT_FD);
    int retval = execute(command[0], command);
    dup2(stdout_cpy, fd);
    return retval;
}

// COMMAND >> FILE
int stdout_to_file_append(char *command[], char *file) {
    return 0;
}

// COMMAND 2> FILE
int stderr_to_file_overwrite(char *command[], char *file) {
    return 0;
}

// COMMAND 2>> FILE
int stderr_to_file_append(char *command[], char *file) {
    return 0;
}

// COMMAND 2>1 FILE
int stderr_to_stdout(char *command[], char *file){
    return 0;
}

// COMMAND &>
int stdout_to_stderr(char *command[]) {
    return 0;
}

// COMMAND < FILE
int file_to_stdin(char *command[], char *file) {
    return 0;
}

int get_and_run_userin(int *should_exit) {
    char buff[256];
    fgets(buff, sizeof(buff), stdin);

    // Buff will have space-separated command line arguments, then a newline,
    // then a terminating null. We can use strsep to get rid of the newline.
    // Since fgets will terminate on the first newline, we can be certain there
    // is no next command after it

    char *ptr_to_buff = buff;
    char *line = strsep(&ptr_to_buff, "\n");
    // Blank lines should just return 0, since a blank command doesn't 'crash'
    if (!(*line)) {
        return 0;
    }

    int num_commands = 1;

    int i; for (i = 0; line[i]; i++) {
        if (line[i] == ';') num_commands++;
    }
    char *commands[num_commands];
    for (i = 0; i < num_commands; i++) {
        commands[i] = strsep(&line, ";");
    }

    int retval;
    int counter; for (counter = 0; counter < num_commands; counter++) {

        // Get the number of arguments by counting non space chunks
        int num_args = 0;
        int in_space = 0;
        for (i = 0; commands[counter][i]; i++) {
            if (commands[counter][i] != ' ') {
                if (num_args == 0) {
                    num_args = 1;
                } else if (in_space) {
                    in_space = 0;
                    num_args++;
                }
            }
            if (commands[counter][i] == ' ' && num_args) in_space = 1;
        }

        // Split the chunks by spaces and put into an array of char pointers of
        // size num_args + 1
        char *args[num_args + 1];
        for (i = 0; i < num_args; i++) {
            args[i] = strsep(&commands[counter], " ");
            if (!args[i][0]) i--;
        }
        if (!args[num_args - 1][0]) args[num_args - 1] = NULL;
        args[num_args] = NULL;

        char *command[num_args];
        char *command2[num_args];
        int j;
        int redirected = 0;
        // Check for redirection or pipe symbols
        for (i = 0; i < num_args; i++) {
            if (!strcmp(args[i], ">")) {
                for (j = 0; j < i; j++) {
                    command[j] = args[j];
                }
                command[i] = NULL;
                retval = stdout_to_file_overwrite(command, args[num_args - 1]);
                redirected = 1;
            } else if (!strcmp(args[i], ">>")) {
                for (j = 0; j < i; j++) {
                    command[j] = args[j];
                }
                command[i] = NULL;
                retval = stdout_to_file_append(command, args[num_args - 1]);
                redirected = 1;
            } else if (!strcmp(args[i], "2>")) {
                for (j = 0; j < i; j++) {
                    command[j] = args[j];
                }
                command[i] = NULL;
                retval = stderr_to_file_overwrite(command, args[num_args - 1]);
                redirected = 1;
            } else if (!strcmp(args[i], "2>>")) {
                for (j = 0; j < i; j++) {
                    command[j] = args[j];
                }
                command[i] = NULL;
                retval = stderr_to_file_append(command, args[num_args - 1]);
                redirected = 1;
            } else if (!strcmp(args[i], "&>")) {
                retval = stdout_to_stderr(command);
                redirected = 1;
            } else if (!strcmp(args[i], "<")) {
                for (j = 0; j < i; j++){
                    command[j] = args[j];
                }
                command[i] = NULL;
                retval = file_to_stdin(command, args[num_args - 1]);
                redirected = 1;
            } else if (!strcmp(args[i], "|")) {
                for (j = 0; j < i; j++){
                    command[j] = args[j];
                }
                command[i] = NULL;
                for (j = i + 1; j < num_args; j++){
                    command2[j - i - 1] = args[j];
                }
                command2[j - i] = NULL;
                //retval = piping(command, command2);
                redirected = 1;
            }
        }

        // Special actions for "cd" and "exit"
        if (!strcmp(args[0], "cd")) {
            if (sizeof(args) / sizeof(args[0]) == 2) { // 2 means the array is {"cd", NULL}
                retval = chcwd(expand_path("~"));
            } else {
                retval = chcwd(expand_path(args[1]));
            }
        }

        if (!strcmp(args[0], "exit")) {
            *should_exit = 1;
            return 0;
        }
        if (!redirected) retval = execute(args[0], args);
    }
    return retval;
}

