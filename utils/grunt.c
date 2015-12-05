// See 'grunt.h' for explanation of what this file is.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "grunt.h"
#include "dir_man.h"

int execute(char *path, char *arg_arr[]) {
    // Special action for "cd"
    if (!strcmp(arg_arr[0], "cd")) {
        int i = 0;
        if (!arg_arr[1]) {
            return chcwd(expand_path("~"));
        } else {
            int retval = chcwd(expand_path(arg_arr[1]));
            if (retval) {
                fprintf(stderr, "Directory [%s] not found.\n", arg_arr[1]);
            }
            return retval;
        }
    }
    // Special action for "exit"
    if (!strcmp(arg_arr[0], "exit")) {
        should_exit = 1;
        return 0;
    }

    // Normal execution
    int pid;
    if (!(pid = fork())) {
        execvp(path, arg_arr);
        // This is only reached if the execvp function fails
        // Print to stderr
        fprintf(stderr, "Command [%s] not found.\n", arg_arr[0]);
        // Exit with non zero status
        exit(-1);
    } else {
        int status;
        wait(&status);
        return status;
    }
}

// COMMAND > FILE
int stdout_to_file_overwrite(char *command[], char *file) {
    // Create a copy of STDOUT
    int stdout_cpy = dup(STDOUT_FILENO);
    // Close STDOUT so it's file descriptor can be used
    close(STDOUT_FILENO);
    // Create a file with TRUNC and WRITE perms to replace STDOUT
    int fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
    // dup fd into STDOUT_FILENO
    dup2(fd, STDOUT_FILENO);
    // Get a return value from the command
    int retval = execute(command[0], command);
    // Close the file descriptor
    close(fd);
    // dup the backup of STDOUT into STDOUT_FILENO
    dup2(stdout_cpy, STDOUT_FILENO);
    // Close the STDOUT backup to free up the file table
    close(stdout_cpy);
    return retval;
}

// COMMAND >> FILE
int stdout_to_file_append(char *command[], char *file) {
    // Create copy of STDOUT
    int stdout_cpy = dup(STDOUT_FILENO);
    // Close STDOUT from file table to let it's fd be used.
    close(STDOUT_FILENO);
    // Open the file for appending
    int fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
    // dup the file into STDOUT_FILENO
    dup2(fd, STDOUT_FILENO);
    // Get return value from command
    int retval = execute(command[0], command);
    // Close the file descriptor
    close(fd);
    // dup the stdout backup into STDOUT_FILENO
    dup2(stdout_cpy, STDOUT_FILENO);
    // Close the STDOUT backup
    close(stdout_cpy);
    return retval;
}

// COMMAND 2> FILE
// Works the same way as stdout_to_file_overwrite, just replace STDOUT with
// STDERR
int stderr_to_file_overwrite(char *command[], char *file) {
    int stderr_cpy = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    int fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
    dup2(fd, STDERR_FILENO);
    int retval = execute(command[0], command);
    close(fd);
    dup2(stderr_cpy, STDERR_FILENO);
    close(stderr_cpy);
    return retval;
}

// COMMAND 2>> FILE
// Works the same way as stdout_to_file_append, just replace STDOUT with STDERR
int stderr_to_file_append(char *command[], char *file) {
    int stderr_cpy = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    int fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
    dup2(fd, STDERR_FILENO);
    int retval = execute(command[0], command);
    close(fd);
    dup2(stderr_cpy, STDERR_FILENO);
    close(stderr_cpy);
    return retval;
}

// COMMAND 2>1
int stderr_to_stdout(char *command[]) {
    // Create backup of stdout and stderr
    int stdout_cpy = dup(STDOUT_FILENO);
    int stderr_cpy = dup(STDERR_FILENO);
    // Close STDERR only! STDERR should > to STDOUT, but STDOUT should still
    // function normally!
    close(STDERR_FILENO);
    // Dup the *COPY* of stdout to STDERR_FILENO. This leaves STDOUT intact!
    dup2(stdout_cpy, STDERR_FILENO);
    // Get return value from command
    int retval = execute(command[0], command);
    // Close the copy of stdout, which is currently at STDERR_FILENO
    close(stdout_cpy);
    // dup the copy of stderr back into STDERR_FILENO
    dup2(stderr_cpy, STDERR_FILENO);
    // Close the copy of stderr
    close(stderr_cpy);
    return retval;
}

// COMMAND &>
int stdout_to_stderr(char *command[]) {
    // Create backup of stdout and stderr
    int stdout_cpy = dup(STDOUT_FILENO);
    int stderr_cpy = dup(STDERR_FILENO);
    // Close STDOUT only! STDOUT should > to STDERR, but STDERR should still
    // function normally!
    close(STDOUT_FILENO);
    // Dup the *COPY* of stderr to STDOUT_FILENO. This leaves STDERR intact!
    dup2(stderr_cpy, STDOUT_FILENO);
    // Get return value from command
    int retval = execute(command[0], command);
    // Close the copy of stderr, which is currently at STDOUT_FILENO
    close(stderr_cpy);
    // dup the copy of stdout back into STDOUT_FILENO
    dup2(stdout_cpy, STDOUT_FILENO);
    // Close the copy of stdout
    close(stdout_cpy);
    return retval;
}

