#pragma once
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <functional>

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
     * @brief Render a menu box and header.
     * 
     * @param menuHeader header in the top middle of the window
     */
    void drawMenu ( const string & menuHeader );
    /**
     * @brief Control the settings menu.
     * 
     * @return true = pressed return button, false = CTRL-D
     */
    bool handleSettings ( void );
    /**
     * @brief Control the main menu.
     * 
     * @return 1 = createdGame, 2 = loadedGame. -1 == quit
     */
    int handleMainMenu ( CGameStateManager & gsm );
    /**
     * @brief Control the create game menus.
     * 
     * @return -1 = requested exit, 1 = successfuly created a game, 0 = game creation failure
     */
    int handleCreateMenu ( CGameStateManager & gsm );
    /**
     * @brief Control the menu for loading a game from a savefile.
     * 
     * @return 0 = couldn't load from savefile, 1 = successfully loaded a game, -1 = requested exit
     */
    int handleLoadGameMenu ( CGameStateManager & gsm );
  protected:
    /**
     * @brief Control the menu for choosing a deck.
     * 
     * @param decks decks to choose from
     * @return CDeck chosen deck, deck with empty string as name if quit requested
     */
    CDeck chooseDeckMenu ( vector<CDeck> & decks );
    /**
     * @brief Read a player name from the user.
     * 
     * @param menuHeader menu title
     * @return string mhosen name
     */
    string chooseNameMenu ( const char * menuHeader );
    /**
     * @brief Control the menu for choosing a character.
     * 
     * @param characters Characters to choose from.
     * @param menuHeader menu title
     * @return shared_ptr<CCharacter> chosen character
     */
    shared_ptr<CCharacter> chooseCharacter ( vector<shared_ptr<CCharacter>> & characters,
                                             const char * menuHeader );
    /**
     * @brief Control the menu for creating a player.
     * 
     * @param loadedCharacters characters to choose from
     * @param decks decks to choose from
     * @param menuHeader menu title
     * @return shared_ptr<CPlayer> created player, nullptr = exit requested
     */
    shared_ptr<CPlayer> createPlayerMenu ( map<string,shared_ptr<CCharacter>> & loadedCharacters,
                                           vector<CDeck> & decks, const char * menuHeader );
    /**
     * @brief Communicate the state of loading the necessities for creating players.
     * 
     * @param characters where to load characters
     * @param cards where to load cards 
     * @param decks where to load decks 
     * @return true = loading success, false = a necessity wasn't loaded
     */
    bool loadingScreen ( map<string,shared_ptr<CCharacter>> & characters,
                         map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks );
    /**
    * @brief Read user input for menu navigation.
    * 
    * @param menuSize number of menu items to navigate through
    * @return 1 = Enter, 0 = normal navigation or random button, -1 = CTRL-D
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
    void printError ( const string & errorMessage );
    /**
     * @brief Load everything necessary for creating a game.
     */
    bool loadNecessities ( map<string,shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks );
    /**
     * @brief Load directory entries with the save_game prefix.
     * 
     * @param savePath Path where to look.
     * @return vector<fs::directory_entry> loaded entries 
     */
    vector<fs::directory_entry> loadSaves ( fs::path savePath );

    int m_XMax, m_YMax;
    int m_Height, m_Width;
    WINDOW * m_Win;
    size_t m_Highlight = 0;
    CGameSettings m_Settings;
};