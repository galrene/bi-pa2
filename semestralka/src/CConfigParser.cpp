#include "CConfigParser.h"

CConfigParser::CConfigParser ( void )
: m_Path ( defaultPath ), m_LogFilePath ( defaultLogFilePath / "parser_log.txt"  ) {
    m_LogStream = ofstream ( m_LogFilePath.generic_string() );
}

CConfigParser::CConfigParser ( const fs::path & location, const fs::path & logFilePath )
: m_Path ( location ), m_LogFilePath ( logFilePath / "parser_log.txt"  ) {
    m_LogStream = ofstream ( m_LogFilePath.generic_string() );
}

bool CConfigParser::enterDirectory ( const string & dirName ) {
    fs::path tmpPath = m_Path;
    tmpPath.append ( dirName );
    if ( ! fs::exists ( tmpPath ) ) {
        m_LogStream << "Directory " << tmpPath << " doesn't exist " << endl;
        return false;
    }
    if ( ! fs::is_directory ( tmpPath ) ) {
        m_LogStream << tmpPath << " isn't a directory" << endl;
        return false;
    }
    fs::perms p = status ( tmpPath ).permissions();
    if ( ( p & fs::perms::owner_read ) == fs::perms::none ||
         ( p & fs::perms::owner_write ) == fs::perms::none ) {
        m_LogStream << "The owner doesn't have the correct permissions to write and read " << tmpPath << endl;
        return false;
    }
    m_Path = tmpPath;
    return true;
}
bool CConfigParser::setPath ( const fs::path & location ) {
    if ( ! fs::exists ( location ) ) {
        m_LogStream << "Directory " << location << " doesn't exist " << endl;
        return false;
    }
    if ( ! fs::is_directory ( location ) ) {
        m_LogStream << location << " isn't a directory" << endl;
        return false;
    }
    m_Path = location;
    return true;
}
bool CConfigParser::isIni ( const fs::directory_entry & entry ) {
    if ( ! entry.is_regular_file() ) {
        m_LogStream << entry << " isn't a regular file" << endl;
        return false;
    }
    if ( entry.path().extension() != ".ini" ) {
        m_LogStream << entry << " isn't an ini file" << endl;
        return false;
    }
    return true;
}

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
                m_LogStream << "Each config file should have only one section" << endl;
                return "";
            }
            line.erase(line.begin());
            line.pop_back();
            header = line;
            foundHeader = true;
            continue;
        }
        if ( ! readKeyValue ( line, tmpKey, tmpVal ) ) {
            m_LogStream << "A key value pair in " << iniPath << " is invalid" << endl;
            return "";
        }
        m_LoadedData[tmpKey] = tmpVal;
    }
    if ( header == "" ) {
        m_LogStream << "Missing header in " << iniPath << endl;
        return "";
    }
    return header;
}

bool CConfigParser::constructCharacter ( const fs::directory_entry & entry,
                                         map<string,shared_ptr<CCharacter>> & loadedCharacters ) {
    CCharacter character ( m_LoadedData );
    if ( loadedCharacters.count ( m_LoadedData["name"] ) != 0 ) {
        m_LogStream << "Character with name \"" << m_LoadedData["name"] << "\" already exists" << endl;
        return false;
    }
    if ( ! character.buildCharacter() )
        return false;
    loadedCharacters [ m_LoadedData["name"] ] = make_shared<CCharacter> ( character );
    return true;
}
bool CConfigParser::loadCharacterFromIni ( const fs::directory_entry & entry, map<string,shared_ptr<CCharacter>> & loadedCharacters ) {
    if ( ! isIni ( entry ) ) {
        m_LogStream << entry.path().generic_string() << "isn't a .ini" << endl;
        return false;
    }
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) {
        m_LoadedData.clear();
        return false;
    }
    if ( header != "character" ) {
        m_LogStream << "No [character] section found in" << entry.path().generic_string() << endl;
        m_LoadedData.clear();
        return false;
    }
    try {
        if ( ! constructCharacter ( entry, loadedCharacters ) ) {
            m_LogStream << "Failed to set all of the required character attributes" << endl;
            m_LoadedData.clear();
            return false;
        }
    }
    catch ( const exception & e ) {
        m_LogStream << e.what() << '\n';
        m_LogStream << "Bad character that should be an integer or an invalid attribute name in " << entry.path() << '\n';
        m_LoadedData.clear();
        return false;
    }
    m_LoadedData.clear();
    return true;
}

