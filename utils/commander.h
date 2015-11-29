#ifndef _WISH_COMMANDER
#define _WISH_COMMANDER

/*
 * Gets 255 characters from stdin or until the next newline and runs the command
 * if possible.
 *
 *      Params:
 *          int *should_exit
 *              A pointer to an int that get_and_run_userin can modify if the
 *              command is exit, since if the command is forked to a child
 *              process, "exit" will end the child process instead. If
 *              *should_exit = 0, the loop will not exit.
 *
 *      Returns:
 *          the exit code of the command executed.
 */
int get_and_run_userin(int *should_exit);

#endif

// Define the standard file constants if not defined

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif
