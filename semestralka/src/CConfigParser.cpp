// #pragma once
#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include "CCharacter.h"

using namespace std;
namespace fs = filesystem;

const fs::path defaultPath = fs::current_path().parent_path();

class CConfigParser {
  public:
    CConfigParser ( void );
    explicit CConfigParser ( fs::path location );
    vector<shared_ptr<CCharacter>> loadCharacters ( const string & dirName );
    bool setPath ( const fs::path & location );
    bool loadCards ();
  private:
    bool constructCharacter ( vector<shared_ptr<CCharacter>> & loadedCharacters,
                              const fs::directory_entry & chEntry );
    string readIni ( const fs::path & iniPath );
    bool isIni ( const fs::directory_entry & entry );
    bool enterDirectory ( const string & dirName );
    bool readKeyValue ( const string & line, string & key, string & value );
    bool loadCharacterFromIni ( const fs::directory_entry & entry,
                                vector<shared_ptr<CCharacter>> & loadedCharacters );

    vector<string> failedToLoad;
    map<string,string> loadedData;
    fs::path m_Path;
};
/**
 * @brief Construct a new CConfigParser
 * 
 * @param location
 */
CConfigParser::CConfigParser ( fs::path location )
: m_Path ( location ) {}
/**
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
    return header;
}

bool CConfigParser::constructCharacter ( vector<shared_ptr<CCharacter>> & loadedCharacters,
                                         const fs::directory_entry & chEntry ) {
    // stoi exception catch
    try {
        CCharacter character ( loadedData["name"],
                                loadedData["class"],
                                stoi(loadedData["hp"]),
                                stoi(loadedData["mana"]),
                                stoi(loadedData["strength"]) );
        if ( ! character.isSet() )
            return false;
        loadedCharacters.emplace_back ( make_shared<CCharacter> ( character ) );
    }
    catch ( const exception & e ) {
        failedToLoad.push_back ( chEntry.path().filename() );
        cerr << e.what() << " exception:\n";
        cerr << "Bad character that should be an integer or an invalid attribute name in " << chEntry.path() << '\n';
    }
    loadedData.clear();
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
    if ( header != "character" && header != "" ) {
        cerr << "No [character] section found in" << entry.path().generic_string() << endl;
        return false;
    }
    if ( header == "" ) 
        return false;
    if ( ! constructCharacter ( loadedCharacters, entry ) ) {
        cerr << "Failed to set all of the required character attributes" << endl;
        return false;
    }
    return true;
}

/**
 * @brief load characters from ini files contained inside of a given directory
 * @param dirName name of directory ( in the parent directory ) containing character definitions
 * @return vector<shared_ptr<CCharacter>> of loaded characters on success
 * @return empty vector on failure to access directory
 */
vector<shared_ptr<CCharacter>> CConfigParser::loadCharacters ( const string & dirName ) {
    vector<shared_ptr<CCharacter>> loadedCharacters;

    if ( ! enterDirectory ( dirName ) )
        return {}; 
    for ( const auto & chEntry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadCharacterFromIni ( chEntry, loadedCharacters ) ) {
            failedToLoad.push_back ( chEntry.path().generic_string() );
            continue;
        }
    }
    if ( ! failedToLoad.empty() ) {
        cerr << "Failed to load files: " << endl;
        for ( const auto & x : failedToLoad )
            cerr << "\t" << x << endl;
    }
    failedToLoad.clear();
    return loadedCharacters;
}

int main ( int argc, char const *argv[] ) {
    CConfigParser cfgp;
    vector<shared_ptr<CCharacter>> characters = cfgp.loadCharacters ( "characters" );

    return 0;
}
