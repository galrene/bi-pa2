#pragma once
#include <iostream>
#include <fstream>

#include <map>
#include <set>
#include <vector>
#include <string>
#include <filesystem>
#include "CCharacter.h"
#include "Constants.h"
#include "CCard.h"
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

    vector<shared_ptr<CCharacter>> loadCharacters ( const string & dirName );
    vector<shared_ptr<CCard>> loadCards ( const string & dirName );
    bool setPath ( const fs::path & location );
  private:
    /**
     * @brief check if currently loaded element in m_LoadedData already exists in container
     * 
     * @tparam C CCard or CCharacter
     * @param loaded container containing currently loaded cards / characters
     */
    bool constructCharacter ( const fs::directory_entry & entry,
                              vector<shared_ptr<CCharacter>> & loadedCharacters );
    bool constructCard ( const fs::directory_entry & entry,
                         vector<shared_ptr<CCard>> & loadedCards );                              
    string readIni ( const fs::path & iniPath );
    bool isIni ( const fs::directory_entry & entry );
    bool enterDirectory ( const string & dirName );
    bool readKeyValue ( const string & line, string & key, string & value );
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                vector<shared_ptr<CCharacter>> & loadedCharacters );
    bool loadCardFromIni ( const fs::directory_entry & entry,
                            vector<shared_ptr<CCard>> & loadedCards );
    bool createDirectory ( string & dirName );

    vector<string> failedToLoad;
    map<string,string> loadedData;
    set<string> loadedUniqueElements;
    fs::path m_Path;
};