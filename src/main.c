#include "window.h"

int main(int argc, char **argv)
{
    WINDOW *win;

    init_window();
    win = new_window();

    if (argc < 2)
        main_program(win, "/home/mikel/Escritorio");
    else
        main_program(win, argv[1]);

    end_window(win);
    return 0;
}