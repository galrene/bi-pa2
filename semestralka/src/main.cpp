#include <ncurses.h>
#include <iostream>
#include <vector>
using namespace std;

#define OFF_PAIR COLOR_PAIR(3)
#define ON_PAIR COLOR_PAIR(2)

bool initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
    if ( ! has_colors () )
        return false;
    start_color();
    init_pair ( 1, COLOR_WHITE, COLOR_BLACK );
    init_pair ( 2, COLOR_WHITE, COLOR_GREEN );
    init_pair ( 3, COLOR_WHITE, COLOR_RED );
    curs_set ( 0 ); // hide cursor

    return true;
}

void drawMenu ( WINDOW *& win, const char * menuHeader  ) {
    wclear ( win );
    /* lines, colons, beginY, beginX */
    box ( win, 0, 0 );
    mvwprintw ( win, 0, 7, "%s", menuHeader );
}

void handleMenuMovement ( WINDOW *& win, size_t & highlight, vector<string> & menuItems ) {
    int button;
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == highlight )
                wattron ( win, A_REVERSE );
            mvwprintw ( win, i+1, 2, "%s", menuItems[i].c_str() );
            wattroff ( win, A_REVERSE );
        }
        // handle input
        button = wgetch ( win );
        switch ( button ) {
            case KEY_UP:
                if ( highlight )
                    highlight--;
                else highlight = menuItems.size() - 1;
                break;
            case KEY_DOWN:
                if ( highlight != menuItems.size() - 1)
                    highlight++;
                else highlight = 0;
                break;
            default:
                break;
        }

        // pressed enter
        if ( button == 10 )
            break;
    }
}

void handleSettingsMovement ( WINDOW *& win, size_t & highlight, vector<pair<string,bool>> & menuItems ) {
    int button;
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == highlight )
                wattron ( win, A_REVERSE );
            else if ( i != menuItems.size() - 1 && ! menuItems[i].second )
                wattron ( win, OFF_PAIR );
            else if ( i != menuItems.size() - 1 && menuItems[i].second )
                wattron ( win, ON_PAIR );
            mvwprintw ( win, i+1, 2, "%s", menuItems[i].first.c_str() );
            wattroff ( win, OFF_PAIR );
            wattroff ( win, OFF_PAIR );
            wattroff ( win, A_REVERSE );
        }
        // handle input
        button = wgetch ( win );
        switch ( button ) {
            case KEY_UP:
                if ( highlight )
                    highlight--;
                else highlight = menuItems.size() - 1;
                break;
            case KEY_DOWN:
                if ( highlight != menuItems.size() - 1)
                    highlight++;
                else highlight = 0;
                break;
            default:
                break;
        }

        // pressed enter
        if ( button == 10 )
            break;
    }
}

void toggleItem ( WINDOW *& win, size_t i, vector<pair<string,bool>> & menuItems ) {
    wmove ( win, 1+i,2 );
    wclrtoeol(win);
    drawMenu ( win, "Settings" );
    wrefresh(win);
    if ( ! menuItems[i].second ) {
        wattron ( win, ON_PAIR );
        menuItems[i].second = true;
    }
    else {
        wattron ( win, OFF_PAIR );
        menuItems[i].second = false;
    }
    mvwprintw ( win , 1+i, 2, "%s", menuItems[i].first.c_str() );
    wattroff ( win, ON_PAIR );
    wattroff ( win, OFF_PAIR );
}

bool handleSettings ( WINDOW *& win ) {
    drawMenu ( win, "Settings" );
    

    vector<pair<string, bool>> menuItems = { {"Two-player game", false},
                                    {"Deck size", false},
                                    {"-Return-", false} };
    size_t highlight = 0;

    while ( highlight != 2 ) {
        handleSettingsMovement ( win, highlight, menuItems );
        
        if ( highlight == 0 )
            toggleItem ( win, 0, menuItems );
        if ( highlight == 1 )
            toggleItem ( win, 1, menuItems );
    }
    
    printw ("Your choice was : %s\n",menuItems[highlight].first.c_str() );
    refresh();
    return true;
}

bool handleMainMenu ( WINDOW *& win ) {
    drawMenu ( win, "Main menu" );
    vector<string> menuItems = {"New Game","Load Game","Settings", "-Quit-"};
    size_t highlight = 0;
    
    handleMenuMovement ( win, highlight, menuItems );
    // selected quit
    if ( highlight == 3 )
        return false;
    else if ( highlight == 2 )
        handleSettings(win);
    
    printw ("Your choice was : %s\n",menuItems[highlight].c_str() );
    refresh();
    return true;
}

int main ( int argc, char const *argv[] ) {
    int xMax, yMax;
    if ( ! initCurses() ) {
        printw ( "Initialization error" );
        getch();
        endwin();
        return -1;
    }
    getmaxyx ( stdscr, yMax, xMax );

    WINDOW * win = newwin ( yMax/3, 25, yMax/4, (xMax/2-10) );
    keypad ( win, TRUE ); // enable keypad inputs

    while ( handleMainMenu ( win ) ) {}
    endwin();
    return 0;
}
