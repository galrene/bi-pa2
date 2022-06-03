#include <ncurses.h>
#include <iostream>
#include <vector>
#include <functional>

#include "CGameSettings.h"
#include "CGameStateManager.h"
#include "CConfigParser.h"
#include "Constants.h"
using namespace std;

class CApplication {
  public:
    CApplication ( CGameSettings settings/*, CGameStateManager gsm*/ );
    
    void drawMenu ( const char * menuHeader  );
    bool handleSettings ( void );
    bool handleMainMenu ( void );
    int  handleNavigation ( const size_t & menuSize );
    int  readNumber ( size_t yCoord, size_t xCoord );

    bool handleCreateMenu ( void );
    void printCharacters ( vector<shared_ptr<CCharacter>> & characters );

  protected:
    bool handleCreateMovement ( vector<shared_ptr<CCharacter>> & characters );
    void toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems );
    bool handleMenuMovement ( vector<string> & menuItems );
    bool handleSettingsMovement ( vector<pair<string,bool>> & menuItems );
    bool isValidDeckSize ( int number );
    bool initCurses ( void );
    int m_XMax, m_YMax;
    WINDOW * m_Win;
    CGameSettings m_Settings;
    size_t m_Highlight = 0;
};

CApplication::CApplication ( CGameSettings settings/*, CGameStateManager gsm*/ ) {
    initCurses(); // add try catch block
    getmaxyx ( stdscr, m_YMax, m_XMax );
    m_Win = newwin ( m_YMax/3, 25, m_YMax/4, (m_XMax/2-10) );
    keypad ( m_Win, TRUE ); // enable keypad inputs
    // m_Gsm = gsm;
    m_Settings = settings;

}

void CApplication::drawMenu ( const char * menuHeader  ) {
    wclear ( m_Win );
    /* lines, colons, beginY, beginX */
    box ( m_Win, 0, 0 );
    mvwprintw ( m_Win, 0, 8, "%s", menuHeader );
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
/**
 * @brief handle navigating the menu using arrow keys
 * 
 * @param menuSize count of menu items
 * @param highlight currently highlighted menu item
 * @return 1 on enter, 0 on normal navigation, -1 on q
 */
int CApplication::handleNavigation ( const size_t & menuSize ) {
    int button;
    button = wgetch ( m_Win );
    switch ( button ) {
        case KEY_UP:
            if ( m_Highlight )
                m_Highlight--;
            else m_Highlight = menuSize - 1;
            break;
        case KEY_DOWN:
            if ( m_Highlight != menuSize - 1)
                m_Highlight++;
            else m_Highlight = 0;
            break;
        case ('d' & 0x1F):
            return -1;
        default:
            break;
    }
    // pressed enter
    if ( button == 10 )
        return 1;
    return 0;
}

bool CApplication::handleSettingsMovement (  vector<pair<string,bool>> & menuItems ) {
    // SIMPLIFY
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == m_Highlight && ! menuItems[i].second && i == 0 )
                wattron ( m_Win, OFF_SELECTED_PAIR );
            else if ( i == m_Highlight && menuItems[i].second && i == 0 )
                wattron ( m_Win, ON_SELECTED_PAIR );
            else if ( i == 0 && ! menuItems[i].second )
                wattron ( m_Win, OFF_PAIR );
            else if ( i == 0 && menuItems[i].second )
                wattron ( m_Win, ON_PAIR );
            if ( i == m_Highlight && i != 0 )
                wattron ( m_Win, A_REVERSE );
            
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].first.c_str() );
            if ( i == 1 )
                wprintw ( m_Win, " %d", m_Settings.getMaxDeckSize() );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, ON_SELECTED_PAIR );
            wattroff ( m_Win, OFF_SELECTED_PAIR );
            wattroff ( m_Win, A_REVERSE );
        }
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}
/**
 * @brief reads a number at given coords
 * 
 * @param yCoord 
 * @param xCoord 
 * @return int 
 */
