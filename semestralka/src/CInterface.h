#include <vector>
#include <ncurses.h>
#include <string>
using namespace std;

class CInterface {
  public:
    CInterface ( void ) {
        getmaxyx ( stdscr, m_YMax, m_XMax );
        m_Win = newwin ( m_YMax/3, 25, m_YMax/4, (m_XMax/2-10) );
        initCurses(); // add try catch block
    }

    void drawMenu ( const char * menuHeader  );
    void toggleColor ( size_t itemIndex, vector<pair<string,bool>> & menuItems );
    bool handleSettings ( void );
    bool handleMainMenu ( void );
    void handleMenuMovement ( size_t & highlight, vector<string> & menuItems );
    void handleSettingsMovement ( size_t & highlight, vector<pair<string,bool>> & menuItems );

  protected:
    bool initCurses ( void );
    int m_XMax, m_YMax;
    WINDOW * m_Win;
};