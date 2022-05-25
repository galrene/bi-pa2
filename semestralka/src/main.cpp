#include <ncurses.h>
#include <iostream>
#include <vector>
using namespace std;

void initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
}

WINDOW * drawMenu ( WINDOW *& win, int & xMax, int & yMax ) {
    /* lines, colons, beginY, beginX */
    box ( win,0,0 );
    mvwprintw ( win, 0, 7, " Main menu ");
    wrefresh (win);
    return win;
}
bool handleMenu ( WINDOW *& win ) {
    vector<string> menuItems = {"New Game","Load Game","Settings", "Quit"};
    size_t highlight = 0;
    int button;
    keypad (win, TRUE );
    curs_set(0);

    while ( 1 ) {
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == highlight )
                wattron ( win, A_REVERSE );
            mvwprintw ( win, i+1, 2, "%s", menuItems[i].c_str() );
            wattroff ( win, A_REVERSE );
        }
        button = wgetch ( win );
        
        switch ( button ) {
            case KEY_UP:
                if ( highlight )
                    highlight--;
                break;
            case KEY_DOWN:
                if ( highlight != menuItems.size() - 1)
                    highlight++;
                break;
            default:
                break;
        }
        if ( button == 10 )
            break;
    }
    // selected quit
    if ( highlight == 3 )
        return false;
    printw ("Your choice was : %s\n",menuItems[highlight].c_str() );
    refresh();
    return true;
}

int main ( int argc, char const *argv[] )
{
    int xMax, yMax;
    initCurses();
    getmaxyx ( stdscr, yMax, xMax );
    WINDOW * win = newwin ( yMax/3, 25, yMax/4, (xMax/2-10) );
    while ( 1 ) {
        drawMenu ( win, xMax, yMax );
        if ( ! handleMenu ( win ) )
            break;
    }
    endwin();
    return 0;
}