int CApplication::readNumber ( size_t yCoord, size_t xCoord ) {
    curs_set ( 1 );
    echo();
    char buff[4] = {0};
    mvwgetnstr ( m_Win, yCoord, xCoord, buff, 3);
    noecho();
    curs_set ( 0 );
    for ( size_t i = 0; i < 3; i ++ ) // use isDigit() ?
        if ( ! isdigit ( buff[i] ) && buff[i] != '\0' )
            return -1;
    return atoi ( buff );
    // printw ( "Entered: %d \n", loadedNumber );
    // printw ( "Res: %d \n", res );
}

bool CApplication::isValidDeckSize ( int number ) {
    if ( number <= 0 ) {
        printw ( "ERROR: %d Must be a number\n", number );
        refresh();
        return false;
    }
    return true;
}
//  nechcem iba breaknut on click v menu?
// Spravit ako simple string vector bez paru, hodnoty citat iba zo settings
bool CApplication::handleSettings ( void ) {
    drawMenu ( "Settings" );
    vector<pair<string, bool>> menuItems = { {"Two-player game", m_Settings.isTwoPlayerGame() },
                                             {"Dick size:", false},
                                             {"-Return-", false} };
    m_Highlight = 0;
    while ( m_Highlight != 2 ) {
        if ( ! handleSettingsMovement ( menuItems ) )
            return false;
        if ( m_Highlight == 0 ) {
            toggleColor ( 0, menuItems );
            m_Settings.toggleSP();
        }
        if ( m_Highlight == 1 ) {
            mvwprintw ( m_Win, 2, 3 + menuItems[m_Highlight].first.size(), "   " ); // clear 3 spaces 
            int num = readNumber ( 2, 3 + menuItems[m_Highlight].first.size() );
            if ( isValidDeckSize ( num ) )
                m_Settings.setMaxDeckSize ( num );
            drawMenu ( "Settings" );
            wrefresh ( m_Win );
        }
    }
    
    // printw ("Your choice was : %s\n",menuItems[m_Highlight].first.c_str() );
    refresh();
    return true;
}

bool CApplication::handleMenuMovement ( vector<string> & menuItems ) {
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == m_Highlight )
                wattron ( m_Win, A_REVERSE );
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].c_str() );
            wattroff ( m_Win, A_REVERSE );
        }
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}

void CApplication::printCharacters ( vector<shared_ptr<CCharacter>> & characters ) {
    for ( size_t i = 0; i < characters.size(); i++ ) {
        if ( i == m_Highlight )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, 2+i, 2, characters[i]->getHeader().c_str() );
        wattroff ( m_Win, A_REVERSE );
    }
}

bool CApplication::handleCreateMovement ( vector<shared_ptr<CCharacter>> & characters ) {
    while ( 1 ) {
        printCharacters ( characters );
        int res = handleNavigation ( characters.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}

bool CApplication::handleCreateMenu ( void ) {
    drawMenu ( "New game:\nPlayer 1 chooses character" );
    CConfigParser cfgp;
    vector<shared_ptr<CCharacter>> characters = cfgp.loadCharacters ( "characters" );
    m_Highlight = 0;
    if ( ! handleCreateMovement ( characters ) )
        return false;
    // chooseNames();
    return true;
}

bool CApplication::handleMainMenu ( void ) {
    drawMenu ( "Main menu" );
    vector<string> menuItems = {"New game","Load saved game","Settings", "-Quit-"};
    m_Highlight = 0;
    
    if ( ! handleMenuMovement ( menuItems ) )
        return false;
    if ( m_Highlight == 0 ) {
        if ( ! handleCreateMenu () )
            return false;
    }
    else if ( m_Highlight == 1 )
        cout << "1" << endl;
    else if ( m_Highlight == 2 ) {
        if ( ! handleSettings () )
            return false;
    }
    else if ( m_Highlight == 3 )
        return false;
    
    // printw ("Your choice was : %s\n",menuItems[highlight].c_str() );
    refresh();
    return true;
}





int main ( int argc, char const *argv[] ) {
    CGameSettings settings;
    CApplication app ( settings );
    while ( app.handleMainMenu () ) {}
    endwin();
    return 0;
}
