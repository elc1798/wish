#ifndef _WISH_COMMANDER
#define _WISH_COMMANDER

/*
 * Gets 255 characters from stdin or until the next newline and runs the command
 * if possible.
 *
 *      Returns:
 *          the exit code of the command executed.
 */
int get_and_run_userin();

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
