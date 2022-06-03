#include <ncurses.h>
#include <iostream>
#include <vector>
#include <functional>
#include "string.h"

#include "CGameSettings.h"
#include "CGameStateManager.h"
#include "CConfigParser.h"
#include "Constants.h"
using namespace std;

class CMenu {
  public:
    CMenu ( void );
    ~ CMenu ( void );    
    void drawMenu ( const char * menuHeader  );
    bool handleSettings ( void );
    bool handleMainMenu ( void );
    int  handleNavigation ( const size_t & menuSize );
    int  readNumber ( size_t yCoord, size_t xCoord );
    string readString ( const size_t & y, const size_t & x, const size_t & n );

    bool handleCreateMenu ( void );
    void printCharacters ( vector<shared_ptr<CCharacter>> & characters );
    string chooseName ( const char * menuHeader );
    shared_ptr<CCharacter> chooseCharacter ( vector<shared_ptr<CCharacter>> & characters, const char * menuHeader );
    shared_ptr<CPlayer> createPlayer ( vector<shared_ptr<CCharacter>> & loadedCharacters, const char * menuHeader );
    
  protected:
    bool chooseCharacterMovement ( vector<shared_ptr<CCharacter>> & characters );
    void clearSpaces ( const size_t & y, const size_t & x, const size_t & nSpaces );
    void toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems );
    bool handleMenuMovement ( vector<string> & menuItems );
    bool handleSettingsMovement ( vector<pair<string,bool>> & menuItems );
    bool isValidDeckSize ( int number );
    bool initCurses ( void );
    int m_XMax, m_YMax;
    int m_Lines, m_Cols;
    WINDOW * m_Win;
    size_t m_Highlight = 0;
    CGameSettings m_Settings;
};