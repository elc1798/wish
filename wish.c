#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"
#include "ansi_colors.h"
#include "commander.h"

int main() {
    char cwd[1024];
    if (!getcwd(cwd, sizeof(cwd))) return -1;

    load_prompt_defaults();

    while (1) {
        prompt(cwd);
        get_and_run_userin();
    }
    return 0;
}