bool CConfigParser::constructCard ( const fs::directory_entry & entry, map<string,shared_ptr<CCard>> & loadedCards ) {
    if ( m_LoadedData["type"] == "" ) {
        m_LogStream << "Missing type atribute in card " << entry << endl;
        return false;
    }
    if ( loadedCards.find ( m_LoadedData["name"] ) != loadedCards.end() ) {
        m_LogStream << "Card with name \"" << m_LoadedData["name"] << "\" already exists" << endl;
        return false;
    }
    else if ( m_LoadedData["type"] == "attack" ) {
        CAttack att ( m_LoadedData );
        if ( ! att.buildCard() )
            return false;
        loadedCards[ m_LoadedData["name"] ] = make_shared<CAttack> ( att );
    }
    else if ( m_LoadedData["type"] == "defense" ) {
        CDefense def ( m_LoadedData );
        if ( ! def.buildCard() )
            return false;
        loadedCards[ m_LoadedData["name"] ] = make_shared<CDefense> ( def );
    }
    else if ( m_LoadedData["type"] == "passive" ) {
        CPassive pass ( m_LoadedData );
        if ( ! pass.buildCard() )
            return false;
        loadedCards[ m_LoadedData["name"] ] = make_shared<CPassive> ( pass );
    }
    else if ( m_LoadedData["type"] == "special" ) {
        CSpecial spec ( m_LoadedData );
        if ( ! spec.buildCard() )
            return false;
        loadedCards[ m_LoadedData["name"] ] = make_shared<CSpecial> ( spec );
    }
    else {
        m_LogStream << "Unidentified card type \"" << m_LoadedData["type"] << "\"" << endl;
        return false;
    }
    return true;
}

bool CConfigParser::loadCardFromIni ( const fs::directory_entry & entry, map<string,shared_ptr<CCard>> & loadedCards ) {
    if ( ! isIni ( entry ) ) {
        m_LogStream << entry.path().generic_string() << " isn't a .ini" << endl;
        return false;
    }
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) {
        m_LoadedData.clear();
        return false;
    }
    if ( header != "card" ) {
        m_LogStream << "No [card] section found in" << entry.path().generic_string() << endl;
        m_LoadedData.clear();
        return false;
    }
    try {
        if ( ! constructCard ( entry, loadedCards ) ) {
            m_LogStream << "Failed to set all of the required card attributes" << endl;
            m_LoadedData.clear();
            return false;
        }
    }
    catch ( const exception & e ) {
        m_LogStream << e.what() << '\n';
        m_LogStream << "Bad character that should be an integer or an invalid attibute name in " << entry << endl;
        m_LoadedData.clear();
        return false;
    }
    m_LoadedData.clear();
    return true;
}

bool CConfigParser::isDeckValid ( const fs::directory_entry & entry, CDeck & deck, map<string,shared_ptr<CCard>> & cardDefinitions ) {
    for ( const auto & cardAndCount : m_LoadedData ) {
        if ( cardDefinitions.count ( cardAndCount.first ) == 0 ) {
            m_LogStream << "Card " << cardAndCount.first << " in deck " << entry.path().filename() << " is undefined." << endl;
            return false;
        }
        shared_ptr<CCard> card = cardDefinitions[cardAndCount.first];
        for ( const auto & digit : cardAndCount.second )
            if ( ! isdigit ( digit ) ) {
                m_LogStream << "Card count of " << cardAndCount.first << " must be a positive intiger, not \"" << cardAndCount.second << "\"" << endl;
                return false;
            }
        size_t count = stoi(cardAndCount.second);
        for ( size_t i = 0; i < count; i++ )
            deck.addCard ( card );
    }
    deck.setData ( m_LoadedData ); // pass the deck it's card count info
    return true;
}

bool CConfigParser::loadDeckFromIni ( const fs::directory_entry & entry, vector<CDeck> & loadedDecks, map<string,shared_ptr<CCard>> & cardDefinitions ) {
    if ( ! isIni ( entry ) ) {
        m_LogStream << entry.path().generic_string() << " isn't a .ini" << endl;
        return false;
    }
    CDeck deck ( entry.path().stem() );
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) {
        m_LoadedData.clear();
        return false;
    }
    if ( header != "deck" ) {
        m_LogStream << "No [deck] section found in" << entry.path().generic_string() << endl;
        m_LoadedData.clear();
        return false;
    }
    if ( ! isDeckValid ( entry, deck, cardDefinitions ) ) {
        m_LoadedData.clear();
        return false;
    }
    loadedDecks.push_back ( deck );
    m_LoadedData.clear();
    return true;
}

