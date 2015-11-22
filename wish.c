#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils/prompt.h"
#include "utils/ansi_colors.h"
#include "utils/commander.h"

int main() {
    char cwd[1024];

    load_prompt_defaults();

    while (1) {
        if (!getcwd(cwd, sizeof(cwd))) return -1;
        prompt(cwd);
        get_and_run_userin();
    }
    return 0;
}

