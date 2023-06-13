#include "window.h"

int dir_length(char **dirs)
{
    int i;
    i = 0;
    while (dirs[i] != NULL)
        i++;
    return i;
}

char **dir_items(char *path)
{
    DIR *dir = opendir(path);
    char **current_dirs;
    struct dirent *entry;

    current_dirs = (char **)malloc(MAX_LENGTH * sizeof(char *));
    if (dir == NULL)
    {
        printf("Failed to open directory.\n");
        return NULL;
    }

    int i = 0;
    while ((entry = readdir(dir)) != NULL && i < MAX_LENGTH)
    {
        char *name = entry->d_name;
        current_dirs[i] = (char *)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(current_dirs[i], name);
        i++;
    }
    current_dirs[i] = NULL;

    closedir(dir);

    return current_dirs;
}

void free_dir_items(char **current_dirs)
{
    int i;
    if (current_dirs == NULL)
        return;

    i = 0;
    while (current_dirs[i] != NULL)
    {
        free(current_dirs[i]);
        i++;
    }
    free(current_dirs);
}

void free_dir(char **dirs)
{
    int i;
    if (dirs == NULL)
        return;
    i = 0;
    while (dirs[i] != NULL)
    {
        free(dirs[i]);
        i++;
    }
    free(dirs);
}

void init_window()
{
    initscr();
    cbreak();
    noecho();
    start_color();
    curs_set(0);
    init_pair(1, COLOR_BLACK, COLOR_BLUE);
    refresh();
}

WINDOW *new_window()
{
    WINDOW *win;
    int start_x, start_y;

    getmaxyx(stdscr, start_y, start_x);
    start_x = (start_x - WDIMX) / 2;
    start_y = (start_y - WDIMY) / 2;
    win = newwin(WDIMY, WDIMX, start_y, start_x);
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(1));
    wrefresh(win);
    return win;
}

void main_program(WINDOW *win, char *path)
{
    MENU *menu;
    ITEM **menu_items;
    char **dirs, c;
    int length;
    int i;

    dirs = dir_items(path);
    length = dir_length(dirs);
    menu_items = (ITEM **)malloc((length + 1) * sizeof(ITEM *));
    for (int i = 0; i < length; i++)
        menu_items[i] = new_item(dirs[i], "");
    menu_items[length] = NULL;

    menu = new_menu(menu_items);
    attach_menu(win, menu);
    wrefresh(win);
    refresh();

    i = 0;
    while ((c = wgetch(win)) != KEY_F(1))
    {
        switch (c)
        {
        case 'j':
            menu_driver(menu, REQ_DOWN_ITEM);
            if (i > length)
                i = length;
            else
                i++;
            wrefresh(win);
            break;
        case 'k':
            menu_driver(menu, REQ_UP_ITEM);
            if (i < 0)
                i = 0;
            else
                i--;
            wrefresh(win);
            break;
        case '\n':
            unpost_menu(menu);
            free_menu(menu);
            free_dir_items(menu_items);
            free_dir(dirs);

            dirs = dir_items(path);
            length = dir_length(dirs);
            menu_items = (ITEM **)malloc((length + 1) * sizeof(ITEM *));
            for (int i = 0; i < length; i++)
                menu_items[i] = new_item(dirs[i], "");
            menu_items[length] = NULL;

            menu = new_menu(menu_items);
            attach_menu(win, menu);
            wrefresh(win);
            break;
        case 'q':
            end_window(win);
            exit(-1);
            break;
        }
    }
}

void attach_menu(WINDOW *win, MENU *m)
{
    set_menu_win(m, win);
    set_menu_sub(m, derwin(win, 25, 40, 2, 1));
    set_menu_format(m, 25, 1);
    set_menu_back(m, COLOR_PAIR(1));
    post_menu(m);
    wrefresh(win);
}

void end_window(WINDOW *win)
{
    sleep(4);
    delwin(win);
    endwin();
}