vector<CDeck> CConfigParser::loadDecks ( const string & dirName, map<string,shared_ptr<CCard>> & cardDefinitions ) {
    vector<CDeck> loadedDecks;
    if ( ! enterDirectory ( dirName ) )
        return {};
    for ( const auto & entry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadDeckFromIni ( entry, loadedDecks, cardDefinitions ) )
            m_FailedToLoad.push_back ( entry.path().generic_string() );
        m_LoadedData.clear();
    }
    if ( ! m_FailedToLoad.empty() ) {
        m_LogStream << "Failed to load files: " << endl;
        for ( const auto & x : m_FailedToLoad )
            m_LogStream << "\t" << x << endl;
    }
    if ( loadedDecks.empty() )
        m_LogStream << "No decks loaded." << endl;
    m_FailedToLoad.clear();
    m_Path = m_Path.parent_path();
    return loadedDecks;
}

map<string,shared_ptr<CCard>> CConfigParser::loadCards ( const string & dirName ) {
    map<string,shared_ptr<CCard>> loadedCards;
    if ( ! enterDirectory ( dirName ) )
        return {};
    for ( const auto & entry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadCardFromIni ( entry, loadedCards ) )
            m_FailedToLoad.push_back ( entry.path().generic_string() );
        m_LoadedData.clear();
    }
    if ( ! m_FailedToLoad.empty() ) {
        m_LogStream << "Failed to load files: " << endl;
        for ( const auto & x : m_FailedToLoad )
            m_LogStream << "\t" << x << endl;
    }
    if ( loadedCards.empty() )
        m_LogStream << "No cards loaded." << endl;
    m_FailedToLoad.clear();
    m_Path = m_Path.parent_path();
    return loadedCards;
}

map<string,shared_ptr<CCharacter>> CConfigParser::loadCharacters ( const string & dirName ) {
    map<string,shared_ptr<CCharacter>> loadedCharacters;

    if ( ! enterDirectory ( dirName ) )
        return {}; 
    for ( const auto & chEntry : fs::directory_iterator ( m_Path ) ) {
        if ( ! loadCharacterFromIni ( chEntry, loadedCharacters ) )
            m_FailedToLoad.push_back ( chEntry.path().generic_string() );
        m_LoadedData.clear();
    }
    if ( ! m_FailedToLoad.empty() ) {
        m_LogStream << "Failed to load files: " << endl;
        for ( const auto & x : m_FailedToLoad )
            m_LogStream << "\t" << x << endl;
    }
    if ( loadedCharacters.empty() )
        m_LogStream << "No characters loaded." << endl;
    m_FailedToLoad.clear();
    m_Path = m_Path.parent_path();
    return loadedCharacters;
}

bool CConfigParser::loadSettingsFromIni ( const fs::directory_entry & entry, CGameSettings & sett ) {
    if ( ! isIni ( entry ) ) {
        m_LogStream << entry.path().generic_string() << " isn't a .ini" << endl;
        return false;
    }
    string header = readIni ( entry.path().generic_string() );
    if ( header == "" ) 
        return false;
    if ( header != "settings" ) {
        m_LogStream << "No [settings] section found in" << entry.path().generic_string() << endl;
        return false;
    }
    try {
        if ( ! sett.load ( m_LoadedData ) ) {
            m_LogStream << "Failed to set all of the required settings" << endl;
            return false;
        }
    }
    catch ( const exception & e ) {
        m_LogStream << e.what() << '\n';
        m_LogStream << "Bad character that should be an integer or an invalid attibute name in " << entry << endl;
        return false;
    }
    return true;
}