// COMMAND < FILE
int file_to_stdin(char *command[], char *file) {
    int retval = 0;

    // Create a copy of STDIN
    int stdin_cpy = dup(STDIN_FILENO);
    // Close STDIN so it's file descriptor can be used
    close(STDIN_FILENO);
    // Open the file
    int fd = open(file, O_RDONLY);

    // Only execute if file exists!
    if (fd != -1) {
        // dup fd into STDIN_FILENO
        dup2(fd, STDIN_FILENO);
        // Get a return value from the command
        retval = execute(command[0], command);
    } else {
        fprintf(stderr, "File \"%s\" not found.\n", file);
        retval = -1;
    }
    // Close the file descriptor
    close(fd);
    // dup the backup of STDIN into STDIN_FILENO
    dup2(stdin_cpy, STDIN_FILENO);
    // Close the STDIN backup to free up the file table
    close(stdin_cpy);
    return retval;
}

// COMMAND 1 OUTPUT > COMMAND 2 INPUT
/*
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
,,,,,####################################,,,,
,,,,,@'+++'++++++++++++++++++++++++++++##,,,,
,,,,,@;'''''';'';;'''''''''''''''''''''+#,,,,
,,,,,@++++++;;;;;;+++++++++++++++++++++#@,,,,
,,,,,@;;;'++;;';;;++;'+++++++++++;+'+';'#,,,,
,,,,,@:;;'++;;;;;;++;'++++++++++++'+';''@,,,,
,,,,,@:;;'++;;;;;;++;'+++++++++++;+;+';'#,,,,
,,,,,@:;;'++;;;;;;++;'++++++++++++'+';''#,,,,
,,,,,@:;;'++;;;;;;++;'+++++++++++;+'+';'#,,,,
,,,,,@:;;'++;;;;;;++;'++++++++++++'+';;'#,,,,
,,,,,@:;;'++;;;;;;++;'+++++++++++'+'+';'#,,,,
,,,,,@:;;'++;';';;++;;++++++++++++'+';;'@,,,,
,,,,,@;';'++;;;;;;++;'++++++++++++'+';;'#,,,,
,,,,,@:;;'++;''';'++''+++++++++++'+'+';'#,,,,
,,,,,@:;;'++;;;;;;++;;++++++++++++;+;;;'#,,,,
,,,,,@:;;'++;;;;;;++;'+++++++++++;+;+';'#:,,,
,,,,,#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#,,,,
,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;,,,,,,
,,,,,,,@';;;++;;;;;'+;;+++++++++:+;;;@;,,,,,,
,,,,,,,@';;;++;;;;;'+;'++++++++++'+;;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;'+++++++++'+';'@;,,,,,,
,,,,,,,@'';;++;;';;'+;;++++++++++'+;;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;++++++++++'+;;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;+++++++++'+';'@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;++++++++++;+;;@;,,,,,,
,,,,,,,@';;'++;;    WISH   +++++'+;;;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;++++++++++'+;'@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;+++++++++;+;;;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;++++++++++'+;'@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;+++++++++'+';;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;++++++++++'+';@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;+++++++++'+';;@;,,,,,,
,,,,,,,@';;'++;;;;;'+;;+++++++++'+';'@;,,,,,,
,,,,,,,@';;'++;;;;;'+;'++++++++++'+;;@;,,,,,,
,,,,,,,@'';'++;;;;;'+;;+++++++++;+;;;@;,,,,,,
,,:,,,:@';;'++;;;;;'+;;++++++++++;+;;@;:,,,,,
....''.........@'.....''.........@'....,;'...
''''@.'''''''''@`'''';@.'''''''';@.''''+@.'''
''''@`'''''''''@`'''''@.'''''''''@.''''+@.'''
''''@`'''''''''@`'''''@.'''''''';@.''''+@.'''
#;''@.'''''''''@.#+'''@.'''''''''@.@'''+@.'''
@###+.'''''''''@:@####+.'''''''''@:@####+.'''
++++'.'''''''''@:++++++.'''''''''@;++++++.'''
*/
int pipe_of_wish(char *command1[], char *command2[]) {
    int fds[2];
    pipe(fds);
    int pid;
    int stdout_cpy, stdin_cpy;
    if (!(pid = fork())) {
        close(fds[0]);
        stdout_cpy = dup(STDOUT_FILENO);
        dup2(fds[1], STDOUT_FILENO);
        exit(execute(command1[0], command1));
    } else {
        close(fds[1]);
        int status;
        wait(&status);
        stdin_cpy = dup(STDIN_FILENO);
        dup2(fds[0], STDIN_FILENO);
        execute(command2[0], command2);
        close(fds[0]);
        dup2(stdin_cpy, STDIN_FILENO);
        close(stdin_cpy);
        return status;
    }
}
