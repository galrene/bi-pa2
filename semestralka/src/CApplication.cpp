#include <ncurses.h>
#include <iostream>
#include <vector>
#include "CGameSettings.h"
#include "CGameStateManager.h"
using namespace std;

class CApplication {
  public:
    CApplication ( CGameSettings & settings, CGameStateManager & gsm );
    
    void drawMenu ( const char * menuHeader  );
    void toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems );
    bool handleSettings ( void );
    bool handleMainMenu ( void );
    bool handleNavigation ( const size_t & menuSize, size_t & highlight );
    void handleMenuMovement ( size_t & highlight, vector<string> & menuItems );
    void handleSettingsMovement ( size_t & highlight, vector<pair<string,bool>> & menuItems );

  protected:
    bool initCurses ( void );
    int m_XMax, m_YMax;
    WINDOW * m_Win;
    CGameSettings m_Settings;
    CGameStateManager m_Gsm;
};

CApplication::CApplication ( CGameSettings & settings, CGameStateManager & gsm ) {
    initCurses(); // add try catch block
    getmaxyx ( stdscr, m_YMax, m_XMax );
    m_Win = newwin ( m_YMax/3, 25, m_YMax/4, (m_XMax/2-10) );
    keypad ( m_Win, TRUE ); // enable keypad inputs
    m_Gsm = gsm;
    m_Settings = settings;

}

void CApplication::drawMenu ( const char * menuHeader  ) {
    wclear ( m_Win );
    /* lines, colons, beginY, beginX */
    box ( m_Win, 0, 0 );
    mvwprintw ( m_Win, 0, 7, "%s", menuHeader );
    //wrefresh (m_Win);
}

bool CApplication::initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
    if ( ! has_colors () )
        return false;
    start_color();
    init_pair ( 3, COLOR_WHITE, COLOR_RED );
    init_pair ( 2, COLOR_WHITE, COLOR_GREEN );
    init_pair ( 4, COLOR_GREEN, COLOR_WHITE );
    init_pair ( 1, COLOR_RED, COLOR_WHITE );
    curs_set ( 0 ); // hide cursor
    #define OFF_PAIR COLOR_PAIR(3)
    #define ON_PAIR COLOR_PAIR(2)
    #define ON_SELECTED_PAIR COLOR_PAIR(4)
    #define OFF_SELECTED_PAIR COLOR_PAIR(1)
    return true;
}

/**
 * @brief toggle enabled/disabled color 
 * 
 * @param itemIndex index of menuItem
 * @param menuItems vector with menuItems
 */
void CApplication::toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems ) {
    wmove ( m_Win, 1+itemIndex,2 );
    wclrtoeol(m_Win);
    drawMenu ( "Settings" );
    wrefresh(m_Win);
    if ( ! menuItems[itemIndex].second ) {
        wattron ( m_Win, ON_PAIR );
        menuItems[itemIndex].second = true;
    }
    else {
        wattron ( m_Win, OFF_PAIR );
        menuItems[itemIndex].second = false;
    }
    mvwprintw ( m_Win , 1+itemIndex, 2, "%s", menuItems[itemIndex].first.c_str() );
    wattroff ( m_Win, ON_PAIR );
    wattroff ( m_Win, OFF_PAIR );
}

bool CApplication::handleNavigation ( const size_t & menuSize, size_t & highlight ) {
    int button;
    button = wgetch ( m_Win );
    switch ( button ) {
        case KEY_UP:
            if ( highlight )
                highlight--;
            else highlight = menuSize - 1;
            break;
        case KEY_DOWN:
            if ( highlight != menuSize - 1)
                highlight++;
            else highlight = 0;
            break;
        default:
            break;
    }
    // pressed enter
    if ( button == 10 )
        return true;
    return false;
}

void CApplication::handleSettingsMovement ( size_t & highlight, vector<pair<string,bool>> & menuItems ) {
    // SIMPLIFY
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == highlight && ! menuItems[i].second && i != menuItems.size() - 1 )
                wattron ( m_Win, OFF_SELECTED_PAIR );
            else if ( i == highlight && menuItems[i].second )
                wattron ( m_Win, ON_SELECTED_PAIR );
            else if ( i != menuItems.size() - 1 && ! menuItems[i].second )
                wattron ( m_Win, OFF_PAIR );
            else if ( i != menuItems.size() - 1 && menuItems[i].second )
                wattron ( m_Win, ON_PAIR );
            if ( i == highlight && i == menuItems.size() - 1 )
                wattron ( m_Win, A_REVERSE );
            
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].first.c_str() );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, ON_SELECTED_PAIR );
            wattroff ( m_Win, OFF_SELECTED_PAIR );
            wattroff ( m_Win, A_REVERSE );
        }
        if ( handleNavigation ( menuItems.size(), highlight ) )
            break;
    }
}

bool CApplication::handleSettings ( void ) {
    drawMenu ( "Settings" );
    vector<pair<string, bool>> menuItems = { {"Two-player game", false},
                                             {"Deck size", false},
                                             {"-Return-", false} };
    size_t highlight = 0;
    size_t deckSize = 10; // create a constant of this
    while ( highlight != 2 ) {
        handleSettingsMovement ( highlight, menuItems );
        if ( highlight == 0 ) {
            toggleColor ( 0, menuItems );
            m_Settings.toggleSP();
        }
        if ( highlight == 1 ) {
            //toggleColor ( 1, menuItems );
            curs_set ( 1 );
            if ( mvwscanw ( m_Win, 2, 2 + menuItems[1].first.size(), ": %d", &deckSize ) != 1 ) {
                printw ( "ERROR: Deck size has to be a number\n" );
                refresh();
            }
            curs_set ( 0 );
            m_Settings.setMaxDeckSize(deckSize);
        }
    }
    
    printw ("Your choice was : %s\n",menuItems[highlight].first.c_str() );
    refresh();
    return true;
}

void CApplication::handleMenuMovement ( size_t & highlight, vector<string> & menuItems ) {
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == highlight )
                wattron ( m_Win, A_REVERSE );
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].c_str() );
            wattroff ( m_Win, A_REVERSE );
        }
        if ( handleNavigation ( menuItems.size(), highlight ) )
            break;
    }
}

bool CApplication::handleMainMenu ( void ) {
    drawMenu ( "Main menu" );
    vector<string> menuItems = {"New Game","Load Game","Settings", "-Quit-"};
    size_t highlight = 0;
    
    handleMenuMovement (highlight, menuItems );
    // selected quit
    if ( highlight == 3 )
        return false;
    else if ( highlight == 2 )
        handleSettings ();
    
    printw ("Your choice was : %s\n",menuItems[highlight].c_str() );
    refresh();
    return true;
}





int main ( int argc, char const *argv[] ) {
    CGameStateManager gsm;
    CGameSettings settings;
    CApplication app ( settings, gsm );
    while ( app.handleMainMenu () ) {}
    endwin();
    return 0;
}
