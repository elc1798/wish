#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wordexp.h>

#include "dir_man.h"

int chcwd(char *path) {
    return chdir(path);
}

char *expand_path(char *path) {
    wordexp_t expanded;
    wordexp(path, &expanded, 0);
    return expanded.we_wordv[0];
}