bool CConfigParser::findPlayerFiles ( fs::directory_entry & card_definitons ,fs::directory_entry & char_played,
                                      fs::directory_entry & char_loaded,
                                      fs::directory_entry & deck,
                                      fs::directory_entry & hand ) {
    for ( const auto & entry : fs::directory_iterator ( m_Path ) ) {
        string fileName = entry.path().stem();
        if ( entry.is_directory() && fileName == "card_definitions" )
            card_definitons = entry;
        else if ( entry.is_regular_file() && fileName == "character_loaded" )
            char_loaded = entry;
        else if ( entry.is_regular_file() && fileName == "character_played" )
            char_played = entry;
        else if ( entry.is_regular_file() && fileName == "deck" )
            deck = entry;
        else if ( entry.is_regular_file() && fileName == "hand" )
            hand = entry;
    }
    if ( ! card_definitons.exists() || ! char_played.exists() || ! char_loaded.exists() || ! deck.exists() || ! hand.exists() ) {   
        m_LogStream << "Necessary player files not found in  " << m_Path << endl;
        return false;
    }
    return true;
}
bool CConfigParser::constructPlayer ( shared_ptr<CPlayer> & player, const string & playerName, bool isHuman ) {
    fs::directory_entry card_definitons;
    fs::directory_entry char_played;
    fs::directory_entry char_loaded;
    fs::directory_entry deck;
    fs::directory_entry hand;
    if ( ! findPlayerFiles ( card_definitons, char_played, char_loaded, deck, hand ) )
        return false;
    map<string,shared_ptr<CCard>> cards = loadCards ( card_definitons.path().filename().generic_string() );
    // 0th == deck, 1st == hand
    vector<CDeck> loadedDecks;
    map<string,shared_ptr<CCharacter>> loadedCharacter;
    map<string,shared_ptr<CCharacter>> playedCharacter;
    if ( cards.empty() || ! loadDeckFromIni ( deck, loadedDecks, cards ) ||
         ! loadDeckFromIni ( hand, loadedDecks, cards ) || ! loadCharacterFromIni ( char_loaded, loadedCharacter ) ||
         ! loadCharacterFromIni ( char_played, playedCharacter ) )
        return false;
    if ( loadedCharacter.begin()->first != playedCharacter.begin()->first ) {
        m_LogStream << playerName << "'s characters mismatch." << endl;
        return false;
    }

    if ( loadedDecks[1].size() != handSize ) {
        m_LogStream << playerName << "'s hand must contain " << handSize << " cards." << endl;
        return false;
    }
    isHuman
    ? player = make_shared<CHuman> ( CHuman ( playerName, *loadedCharacter.begin()->second, *playedCharacter.begin()->second, loadedDecks[0], loadedDecks[1] ) )
    : player = make_shared<CBot> ( CBot ( playerName, *loadedCharacter.begin()->second, *playedCharacter.begin()->second, loadedDecks[0], loadedDecks[1] ) );
    return true;
}

bool CConfigParser::loadPlayers ( shared_ptr <CPlayer> & p1, shared_ptr<CPlayer> & p2, const fs::path & savePath, bool isTwoPlayerGame ) {
    fs::directory_entry p1Dir;
    fs::directory_entry p2Dir;
    for ( const auto & entry : fs::directory_iterator ( savePath ) ) {
        string fileName = entry.path().filename();
        if ( entry.is_directory() && fileName.find("P1_") != fileName.npos )
            p1Dir = entry;
        else if ( entry.is_directory() && fileName.find("P2_") != fileName.npos )
            p2Dir = entry;
    }
    if ( ! p2Dir.exists() || ! p1Dir.exists() ) {
        m_LogStream << "Player directories not found in " << savePath << endl;
        return false;
    }
    if ( ! enterDirectory ( p1Dir.path().filename() ) ) {
        m_LogStream << "Cannot enter " << savePath << endl;
        return false;
    }
    string p1Name = p1Dir.path().filename().generic_string(); p1Name = p1Name.substr ( p1Name.find ( "P1_" ) + 3 );
    if ( ! constructPlayer ( p1, p1Name, true ) )
        return false;
    m_Path = m_Path.parent_path(); // leave p1Dir
    if ( ! enterDirectory ( p2Dir.path().filename() ) ) {
        m_LogStream << "Cannot enter " << savePath << endl;
        return false;
    }
    string p2Name = p2Dir.path().filename().generic_string(); p2Name = p2Name.substr ( p2Name.find ( "P2_" ) + 3 );
    if ( ! constructPlayer ( p2, p2Name, isTwoPlayerGame ) )
        return false;
    m_Path = m_Path.parent_path(); // leave p2Dir
    return true;
}

bool CConfigParser::loadSave ( CGameStateManager & gsm, const fs::path & savePath ) {
    if ( ! enterDirectory ( savePath.filename() ) )
        return false;
    CGameSettings settings;
    fs::path settingsPath = savePath; settingsPath /= defaultSettingsFileName;
    if ( ! fs::directory_entry ( settingsPath ).exists() ) {
        m_Path = savePath.parent_path();
        return false;
    }
    if ( ! loadSettingsFromIni ( fs::directory_entry ( settingsPath ), settings ) ) {
        m_LoadedData.clear();
        m_Path = savePath.parent_path();
        return false;
    }
    m_LoadedData.clear();
    shared_ptr<CPlayer> p1;
    shared_ptr<CPlayer> p2;
    if ( ! loadPlayers ( p1, p2, savePath, settings.isTwoPlayerGame() ) ) {
        m_LoadedData.clear();
        m_Path = savePath.parent_path();
        return false;
    }
    m_LoadedData.clear();
    gsm = CGameStateManager ( p1, p2, settings );
    m_Path = savePath.parent_path();
    return true;
}