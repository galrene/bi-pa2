#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>

#include "CCharacter.h"
#include "Constants.h"
#include "CDeck.h"
#include "CGameStateManager.h"

#include "CAttack.h"
#include "CDefense.h"
#include "CSpecial.h"
#include "CPassive.h"
#include "CSaver.h"

#include "CHuman.h"
#include "CBot.h"

using namespace std;
namespace fs = filesystem;

class CConfigParser {
  public:
   /**
    * @brief Construct a new CConfigParser at default location
    */
    CConfigParser ( void );
    CConfigParser ( const fs::path & location, const fs::path & logFilePath );
    /**
     * @brief load characters from ini files contained inside of a given directory
     * @param dirName name of directory containing character definitions, search in program main directory by default
     * @return loaded characters on success, empty vector on failing to access directory
     */
    map<string,shared_ptr<CCharacter>> loadCharacters ( const string & dirName );    map<string,shared_ptr<CCard>> loadCards ( const string & dirName );
    vector<CDeck> loadDecks ( const string & dirName, map<string,shared_ptr<CCard>> & cardDefinitions );
    /**
     * @brief Load a savegame from file
     * 
     * @param gsm created game from file
     * @param savePath path to savegame
     * @return true = sucessfully created a playable game, false = failed to load some component
     */
    bool loadSave ( CGameStateManager & gsm, const fs::path & savePath );
    /**
     * @brief Load players from directory
     * 
     * @param p1 loaded player1
     * @param p2 loaded player2 or bot
     * @param savePath 
     * @param isTwoPlayerGame whether to create a player or bot
     * @return true = success, false = failed to enter some directory or load a player from file
     */
    bool loadPlayers ( shared_ptr <CPlayer> & p1, shared_ptr<CPlayer> & p2, const fs::path & savePath, bool isTwoPlayerGame );
    /**
     * @brief Set the current working directory
     * 
     * @param location path which to set
     * @return true = exist, false = doesn't exist or isn't a directory
     */
    bool setPath ( const fs::path & location );
  private:
    bool constructCharacter ( const fs::directory_entry & entry,
                              map<string,shared_ptr<CCharacter>> & loadedCharacters );
    bool constructCard ( const fs::directory_entry & entry,
                         map<string,shared_ptr<CCard>> & loadedCards );
    bool constructPlayer ( shared_ptr<CPlayer> & player, const string & playerName, bool isBot );
    /**
     * @brief Load a single character form .ini file.
     * 
     * @param entry file from which to load character
     * @param loadedCharacters where to load the character
     * @return true = success, false = ini file is invalid
     */
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                map<string,shared_ptr<CCharacter>> & loadedCharacters );
    bool loadCardFromIni ( const fs::directory_entry & entry,
                            map<string,shared_ptr<CCard>> & loadedCards );
    bool loadDeckFromIni ( const fs::directory_entry & entry,
                           vector<CDeck> & loadedDecks,
                           map<string,shared_ptr<CCard>> & cardDefinitions );
    bool loadSettingsFromIni ( const fs::directory_entry & entry, CGameSettings & sett );
    bool isDeckValid ( const fs::directory_entry & entry, CDeck & deck, map<string,shared_ptr<CCard>> & cardDefinitions );
    bool createDirectory ( string & dirName );
    /**
     * @brief Read a simplified ini file.
     * 
     * @param iniPath path to ini file
     * @return header of a section inside the file
     */
    string readIni ( const fs::path & iniPath );
    /**
     * @brief Check if a directory_entry is an .ini file.
     * 
     * @param entry directory entry
     * @return true is an .ini file
     * @return false isn't an .ini file
     */
    bool isIni ( const fs::directory_entry & entry );
    /**
     * @brief Enter a directory from current working directory.
     * 
     * @param dirName 
     * @return true = success, false = couldn't enter directory
     */
    bool enterDirectory ( const string & dirName );
    /**
     * @brief Read key value pair from "line" into "key" and "value"
     * @return true = success, false = key value invalid or too many values in a line
     */
    bool readKeyValue ( const string & line, string & key, string & value );
    /**
     * @brief Check if files and directories necessary for creating a player exits.
     * 
     * @param card_definitons 
     * @param char_played 
     * @param char_loaded 
     * @param deck 
     * @param hand 
     * @return true = all exist, false = some directory doesn't exist
     */
    bool findPlayerFiles ( fs::directory_entry & card_definitons ,fs::directory_entry & char_played , fs::directory_entry & char_loaded ,fs::directory_entry & deck, fs::directory_entry & hand );

    vector<string> m_FailedToLoad;
    map<string,string> m_LoadedData;
    fs::path m_Path;
    fs::path m_LogFilePath;
    ofstream m_LogStream;
};