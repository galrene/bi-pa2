#include <ncurses.h>
#include <iostream>
using namespace std;

void initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
    keypad (stdscr, TRUE );
}

int main(int argc, char const *argv[])
{
    initCurses();
    WINDOW * win = newwin(50,50,0,0);
    int c;
    while ( (c = getch()) ) {
        if (  )
            break;
        box(win,0,0);
        wprintw(win, "Test:\n ");
        wrefresh(win);
        waddch ( win, c );
        wrefresh(win);
    }


    endwin();
    return 0;
}
