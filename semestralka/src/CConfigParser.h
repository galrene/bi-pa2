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
    vector<shared_ptr<CCharacter>> loadCharacters ( const string & dirName );
    map<string,shared_ptr<CCard>> loadCards ( const string & dirName );
    vector<CDeck> loadDecks ( const string & dirName, map<string,shared_ptr<CCard>> & cardDefinitions );

    bool setPath ( const fs::path & location );
  private:
       bool constructCharacter ( const fs::directory_entry & entry,
                              vector<shared_ptr<CCharacter>> & loadedCharacters );
    bool constructCard ( const fs::directory_entry & entry,
                         map<string,shared_ptr<CCard>> & loadedCards );                              
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                vector<shared_ptr<CCharacter>> & loadedCharacters );
    bool loadCardFromIni ( const fs::directory_entry & entry,
                            map<string,shared_ptr<CCard>> & loadedCards );
    bool loadDeckFromIni ( const fs::directory_entry & entry,
                           vector<CDeck> & loadedDecks,
                           map<string,shared_ptr<CCard>> & setOfCards );
    bool isDeckValid ( const fs::directory_entry & entry, CDeck & deck, map<string,shared_ptr<CCard>> & cardDefinitions );
    bool createDirectory ( string & dirName );
    string readIni ( const fs::path & iniPath );
    bool isIni ( const fs::directory_entry & entry );
    bool enterDirectory ( const string & dirName );
    bool readKeyValue ( const string & line, string & key, string & value );

    vector<string> failedToLoad;
    map<string,string> loadedData;
    set<string> loadedUniqueElements;
    fs::path m_Path;
};
#endif