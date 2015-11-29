// Grunt is, as the name suggests, an army grunt. Grunt handles all the
// gruntwork that Commander does not want to. It contains a bunch of functions
// that are called by Commander.

#ifndef _WISH_GRUNT
#define _WISH_GRUNT

int execute(char *path, char *arg_arr[]);

int stdout_to_file_overwrite(char *command[], char *file);
int stdout_to_file_append(char *command[], char *file);
int stderr_to_file_overwrite(char *command[], char *file);
int stderr_to_file_append(char *command[], char *file);
int stderr_to_stdout(char *command[]);
int stdout_to_stderr(char *command[]);
int file_to_stdin(char *command[], char *file);

#endif

