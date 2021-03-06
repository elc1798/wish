#ifndef _WISH_PROMPT
#define _WISH_PROMPT

/*
 * The wish shell prompt has its own format characters, much like how bash,
 * zsh, fish, and ksh all have their own formatting characters.
 *
 * %u       - The username of the current user
 * %h       - The hostname
 * %d~n     - The top n levels of the working directory. If n is not provided
 *            then the entire working directory is displayed. (i.e. %d, %d~2)
 *            This assumes that 0 <= n <= 9.
 * %t       - The time in 24 hour format
 * %T       - The time in 12 hour (AM/PM) format
 * %%       - The percent symbol
 *
 *
 * NOTES:
 *
 * If there is a % at the end of the prompt, it will be implied that it is %%.
 * An unrecognized formatting character will be interpretted literally.
 */

char *PROMPT;

//char *get_format_substitution(char *prompt_var);

void load_prompt_defaults();
void prompt(char *current_working_dir);

#endif

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

