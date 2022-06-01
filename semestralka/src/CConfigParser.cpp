// #pragma once
#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include "CCharacter.h"
#include "Constants.h"
#include "CCard.h"

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

    vector<string> failedToLoad;
    map<string,string> loadedData;
    fs::path m_Path;
};
/*
 * @brief Construct a new CConfigParser at default location
 */
CConfigParser::CConfigParser ( void )
: m_Path ( defaultPath ) {}
/**
 * @brief enters a directory from current working directory
 * 
 * @param dirName 
 * @return true
 * @return false 
 */
bool CConfigParser::enterDirectory ( const string & dirName ) {
    m_Path = defaultPath;
    m_Path.append ( dirName );
    if ( ! fs::exists ( m_Path ) ) {
        cerr << "Directory " << m_Path << " doesn't exist " << endl;
        return false;
    }
    if ( ! fs::is_directory ( m_Path ) ) {
        cerr << m_Path << " isn't a directory" << endl;
        return false;
    }
    return true;
}

/**
 * @brief sets the current working directory
 * 
 * @param location path which to set
 * @return true if exists
 * @return false if doesn't exists or isn't a directory
 */
bool CConfigParser::setPath ( const fs::path & location ) {
    if ( ! fs::exists ( location ) ) {
        cerr << "Directory " << location << " doesn't exist " << endl;
        return false;
    }
    if ( ! fs::is_directory ( location ) ) {
        cerr << location << " isn't a directory" << endl;
        return false;
    }
    return true;
}


/**
 * @brief Checks if directory_entry is an .ini file
 * 
 * @param entry 
 * @return true 
 * @return false 
 */
bool CConfigParser::isIni ( const fs::directory_entry & entry ) {
    if ( ! entry.is_regular_file() ) {
        cerr << entry << " isn't a regular file" << endl;
        return false;
    }
    if ( entry.path().extension() != ".ini" ) {
        cerr << entry << " isn't an ini file" << endl;
        return false;
    }
    return true;
}
/**
 * @brief Read key = value pair from "line" into "key" and "value"
 * @return true 
 * @return false 
 */
bool CConfigParser::readKeyValue ( const string & line, string & key, string & value ) {
    stringstream ss ( line );
    int readCnt = 0;
    string word;
    while ( ss >> word ) {
        switch ( readCnt ) {
            case 0:
                key = word;
                break;
            case 1:
                if ( word != "=" )
                    return false;
                break;
            case 2:
                value = word;
                break;
            default:
                return false;;
        }
        readCnt++;
    }
    if ( readCnt != 3 )
        return false;
    return true;
}
/**
 * @brief read a simplified ini file
 * 
 * @param iniPath path to ini file
 * @return header of a section inside the file
 */
string CConfigParser::readIni ( const fs::path & iniPath ) {
    ifstream ifs ( iniPath.generic_string(), ios::binary );
    stringstream buffer;
    // inserts file content into stringstream
    buffer << ifs.rdbuf();

    string header;
    string tmpKey;
    string tmpVal;
    bool foundHeader = false;
    
    for ( string line ; getline ( buffer, line ) ; ) {
        if ( *line.begin() == '[' && *(--line.end()) == ']' ) {
            if ( foundHeader ) {
                cerr << "Each config file should have only one section" << endl;
                return "";
            }
            line.erase(line.begin());
            line.pop_back();
            header = line;
            foundHeader = true;
            continue;
        }
        if ( ! readKeyValue ( line, tmpKey, tmpVal ) ) {
            cerr << "A key value pair in " << iniPath << " is invalid" << endl;
            return "";
        }
        loadedData[tmpKey] = tmpVal;
    }
    if ( header == "" ) {
        cerr << "Missing header in " << iniPath << endl;
        return "";
    }
    return header;
}

bool CConfigParser::constructCharacter ( const fs::directory_entry & entry,
                                         vector<shared_ptr<CCharacter>> & loadedCharacters ) {
    CCharacter character ( loadedData );
    if ( ! character.buildCharacter() )
        return false;
    loadedCharacters.emplace_back ( make_shared<CCharacter> ( character ) );
    return true;
}
/**
 * @brief load a single character form .ini file into a vector of shared_ptr<CCharacter>
 * 
 * @param entry file from which to load character
 * @param loadedCharacters vector of characters
 * @return true if successful
 * @return false if ini file is invalid
 */
