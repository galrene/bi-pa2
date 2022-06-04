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
    /**
    * @brief reads a number at given coords
    * 
    * @param yCoord 
    * @param xCoord 
    * @return int 
    */
    int readNumber ( const size_t & yCoord, const size_t & xCoord, const size_t & n );
    string readString ( const size_t & y, const size_t & x, const size_t & n );

    bool handleCreateMenu ( vector<shared_ptr<CCharacter>> & loadedCharacters );
    string chooseName ( const char * menuHeader );
    shared_ptr<CCharacter> chooseCharacter ( vector<shared_ptr<CCharacter>> & characters, const char * menuHeader );
    shared_ptr<CPlayer> createPlayer ( vector<shared_ptr<CCharacter>> & loadedCharacters, const char * menuHeader );
    
  protected:
    /**
    * @brief handle navigating the menu using arrow keys
    * 
    * @param menuSize count of menu items
    * @param highlight currently highlighted menu item
    * @return 1 on enter, 0 on normal navigation, -1 on CTRL-D
    */
    int  handleNavigation ( const size_t & menuSize );
    void printCharacters ( vector<shared_ptr<CCharacter>> & characters );
    bool chooseCharacterMovement ( vector<shared_ptr<CCharacter>> & characters );
    void clearSpaces ( const size_t & y, const size_t & x, const size_t & nSpaces );
    /**
    * @brief toggle enabled/disabled color 
    * 
    * @param itemIndex index of menuItem
    * @param menuItems vector with menuItems
    */
    void toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems );
    bool handleSettingsMovement ( vector<pair<string,bool>> & menuItems );
    bool isValidDeckSize ( int number );
    bool initCurses ( void );
    void printError ( const char * errorMessage );
    bool handleMenuMovement ( vector<string> & menuItems );

    int m_XMax, m_YMax;
    int m_Lines, m_Cols;
    WINDOW * m_Win;
    size_t m_Highlight = 0;
    CGameSettings m_Settings;
};