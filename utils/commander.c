#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
        exit(-1);
    } else {
        int status;
        wait(&status);
        return status;
    }
}

int get_and_run_userin(int *should_exit) {
    char buff[256];
    fgets(buff, sizeof(buff), stdin);

    // Buff will have space-separated command line arguments, then a newline,
    // then a terminating null. We can use strsep to get rid of the newline.
    // Since fgets will terminate on the first newline, we can be certain there
    // is no next command after it

    char *ptr_to_buff = buff;
    char *command = strsep(&ptr_to_buff, "\n");
    // Blank lines should just return 0, since a blank command doesn't 'crash'
    if (!(*command)) {
        return 0;
    }
    int num_args = 1;
    int i; for (i = 0; command[i]; i++) {
        if (command[i] == ' ') num_args++;
    }
    char *args[num_args + 1];
    for (i = 0; i < num_args; i++) {
        args[i] = strsep(&command, " ");
    }
    args[num_args] = NULL;

    // Special actions for "cd" and "exit"
    if (!strcmp(args[0], "cd")) {
        if (sizeof(args) / sizeof(args[0]) == 2) { // 2 means the array is {"cd", NULL}
            return chcwd(expand_path("~"));
        } else {
            return chcwd(expand_path(args[1]));
        }
    }

    if (!strcmp(args[0], "exit")) {
        *should_exit = 1;
        return 0;
    }

    return execute(args[0], args);
}

