// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Includes for fancy things
#include <pwd.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>

// Includes for wish header files
#include "ansi_colors.h"
#include "prompt.h"

void load_prompt_defaults() {
    *PROMPT = "%s@%s (%s):$ ";
    *PROMPT_VARS = malloc(3 * sizeof(char *));
    *PROMPT_VARS[0] = "_USERNAME";
    *PROMPT_VARS[1] = "_HOSTNAME";
    *PROMPT_VARS[2] = "_CWD";
}

void load_prompt() {
}

void prompt() {
}

#endif

