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
    PROMPT = "%u@%h: %d:$ ";
}

void load_prompt_config() {
}

void prompt() {
}

