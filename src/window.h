#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <menu.h>
#include <string.h>

#define WDIMX 50
#define WDIMY 30
#define MAX_LENGTH 128

void init_window();
void end_window();
char **dir_items(char *);
int dir_length(char **);
WINDOW *new_window();
void attach_menu(WINDOW *, MENU *);
void main_program(WINDOW *, char *);

#endif // WINDOW_H