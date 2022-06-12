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
#include "CMix.h"
#include "CSaver.h"
#include "CHuman.h"
#include "CBot.h"

using namespace std;
namespace fs = filesystem;
/**
 * @brief Parse configs, create cards, characters, players, from them. Can create a game form a directory.
 * Methods work by searching the CConfigParser working directory, this can be set using setPath.
 * Error messages are dumped into a log file.
 */
class CConfigParser {
  public:
   /**
    * @brief Construct a new CConfigParser at default location with default log file path.
    */
    CConfigParser ( void );
    /**
    * @brief Construct a new CConfigParser at given location with given log file path.
    * @param location where to set the parser's working directory
    * @param logFilePath where to dump error messages
    */
    CConfigParser ( const fs::path & location, const fs::path & logFilePath );
    /**
     * @brief Load characters from ini files contained inside of a given directory.
     * @param dirName name of directory containing character definitions
     * @return loaded characters on success, empty vector on failing to access directory
     */
    map<string,shared_ptr<CCharacter>> loadCharacters ( const string & dirName );    
    /**
     * @brief Load card definitions from given directory.
     * 
     * @param dirName name of directory where to load from
     * @return map<string,shared_ptr<CCard>> first = card name, second = loaded card; empty map on failing to enter the given directory
     */
    map<string,shared_ptr<CCard>> loadCards ( const string & dirName );
    /**
     * @brief Load decks from given directory.
     * 
     * @param dirName name of directory where to load from
     * @param cardDefinitions card definitions to check against (for only loading decks with real cards)
     * @return vector<CDeck> loaded decks, empty vector on failing to enter the given directory
     */
    vector<CDeck> loadDecks ( const string & dirName, map<string,shared_ptr<CCard>> & cardDefinitions );
    /**
     * @brief Load a savegame from a file.
     * 
     * @param gsm created game from file
     * @param savePath path to the savegame directory
     * @return true = sucessfully created a playable game, false = failed to load some component
     */
    bool loadSave ( CGameStateManager & gsm, const fs::path & savePath );
    /**
     * @brief Load players from directory.
     * 
     * @param p1 where to load player1
     * @param p2 where to load player2 or bot
     * @param savePath path to save file
     * @param isTwoPlayerGame whether to create a player or bot
     * @return true = success, false = failed to enter some directory or load a player from file
     */
    bool loadPlayers ( shared_ptr <CPlayer> & p1, shared_ptr<CPlayer> & p2,
                       const fs::path & savePath, bool isTwoPlayerGame );
    /**
     * @brief Set the current working directory.
     * 
     * @param location path which to set as WD
     * @return true = exists, false = doesn't exist, isn't a directory or owner doesn't have r or w or x permission for the directory
     */
    bool setPath ( const fs::path & location );
  private:
    /**
     * @brief Constructs a character from m_LoadedData if the correct data was loaded.
     * 
     * @param loadedCharacters where to construct
     * @return true = success, false = not enough data loaded or character with given name already exists
     */
    bool constructCharacter ( map<string,shared_ptr<CCharacter>> & loadedCharacters );
    /**
     * @brief Constructs a card based on loaded type.
     * 
     * @param entry file entry the card was loaded from
     * @param loadedCards where to construct card
     * @return true = card created, false = card with given name exists, not enough loaded data
     */
    bool constructCard ( const fs::directory_entry & entry,
                         map<string,shared_ptr<CCard>> & loadedCards );
    /**
     * @brief Constructs a player from directory.
     * 
     * @param player constructed player
     * @param playerName how to name the player
     * @param isHuman is the player going to be a bot or human
     * @return true = succesfully created a player; false = some necessary player file or directory doesn't exist, card amount mismatch, character mismatch.
     */
    bool constructPlayer ( shared_ptr<CPlayer> & player, const string & playerName,
                           bool isHuman );
    /**
     * @brief Load a character from .ini file.
     * 
     * @param entry file from which to load character
     * @param loadedCharacters where to load the character
     * @return true = success, false = ini file is invalid
     */
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                map<string,shared_ptr<CCharacter>> & loadedCharacters );
    /**
     * @brief Load a card from .ini file.
     * 
     * @param entry file from which to load card
     * @param loadedCards where to load the card
     * @return true = success, false = ini file is invalid
     */
    bool loadCardFromIni ( const fs::directory_entry & entry,
                          map<string,shared_ptr<CCard>> & loadedCards );
    /**
     * @brief Load a deck from .ini file.
     * 
     * @param entry file from which to load deck
     * @param loadedDecks where to load the deck
     * @param cardDefinitions existing card definitons to check against
     * @return true = success, false = ini file is invalid, specified card in deck doesn't exists
     */
    bool loadDeckFromIni ( const fs::directory_entry & entry,
                           vector<CDeck> & loadedDecks,
                           map<string,shared_ptr<CCard>> & cardDefinitions );
    /**
     * @brief Load settings from .ini file.
     * 
     * @param entry file from which to load settings
     * @param sett where to load the settings
     * @return true = success, false = invalid ini file or incomplete settings file.
     */
    bool loadSettingsFromIni ( const fs::directory_entry & entry, CGameSettings & sett );
    /**
     * @brief Check if a loaded deck is valid.
     * 
     * @param entry file with deck definition
     * @param deck checked deck
     * @param cardDefinitions existing card definitions
     * @return true = deck is fine, false = deck is incorrectly defined
     */
    bool isDeckValid ( const fs::directory_entry & entry, CDeck & deck,
                       map<string,shared_ptr<CCard>> & cardDefinitions );
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
     * @return true = is an .ini file, false = isn't an .ini file
     */
    bool isIni ( const fs::directory_entry & entry );
    /**
     * @brief Enter a directory from current working directory.
     * 
     * @param dirName directory name
     * @return true = success, false = directory doesn't exist/isn't a directory/owner doesn't have rwx perms
     */
    bool enterDirectory ( const string & dirName );
    /**
     * @brief Find a key value pair. from "line" into "key" and "value"
     * @param line where to search
     * @param key where to load key
     * @param value where to load value
     * @return true = success, false = key value invalid or too many values in a line
     */
    bool readKeyValue ( const string & line, string & key, string & value );
    /**
     * @brief Check if files and directories necessary for creating a player exits.
     * 
     * @param card_definitons where to put found card definitions file
     * @param char_played where to put found current character state
     * @param char_loaded where to put found character definition
     * @param deck where to put found player deck
     * @param hand where to put found player hand
     * @return true = all exist, false = some directory/file doesn't exist
     */
    bool findPlayerFiles ( fs::directory_entry & card_definitons, fs::directory_entry & char_played,
                           fs::directory_entry & char_loaded, fs::directory_entry & deck,
                           fs::directory_entry & hand );
    /**
     * @brief Names of failed to load files for logging purposes.
     */
    vector<string> m_FailedToLoad;
    /**
     * @brief Key value pairs loaded from ini file.
     */
    map<string,string> m_LoadedData;
    /**
     * @brief Working firectory.
     * 
     */
    fs::path m_Path;
    fs::path m_LogFilePath;
    ofstream m_LogStream;
};