#ifndef _WISH_DIRMAN
#define _WISH_DIRMAN

#define DIR_EXISTS 1
#define DIR_NEXISTS 0

/*
 * int chcwd(char *path)
 *      Changes the current working directory to the directory given by `path`
 *      Returns 0 if successful, -1 otherwise.
 */
int chcwd(char *path);

/*
 * char *expand_path(char *path)
 *      Returns a char pointer to a POSIX compliant shell expansion path version
 *      of the supplied `path` parameter
 */
char *expand_path(char *path);

#endif

