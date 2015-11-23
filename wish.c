#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils/prompt.h"
#include "utils/ansi_colors.h"
#include "utils/commander.h"

int main() {
    char cwd[1024];
    int *should_exit = malloc(sizeof(int));
    *should_exit = 0; // 0 means should not exit

    load_prompt_defaults();

    while (!(*should_exit)) {
        if (!getcwd(cwd, sizeof(cwd))) return -1;
        prompt(cwd);
        get_and_run_userin(should_exit);
    }
    free(should_exit);
    return 0;
}

