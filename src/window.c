#include "window.h"

int dir_length(char **dirs)
{
    int i = 0;
    while (dirs[i] != NULL)
        i++;
    return i;
}

int compare_strings(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
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
    qsort(current_dirs, i, sizeof(current_dirs[0]), compare_strings);

    return current_dirs;
}

void free_dir_items(char **current_dirs)
{
    if (current_dirs == NULL)
        return;

    int i = 0;
    while (current_dirs[i] != NULL)
    {
        free(current_dirs[i]);
        i++;
    }
    free(current_dirs);
}

void free_dir(char **dirs)
{
    if (dirs == NULL)
        return;

    int i = 0;
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
    char **dirs, c, *path2;
    int length;
    int i;
    bool b = true;

    dirs = dir_items(path);
    length = dir_length(dirs);
    menu_items = (ITEM **)malloc((length + 1) * sizeof(ITEM *));
    for (i = 0; i < length; i++)
        menu_items[i] = new_item(dirs[i], "");
    menu_items[length] = NULL;

    menu = new_menu(menu_items);
    attach_menu(win, menu);
    wrefresh(win);

    i = 0;
    while ((c = wgetch(win)) != KEY_F(1))
    {
        switch (c)
        {
        case 'j':
            menu_driver(menu, REQ_DOWN_ITEM);
            i = (i + 1) % length;
            wrefresh(win);
            break;
        case 'k':
            menu_driver(menu, REQ_UP_ITEM);
            i = (i - 1 + length) % length;
            wrefresh(win);
            break;
        case '\n':
            path2 = (char *)malloc(MAX_LENGTH * sizeof(char));
            getcwd(path2, MAX_LENGTH);
            if (!b)
            {
                strncat(path2, "/", MAX_LENGTH);
                strncat(path2, dirs[i], MAX_LENGTH);
            }
            else
            {
                chdir(path);
                getcwd(path2, MAX_LENGTH);
                strncat(path2, "/", MAX_LENGTH);
                b = false;
            }

            unpost_menu(menu);
            free_menu(menu);
            free_dir_items(menu_items);
            free_dir(dirs);
            dirs = dir_items(path2);
            chdir(path2);
            free(path2);
            path2 = "";
            length = dir_length(dirs);
            menu_items = (ITEM **)malloc((length + 1) * sizeof(ITEM *));
            for (i = 0; i < length; i++)
                menu_items[i] = new_item(dirs[i], "");
            menu_items[length] = NULL;
            menu = new_menu(menu_items);
            attach_menu(win, menu);
            wrefresh(win);
            i = 0;
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
    delwin(win);
    endwin();
}

/*
#include "window.h"

int dir_length(char **dirs)
{
    int i;
    i = 0;
    while (dirs[i] != NULL)
        i++;
    return i;
}
int compare_strings(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
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
    qsort(current_dirs, i, sizeof(current_dirs[0]), compare_strings);

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
    char **dirs, c, *path2, aux[MAX_LENGTH];
    int length;
    int i;
    bool b;

    dirs = dir_items(path);
    length = dir_length(dirs);
    menu_items = (ITEM **)malloc((length + 1) * sizeof(ITEM *));
    for (int i = 0; i < length; i++)
        menu_items[i] = new_item(dirs[i], "");
    menu_items[length] = NULL;

    menu = new_menu(menu_items);
    attach_menu(win, menu);
    wrefresh(win);

    i = 0;
    b = true;
    refresh();
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
            path2 = (char *)malloc(MAX_LENGTH * sizeof(char));
            getcwd(path2, MAX_LENGTH);
            if (b == false)
            {
                printf("%d", i);
                strncat(path2, "/", MAX_LENGTH);
                strncat(path2, dirs[i], MAX_LENGTH);
            }
            else if (b == true)
            {
                chdir(path);
                getcwd(path2, MAX_LENGTH);
                strncat(path2, "/", MAX_LENGTH);
                b = false;
            }

            unpost_menu(menu);
            free_menu(menu);
            free_dir_items(menu_items);
            free_dir(dirs);
            dirs = dir_items(path2);
            chdir(path2);
            free(path2);
            path2 = "";
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
    delwin(win);
    endwin();
}
*/