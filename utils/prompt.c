#ifndef _WISH_PROMPT
#define _WISH_PROMPT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "ansi_colors.h"

char *PROMPT;
char **PROMPT_VARS;
char *RPROMPT;
char **RPROMPT_VARS;

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

