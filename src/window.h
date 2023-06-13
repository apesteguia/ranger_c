#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <menu.h>
#include <string.h>
#include <stdarg.h>
#include <libgen.h>

#define WDIMX 50
#define WDIMY 30
#define MAX_LENGTH 256

void init_window();
void end_window();
char **dir_items(char *);
int dir_length(char **);
WINDOW *new_window();
void attach_menu(WINDOW *, MENU *);
void main_program(WINDOW *, char *);
void safe_string_concat(char *, const char *, size_t);

#endif // WINDOW_H