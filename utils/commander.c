#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "commander.h"
#include "grunt.h"

int get_num_args(char *line) {
    // Get the number of arguments by counting non space chunks
    int num_args = 0;
    int in_space = 0;
    int i; for (i = 0; line[i]; i++) {
        if (line[i] != ' ') {
            if (num_args == 0) {
                num_args = 1;
            } else if (in_space) {
                in_space = 0;
                num_args++;
            }
        }
        if (line[i] == ' ' && num_args) in_space = 1;
    }
    return num_args;
}

int get_and_run_userin() {
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
        if (!(commands[counter][0])) {
            retval = 0;
            continue;
        }
        // Get the number of arguments from get_num_args
        int num_args = get_num_args(commands[counter]);

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
            } else if (!strcmp(args[i], "2>1")) {
                for (j = 0; j < i; j++) {
                    command[j] = args[j];
                } 
                command[i] = NULL;
                retval = stderr_to_stdout(command);
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
                char *command1[i + 1];
                char *command2[num_args - i + 1];
                for (j = 0; j < i; j++){
                    command1[j] = args[j];
                } 
                command1[i] = NULL;

                for (j = i + 1; j <= num_args; j++){
                    command2[j - i - 1] = args[j];
                }
                retval = pipe_of_wish(command1, command2);
                redirected = 1;
            } 
        }

        if (!redirected) retval = execute(args[0], args);
    }
    return retval;
}
