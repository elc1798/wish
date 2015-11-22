// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Includes for fancy things
#include <pwd.h>
#include <limits.h>
#include <time.h>
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

void prompt(char *current_working_dir) {
    // Declare variables beforehand since C doesn't allow for variable
    // declarations in labels (case)
    struct passwd *p = getpwuid(getuid());
    char hostname[HOST_NAME_MAX + 1];
    time_t rawtime;
    struct tm *timeinfo;
    // Copy over the working directory because we modify it!
    char working_dir[strlen(current_working_dir) + 1];
    memcpy(working_dir, current_working_dir, strlen(current_working_dir) + 1);
    // Temporary variables
    int j, levels;
    // Set the time variables to system values
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // Run through the string and print the prompt with formatting characters
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
                    if (i + 1 < strlen(PROMPT) &&
                        PROMPT[i + 1] == '~' &&
                        '0' <= PROMPT[i + 2] && PROMPT[i + 2] <= '9') {
                        i += 2; // Increment i
                        // Start from the back and when the number of levels
                        // reaches the desired amount, do some string shiftery
                        levels = 0;
                        for (j = strlen(working_dir);
                             j > 0 && levels < PROMPT[i] - '0';
                             j--) {
                            if (working_dir[j] == '/') levels++;
                        }
                        printf("%s", working_dir + j);
                    } else {
                        printf("%s", working_dir);
                    }
                    break;
                case 't':
                    printf("%d:%d:%d",
                            timeinfo->tm_hour,
                            timeinfo->tm_min,
                            timeinfo->tm_sec);
                    break;
                case 'T':
                    printf("%d:%d:%d %s",
                            (timeinfo->tm_hour % 12 == 0) ?
                            (timeinfo->tm_hour % 12) + 12 :
                            (timeinfo->tm_hour % 12),
                            timeinfo->tm_min,
                            timeinfo->tm_sec,
                            (timeinfo->tm_hour > 11) ?
                            "AM" :
                            "PM"
                          );
                    break;
                case '%':
                    printf("%%");
                    break;
                default:
                    printf("%c", PROMPT[i]);
                    break;
            }
        } else {
            printf("%c", PROMPT[i]);
        }
    }
}

