#pragma once
#ifndef CMenu_H
#define CMenu_H
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <functional>
#include "string.h"

#include "CGameSettings.h"
#include "CGameStateManager.h"
#include "CConfigParser.h"
#include "Constants.h"

#include "CHuman.h"
#include "CBot.h"
using namespace std;

class CMenu {
  public:
    CMenu ( void );
    ~CMenu ( void );   
    /**
     * @brief Render menu box and header
     * 
     * @param menuHeader header in the top middle of the window
     */
    void drawMenu ( const string & menuHeader );
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool handleSettings ( void );
    /**
    * @return 1 = createdGame, 2 = loadedGame. -1 == quit
    */
    int handleMainMenu ( CGameStateManager & gsm );
    /**
    * @return -1 = requested exit, 1 = successfuly created a game, 0 = game creation failure
    */
    int handleCreateMenu ( CGameStateManager & gsm );
    /**
    * @return -1 on requested exit
    */
    int handleLoadGameMenu ( CGameStateManager & gsm );
    CDeck chooseDeckMenu ( vector<CDeck> & decks );
    string chooseNameMenu ( const char * menuHeader );
    shared_ptr<CCharacter> chooseCharacter ( vector<shared_ptr<CCharacter>> & characters,
                                             const char * menuHeader );
    shared_ptr<CPlayer> createPlayerMenu ( map<string,shared_ptr<CCharacter>> & loadedCharacters,
                                           vector<CDeck> & decks, const char * menuHeader );
    bool loadingScreen ( map<string,shared_ptr<CCharacter>> & characters,
                         map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks );
    
  protected:
    /**
    * @brief Read user input for menu navigation.
    * 
    * @param menuSize number of menu items to navigate through
    * @return 1 on enter, 0 on normal navigation or random button, -1 on CTRL-D, 2 on return one menu back request
    */
    int  handleNavigation ( const size_t & menuSize );
    bool handleMenuMovement ( vector<string> & menuItems );
    bool handleSettingsMovement ( vector<pair<string,bool>> & menuItems );
    bool chooseCharacterMovement ( vector<shared_ptr<CCharacter>> & characters );
    bool loadMenuMovement ( vector<fs::directory_entry> & saves );
    bool chooseDeckMovement ( vector<CDeck> & decks );
    void printDecks ( vector<CDeck> & decks );
    void printCharacters ( vector<shared_ptr<CCharacter>> & characters );
    void printSettings ( vector<pair<string,bool>> & menuItems );
    void printSaves ( vector<fs::directory_entry> & saves );
    void printMainMenu ( vector<string> & menuItems );
    /**
    * @brief Read a number from screen.
    * 
    * @param yCoord y coord
    * @param xCoord x coord
    * @param n max number of characters to read
    * @return int read number
    */
    int readNumber ( const size_t & yCoord, const size_t & xCoord, const size_t & n );
    /**
     * @brief Read string from screen
     * 
     * @param y y coordinate
     * @param x x coordinate
     * @param n max string size
     * @return string read string
     */
    string readString ( const size_t & y, const size_t & x, const size_t & n );
    /**
     * @brief Clear n spaces from screen.
     * 
     * @param y y coordinate
     * @param x x coordinate
     * @param nSpaces number of spaces to clear
     */
    void clearSpaces ( const size_t & y, const size_t & x, const size_t & nSpaces );
    /**
    * @brief Toggle enabled/disabled color in settings menu.
    * 
    * @param itemIndex index of item to toggle
    * @param menuItems items
    */
    void toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems );
    /**
     * @brief Check if a number is a valid deck size.
     */
    bool isValidDeckSize ( int & number );
    /**
     * @brief Initialize ncurses, color pairs, clear, refresh main window
     * 
     * @return true 
     * @return false 
     */
    bool initCurses ( void );
    /**
     * @brief Print error message in the top left corner
     * 
     * @param errorMessage message to print
     */
    void printError ( const char * errorMessage );
    /**
     * @brief Load everything necessary for creating a game.
     */
    bool loadNecessities ( map<string,shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks );
    vector<fs::directory_entry> loadSaves ( fs::path savePath );

    int m_XMax, m_YMax;
    int m_Height, m_Width;
    WINDOW * m_Win;
    size_t m_Highlight = 0;
    CGameSettings m_Settings;
};
#endif