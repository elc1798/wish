#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils/prompt.h"
#include "utils/ansi_colors.h"
#include "utils/commander.h"
#include "utils/grunt.h"

int main() {
    char cwd[1024];

    load_prompt_defaults();
    should_exit = 0;

    while (!(should_exit)) {
        if (!getcwd(cwd, sizeof(cwd))) return -1;
        prompt(cwd);
        get_and_run_userin(should_exit);
    }
    return 0;
}

