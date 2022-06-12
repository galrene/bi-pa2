#include "../src/CGameStateManager.h"
#include <ncurses.h>

class CMenu {
  public:
    CMenu ( const string & name, WINDOW * win, size_t menuSize )
    : m_Win ( win ), m_Highlight ( 0 ), m_Size ( menuSize ) {}
    
    virtual int drawMenu ( void );
    int handleNavigation ( void );
    virtual int handleMenu ( CGameStateManager & gsm ) = 0;
    virtual int movement ( void );
    virtual void print ( void ) = 0;
    string m_Name;
  protected:
    WINDOW * m_Win;
    int m_Highlight;
    int m_Size;
};
// 1 = enter, 0 = up/down/irrelevant button, -1 = CTRL-D
int CMenu::handleNavigation ( void ) {
    int button;
    button = wgetch ( m_Win );
    switch ( button ) {
        case KEY_UP:
            if ( m_Highlight )
                m_Highlight--;
            else m_Highlight = m_Size - 1;
            break;
        case KEY_DOWN:
            if ( m_Highlight != m_Size - 1)
                m_Highlight++;
            else m_Highlight = 0;
            break;
        case ('d' & 0x1F):
            return -1;
        case ('c'): // for clearing error messages in the top left corner
            move(0,0);
            clrtoeol();
            refresh();
            break;
        default:
            break;
    }
    if ( button == 10 ) // enter
        return 1;
    return 0;
}

int CMenu::movement ( void ) {
    while ( 1 ) {
        print();
        int res = handleNavigation();
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}

int CMenu::drawMenu ( void ) {
    wclear ( m_Win );
    box ( m_Win, 0, 0 );
    mvwprintw ( m_Win, 0, getmaxx ( m_Win ) / 2 - ( m_Name.size() / 2 ), "%s", m_Name.c_str() );
    wrefresh (m_Win);
}

class CMainMenu : public CMenu {
  public:
    CMainMenu ( const string & name, WINDOW * win, const vector<shared_ptr<CMenu>> & menuItems, size_t size );

    virtual int handleMenu ( CGameStateManager & gsm ) override;
    virtual void print ( void ) override;
  protected:
  vector<shared_ptr<CMenu>> m_MenuItems;
};

void CMainMenu::print ( void ) {
    size_t i = 0;
    for ( const auto & x : m_MenuItems ) {
        wclear ( m_Win );
        if ( m_Highlight == i )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, 1+i, ( getmaxy(m_Win) / 2 ) - x->m_Name.size()/2, "%s", x->m_Name.c_str() );
        wattroff ( m_Win, A_REVERSE );
        wrefresh ( m_Win );
        ++i;
    }
}

CMainMenu::CMainMenu ( const string & name, WINDOW * win, const vector<shared_ptr<CMenu>> & menuItems, size_t size )
: CMenu ( name, win, size ), m_MenuItems ( menuItems ) {}

int CMainMenu::handleMenu ( CGameStateManager & gsm ) {
    CGameStateManager gsm;
    int res;
    while ( 1 ) {
        drawMenu();
        m_Highlight = 0;
        if ( ! movement () )
            return -1;
        if ( res = m_MenuItems[m_Highlight]->handleMenu ( gsm ); res == -1 ) // ctrl-d
            return -1;
        else if ( res == 1 ) // created a game
            return 1;
        else if ( res == 2 ) // loaded a games
            return 2;
    }
    return -1;
}


int main(int argc, char const *argv[])
{   
    WINDOW * win = newwin(50,50,(getmaxy(stdscr)/2)-25,(getmaxx(stdscr)/2)-25);
        CMainMenu
    ( "Main menu", win,
    { CCreateGameMenu
      ( "New game", win ),
      CLoadGameMenu
      ( "Load saved game", win ),
      CSettingsMenu
      ( "Settings", win ),
       CReturn()
    }
    );
    return 0;
}
