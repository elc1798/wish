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

