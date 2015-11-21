#ifndef _WISH_PROMPT
#define _WISH_PROMPT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "ansi_colors.h"

char *PROMPT = "%s@%s (%s):$ ";
char *RPROMPT = "";

void prompt() {
}

#endif

