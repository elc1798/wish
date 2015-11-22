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
    // Run through the string and print the prompt with formatting characters
    struct passwd *p = getpwuid(getuid());
    char hostname[HOST_NAME_MAX + 1];
    int i; for (i = 0; i < strlen(PROMPT); i++) {
        if (PROMPT[i] == '%' && i + 1 < strlen(PROMPT)) {
            switch(PROMPT[++i]) {
                case 'u':
                    if (!p) {
                        printf("unknown");
                    } else {
                        printf("%s", p->pw_name);
                    }
                    break;
                case 'h':
                    gethostname(hostname, sizeof(hostname));
                    if (errno) {
                        printf("unknown");
                    } else {
                        printf("%s", hostname);
                    }
                    break;
                case 'd':
                case 't':
                case 'T':
                case '%':
                default:
            }
        }
    }
    free(p);
}

