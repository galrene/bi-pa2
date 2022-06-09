#pragma once
#ifndef CConfigParser_H
#define CConfigParser_H
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>

#include "CCharacter.h"
#include "Constants.h"
#include "CDeck.h"
#include "CGameStateManager.h" // ! pls dont break everything

#include "CAttack.h"
#include "CDefense.h"
#include "CSpecial.h"
#include "CPassive.h"
#include "CSaver.h"

using namespace std;
namespace fs = filesystem;

class CConfigParser {
  public:
    CConfigParser ( void );
    explicit CConfigParser ( fs::path location );
    /**
     * @brief load characters from ini files contained inside of a given directory
     * @param dirName name of directory containing character definitions, search in program main directory by default
     * @return vector<shared_ptr<CCharacter>> of loaded characters on success
     * @return empty vector on failure to access directory
     */
    map<string,shared_ptr<CCharacter>> loadCharacters ( const string & dirName );
    map<string,shared_ptr<CCard>> loadCards ( const string & dirName );
    vector<CDeck> loadDecks ( const string & dirName, map<string,shared_ptr<CCard>> & cardDefinitions );
    bool loadSave ( CGameStateManager & gsm, fs::path & savePath );
    bool loadPlayers ( shared_ptr <CPlayer> & p1, shared_ptr<CPlayer> & p2, fs::path & savePath );

    bool setPath ( const fs::path & location );
  private:
    bool constructCharacter ( const fs::directory_entry & entry,
                              map<string,shared_ptr<CCharacter>> & loadedCharacters );
    bool constructCard ( const fs::directory_entry & entry,
                         map<string,shared_ptr<CCard>> & loadedCards );
    bool constructPlayer ( shared_ptr<CPlayer> & player, const string & playerName );
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                map<string,shared_ptr<CCharacter>> & loadedCharacters );
    bool loadCardFromIni ( const fs::directory_entry & entry,
                            map<string,shared_ptr<CCard>> & loadedCards );
    bool loadDeckFromIni ( const fs::directory_entry & entry,
                           vector<CDeck> & loadedDecks,
                           map<string,shared_ptr<CCard>> & setOfCards );
    bool loadSettingsFromIni ( const fs::directory_entry & entry, CGameSettings & sett );

    bool isDeckValid ( const fs::directory_entry & entry, CDeck & deck, map<string,shared_ptr<CCard>> & cardDefinitions );
    bool createDirectory ( string & dirName );
    string readIni ( const fs::path & iniPath );
    bool isIni ( const fs::directory_entry & entry );
    bool enterDirectory ( const string & dirName );
    bool readKeyValue ( const string & line, string & key, string & value );

    vector<string> m_FailedToLoad;
    map<string,string> m_LoadedData;
    fs::path m_Path;
};
#endif