#include <string>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <ncurses.h>

using namespace std;

#define OFF_PAIR COLOR_PAIR(3)
#define ON_PAIR COLOR_PAIR(2)
#define ON_SELECTED_PAIR COLOR_PAIR(4)
#define OFF_SELECTED_PAIR COLOR_PAIR(1)

class CMenuItem {
  public:
             CMenuItem ( WINDOW * win, string name, size_t y, size_t x );
    virtual ~CMenuItem ( void ) noexcept = default;

    virtual void onHighlightAction ( void ) = 0;
    virtual bool onClickAction ( void ) = 0;
    virtual void onRenderAction ( void ) = 0;
    void clearSpaces ( size_t y, size_t x, size_t nSpaces );
  protected:
    WINDOW * m_Win;
    string m_Name;
    int m_yCoord;
    int m_xCoord;
};
class CMenuTraverseItem : public CMenuItem {
  public:
    CMenuTraverseItem ( WINDOW * win, string name, size_t y, size_t x, function<bool(void)> func )
    : CMenuItem ( win, name, y, x ), m_Function ( func ) {}

    virtual void onHighlightAction ( void ) override;
    virtual void onRenderAction ( void ) override;
    virtual bool onClickAction ( void ) override;
  protected:
    function<bool(void)> m_Function;
};
class CTogglableItem : public CMenuItem {
  public:
    CTogglableItem ( WINDOW * win, string name, size_t y, size_t x, bool & togglableSetting )
    : CMenuItem ( win, name, y, x ), m_Toggle ( togglableSetting ) {}

    virtual void onHighlightAction ( void ) override;
    virtual bool onClickAction ( void ) override;
    virtual void onRenderAction ( void ) override;
  protected:
    bool & m_Toggle;
};
class CInputFieldInt : public CMenuItem {
  public:
    CInputFieldInt ( WINDOW * win, string name, size_t y, size_t x, string setValue, size_t maxInputLength )
    : CMenuItem ( win, name, y, x ), m_SetValue ( setValue ), m_MaxInputLength ( maxInputLength ) {}
    
    virtual void onHighlightAction ( void ) override;
    virtual bool onClickAction ( void ) override;
    virtual void onRenderAction ( void ) override;
    int m_ReadValue;
  protected:
    int readNumber ( size_t y, size_t x );
    string m_SetValue;
    size_t m_MaxInputLength;
};
class CInputFieldString : public CMenuItem {
  public:
    CInputFieldString ( WINDOW * win, string name, size_t y, size_t x, string inputtedItem, size_t maxInputLength )
    : CMenuItem ( win, name, y, x ), m_SetValue ( inputtedItem ), m_MaxInputLength ( maxInputLength ) {}
    
    virtual void onHighlightAction ( void ) override;
    virtual bool onClickAction ( void ) override;
    virtual void onRenderAction ( void ) override;
  protected:
    string readString ( size_t y, size_t x );

    string m_SetValue;
    string m_ReadValue;
    size_t m_MaxInputLength;
};
// ------------------------------------------------------------------------------------
CMenuItem::CMenuItem ( WINDOW * win, string name, size_t y, size_t x  )
: m_Win ( win ), m_Name ( name ), m_yCoord ( y ), m_xCoord ( x ) {}

void CMenuItem::clearSpaces ( size_t y, size_t x, size_t nSpaces ) {
    string str;
    for ( size_t i = 0; i < nSpaces; i++ )
        str += " ";
    mvwprintw ( m_Win, y, x, "%s", str.c_str() ); 
}
// ------------------------------------------------------------------------------------
void CMenuTraverseItem::onRenderAction ( void ) {
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
}
void CMenuTraverseItem::onHighlightAction ( void ) {
    wattron ( m_Win, A_REVERSE );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wattroff ( m_Win, A_REVERSE );
}
bool CMenuTraverseItem::onClickAction ( void ) {
    if ( m_Function )
        return true;
    return false;
}
// ------------------------------------------------------------------------------------
void CTogglableItem::onRenderAction ( void ) {
    if ( m_Toggle )
        wattron ( m_Win, ON_PAIR );
    else
        wattron ( m_Win, OFF_PAIR );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
}
void CTogglableItem::onHighlightAction ( void ) {
    if ( m_Toggle )
        wattron ( m_Win, ON_SELECTED_PAIR );
    else
        wattron ( m_Win, OFF_SELECTED_PAIR );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wattroff ( m_Win, ON_SELECTED_PAIR );
    wattroff ( m_Win, OFF_SELECTED_PAIR );
}
bool CTogglableItem::onClickAction ( void ) {
    m_Toggle == false
    ? m_Toggle = true
    : m_Toggle = false;
    clearSpaces ( m_yCoord, m_xCoord, m_Name.size() );
    if ( m_Toggle )
        wattron ( m_Win, OFF_PAIR );
    else
        wattron ( m_Win, ON_PAIR );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wattroff ( m_Win, OFF_PAIR );
    wattroff ( m_Win, ON_PAIR );
    wrefresh ( m_Win );
    return true;
}
// ------------------------------------------------------------------------------------
/**
 * @brief reads a number at coordinates
 * 
 * @return int read number
 * @throws invalid_argument entered value isn't an integer
 */
int CInputFieldInt::readNumber ( size_t y, size_t x ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ m_MaxInputLength + 1 ];
    mvwgetnstr ( m_Win, m_yCoord, m_xCoord, buff, m_MaxInputLength );
    noecho();
    curs_set ( 0 );
    for ( size_t i = 0; i < m_MaxInputLength; i++ )
        if ( ! isdigit ( buff[i] ) )
            throw invalid_argument ( "Entered value must be an integer" );
    return atoi ( buff );
}
void CInputFieldInt::onRenderAction ( void ) {
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wprintw ( m_Win, " %s", m_SetValue.c_str() );
}
void CInputFieldInt::onHighlightAction ( void ) {
    wattron ( m_Win, A_REVERSE );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wattroff ( m_Win, A_REVERSE );
}
/**
 * @brief clears previously entred value from screen and reads a number as a string
 * 
 * @return read number as a string
 * @throws invalid_argument entered value isn't an integer
 */
bool CInputFieldInt::onClickAction ( void ) {
    clearSpaces ( m_yCoord, m_xCoord + m_Name.size(), m_MaxInputLength );
    int num;
    try {
        num = readNumber ( m_yCoord, m_xCoord );
    }
    catch ( const exception & e ) {
        cerr << e.what() << '\n';
        return false;
    }
    m_ReadValue = num;
    return true;
}
// ------------------------------------------------------------------------------------
string CInputFieldString::readString ( size_t y, size_t x ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ m_MaxInputLength + 1 ];
    mvwgetnstr ( m_Win, m_yCoord, m_xCoord, buff, m_MaxInputLength );
    noecho();
    curs_set ( 0 );
    return string ( buff );
}
void CInputFieldString::onRenderAction ( void ) {
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wprintw ( m_Win, " %s", m_SetValue.c_str() );
}
void CInputFieldString::onHighlightAction ( void ) {
    wattron ( m_Win, A_REVERSE );
    mvwprintw ( m_Win, m_yCoord, m_xCoord, "%s", m_Name.c_str() );
    wattroff ( m_Win, A_REVERSE );
}
bool CInputFieldString::onClickAction ( void ) {
    clearSpaces ( m_yCoord, m_xCoord + m_Name.size(), m_MaxInputLength );
    string str = readString ( m_yCoord, m_xCoord );
    m_ReadValue = str;;
    return true;
}