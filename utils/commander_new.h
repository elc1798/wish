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

typedef struct wcommand {
    int instream;           // Bytestream to read from
    int outstream;          // Bytestream to write to
    int errstream;          // Bytestream to send errors to
    char *args[256];        // Array of char pointers for command arguments
    int is_piped;           // 1 if being piped to another command, 0 otherwise
    struct wcommand *next;  // The next command to go to. NULL if no commands left
} wcommand;

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