bool CConfigParser::loadCharacterFromIni ( const fs::directory_entry & entry, vector<shared_ptr<CCharacter>> & loadedCharacters ) {
    if ( ! isIni ( entry ) ) {
        cerr << entry.path().generic_string() << "isn't a .ini" << endl;
        return false;
    }
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) 
        return false;
    if ( header != "character" ) {
        cerr << "No [character] section found in" << entry.path().generic_string() << endl;
        return false;
    }
    try {
        if ( ! constructCharacter ( entry, loadedCharacters ) )
            cerr << "Failed to set all of the required character attributes" << endl;
    }
    catch ( const exception & e ) {
        std::cerr << e.what() << '\n';
        cerr << "Bad character that should be an integer or an invalid attribute name in " << entry.path() << '\n';
        return false;
    }
    
    return true;
}

/**
 * @brief load characters from ini files contained inside of a given directory
 * @param dirName name of directory containing character definitions, search in program main directory by default
 * @return vector<shared_ptr<CCharacter>> of loaded characters on success
 * @return empty vector on failure to access directory
 */
vector<shared_ptr<CCharacter>> CConfigParser::loadCharacters ( const string & dirName ) {
    vector<shared_ptr<CCharacter>> loadedCharacters;

    if ( ! enterDirectory ( dirName ) )
        return {}; 
    for ( const auto & chEntry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadCharacterFromIni ( chEntry, loadedCharacters ) )
            failedToLoad.push_back ( chEntry.path().generic_string() );
        loadedData.clear();
    }
    if ( ! failedToLoad.empty() ) {
        cerr << "Failed to load files: " << endl;
        for ( const auto & x : failedToLoad )
            cerr << "\t" << x << endl;
    }
    failedToLoad.clear();
    return loadedCharacters;
}
bool CConfigParser::constructCard ( const fs::directory_entry & entry, vector<shared_ptr<CCard>> & loadedCards ) {
    if ( loadedData["type"] == "" ) {
        cerr << "Missing type atribute in card " << entry << endl;
        return false;
    }
    else if ( loadedData["type"] == "attack" ) {
        CAttack att ( loadedData );
        if ( ! att.buildCard() )
            return false;
        loadedCards.emplace_back ( make_shared<CAttack> ( att ) );
    }
    else if ( loadedData["type"] == "defense" ) {
        CDefense def ( loadedData );
        if ( ! def.buildCard() )
            return false;
        loadedCards.emplace_back ( make_shared<CDefense> ( def ) );
    }
    else if ( loadedData["type"] == "passive" ) {
        CPassive pass ( loadedData );
        if ( ! pass.buildCard() )
            return false;
        loadedCards.emplace_back ( make_shared<CPassive> ( pass ) );
    }
    else if ( loadedData["type"] == "special" ) {
        CSpecial spec ( loadedData );
        if ( ! spec.buildCard() )
            return false;
        loadedCards.emplace_back ( make_shared<CSpecial> ( spec ) );
    }
    else {
        cerr << "Unidentified card type \"" << loadedData["type"] << "\"" << endl;
        return false;
    }
    return true;
}

bool CConfigParser::loadCardFromIni ( const fs::directory_entry & entry, vector<shared_ptr<CCard>> & loadedCards ) {
    if ( ! isIni ( entry ) ) {
        cerr << entry.path().generic_string() << " isn't a .ini" << endl;
        return false;
    }
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) 
        return false;
    if ( header != "card" ) {
        cerr << "No [card] section found in" << entry.path().generic_string() << endl;
        return false;
    }
    try {
        if ( ! constructCard ( entry, loadedCards ) )
            cerr << "Failed to set all of the required card attributes" << endl;
    }
    catch ( const exception & e ) {
        cerr << e.what() << '\n';
        cerr << "Bad character that should be an integer or an invalid attibute name in " << entry << endl;
        return false;
    }
    return true;
}

vector<shared_ptr<CCard>> CConfigParser::loadCards ( const string & dirName ) {
    vector<shared_ptr<CCard>> loadedCards;
    if ( ! enterDirectory ( dirName ) )
        return {};
    for ( const auto & entry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadCardFromIni ( entry, loadedCards ) )
            failedToLoad.push_back ( entry.path().generic_string() );
        loadedData.clear();
    }
    if ( ! failedToLoad.empty() ) {
        cerr << "Failed to load files: " << endl;
        for ( const auto & x : failedToLoad )
            cerr << "\t" << x << endl;
    }
    failedToLoad.clear();
    return loadedCards;
}

int main ( int argc, char const *argv[] ) {
    CConfigParser cfgp;
    vector<shared_ptr<CCharacter>> characters = cfgp.loadCharacters ( "characters" );
    vector<shared_ptr<CCard>> cards = cfgp.loadCards ( "cards" );
    return 0;
}
