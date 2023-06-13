#include "window.h"

int main(int argc, char **argv)
{
    WINDOW *win;

    init_window();
    win = new_window();

    main_program(win, "/home/mikel/Escritorio");

    end_window(win);
    return 0;
}