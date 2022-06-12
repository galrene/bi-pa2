#include "CMenu.h"

CMenu::CMenu ( void ) {
    if ( ! initCurses() ) {
        cerr << "Your terminal doesn't support colors" << endl;
        return;
    }
    getmaxyx ( stdscr, m_YMax, m_XMax );
    m_Width = m_YMax / 2 + 15;
    m_Height = m_YMax / 3;
    /* lines, cols, int begin_y, int begin_x */
    m_Win = newwin ( m_Height, m_Width, ( m_YMax/2 - m_Height/2 ), (m_XMax/2 - m_Width/2) );
    keypad ( m_Win, TRUE ); // enable keypad inputs
}
CMenu::~CMenu ( void ) {
    delwin ( m_Win );
    endwin();
}
void CMenu::printError ( const string & errorMessage) {
    move (0,0);
    clrtoeol ();
    printw ( "ERROR: %s", errorMessage.c_str() );
    refresh();
}
void CMenu::drawMenu ( const string & menuHeader  ) {
    wclear ( m_Win );
    box ( m_Win, 0, 0 );
    mvwprintw ( m_Win, 0, (m_Width/2) - (menuHeader.size()/2), "%s", menuHeader.c_str() );
    wrefresh (m_Win);
}
bool CMenu::initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
    if ( ! has_colors () )
        return false;
    start_color();
    init_pair ( 3, COLOR_WHITE, COLOR_RED );
    init_pair ( 2, COLOR_WHITE, COLOR_GREEN );
    init_pair ( 4, COLOR_GREEN, COLOR_WHITE );
    init_pair ( 1, COLOR_RED, COLOR_WHITE );
    curs_set ( 0 ); // hide cursor
    #define OFF_PAIR COLOR_PAIR(3)
    #define ON_PAIR COLOR_PAIR(2)
    #define ON_SELECTED_PAIR COLOR_PAIR(4)
    #define OFF_SELECTED_PAIR COLOR_PAIR(1)
    clear();
    refresh();
    return true;
}
void CMenu::toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems ) {
    clearSpaces ( 2+itemIndex, (getmaxx(m_Win)/2) - ( menuItems[itemIndex].first.size() / 2) -1, menuItems[itemIndex].first.size() );
    if ( ! menuItems[itemIndex].second ) {
        wattron ( m_Win, ON_PAIR );
        menuItems[itemIndex].second = true;
    }
    else {
        wattron ( m_Win, OFF_PAIR );
        menuItems[itemIndex].second = false;
    }
    mvwprintw ( m_Win , 2+itemIndex, (getmaxx(m_Win)/2) - ( menuItems[itemIndex].first.size() / 2)-1, "%s", menuItems[itemIndex].first.c_str() );
    wattroff ( m_Win, ON_PAIR );
    wattroff ( m_Win, OFF_PAIR );
    wrefresh(m_Win);
}
string CMenu::readString ( const size_t & y, const size_t & x, const size_t & n ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ n + 1 ];
    for ( size_t i = 0; i < n + 1; i++ )
        buff[i] = 0;
    mvwgetnstr ( m_Win, y, x, buff, n );
    noecho();
    curs_set ( 0 );
    string str ( buff );
    delete [] buff;
    return str;
}
int CMenu::handleNavigation ( const size_t & menuSize ) {
    int button;
    button = wgetch ( m_Win );
    switch ( button ) {
        case KEY_UP:
            if ( m_Highlight )
                m_Highlight--;
            else m_Highlight = menuSize - 1;
            break;
        case KEY_DOWN:
            if ( m_Highlight != menuSize - 1)
                m_Highlight++;
            else m_Highlight = 0;
            break;
        case ('d' & 0x1F):
            return -1;
        case ('c'): // for clearing error messages in the top left corner
            move(0,0);
            clrtoeol();
            refresh();
            break;
        default:
            break;
    }
    // pressed enter
    if ( button == 10 )
        return 1;
    return 0;
}
int CMenu::readNumber ( const size_t & yCoord, const size_t & xCoord, const size_t & n ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ n + 1 ];
    for ( size_t i = 0; i < n + 1; i++ )
        buff[i] = 0;
    mvwgetnstr ( m_Win, yCoord, xCoord, buff, n );
    noecho();
    curs_set ( 0 );
    for ( size_t i = 0; i < n; i ++ )
        if ( ! isdigit ( buff[i] ) && buff[i] != '\0' ) {
            delete [] buff;
            return -1;
        }
    int x = atoi ( buff );
    delete [] buff;
    return x;
}
bool CMenu::isValidDeckSize ( int & number ) {
    if ( number <= 0 ) {
        printError ( "Must be a number" );
        return false;
    }
    else if ( ( size_t ) number < handSize ) {
        printError ( "Must be bigger or equal to hand size" );
        return false;
    }
    return true;
}
void CMenu::clearSpaces ( const size_t & y, const size_t & x, const size_t & nSpaces ) {
    string str;
    for ( size_t i = 0; i < nSpaces; i++ )
        str += " ";
    mvwprintw ( m_Win, y, x, "%s", str.c_str() ); 
}
// -----------------------------------------------------------------------------------------------------
void CMenu::printSettings ( vector<pair<string,bool>> & menuItems ) {
    for ( size_t i = 0; i < menuItems.size(); i++ ) {
        if ( i == m_Highlight && ! menuItems[i].second && (i == 0 || i == 1) )
            wattron ( m_Win, OFF_SELECTED_PAIR );
        else if ( i == m_Highlight && menuItems[i].second && (i == 0 || i == 1) )
            wattron ( m_Win, ON_SELECTED_PAIR );
        else if ( (i == 0 || i == 1) && ! menuItems[i].second )
            wattron ( m_Win, OFF_PAIR );
        else if ( (i == 0 || i == 1) && menuItems[i].second )
            wattron ( m_Win, ON_PAIR );
        if ( i == m_Highlight && i != 0 && i != 1 )
            wattron ( m_Win, A_REVERSE );
        
        mvwprintw ( m_Win, i+2, (getmaxx(m_Win)/2) - ( menuItems[i].first.size() / 2) - 1 , "%s", menuItems[i].first.c_str() );
        if ( i == 2 )
            wprintw ( m_Win, " %ld", m_Settings.getMaxDeckSize() );
        wattroff ( m_Win, OFF_PAIR );
        wattroff ( m_Win, OFF_PAIR );
        wattroff ( m_Win, ON_SELECTED_PAIR );
        wattroff ( m_Win, OFF_SELECTED_PAIR );
        wattroff ( m_Win, A_REVERSE );
        wrefresh ( m_Win );
    }
}
bool CMenu::handleSettingsMovement (  vector<pair<string,bool>> & menuItems ) {
    while ( 1 ) {
        printSettings ( menuItems );
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}
bool CMenu::handleSettings ( void ) {
    drawMenu ( "Settings" );
    vector<pair<string, bool>> menuItems = { { "Two-player game", m_Settings.isTwoPlayerGame() },
                                             { "Cheeky mode", m_Settings.isCheeky() },
                                             { "Deck size:", false },
                                             { "-Return-", false } };
    m_Highlight = 0;
    while ( m_Highlight != 3 ) {
        if ( ! handleSettingsMovement ( menuItems ) )
            return false;
        if ( m_Highlight == 0 ) {
            toggleColor ( 0, menuItems );
            m_Settings.toggleSP();
        }
        if ( m_Highlight == 1 ) {
            toggleColor ( 1, menuItems );
            m_Settings.toggleCheeky();
        }
        if ( m_Highlight == 2 ) {
            clearSpaces ( 4, (getmaxx(m_Win)/2) - ( menuItems[2].first.size() / 2) + menuItems[m_Highlight].first.size(), defaultInputLengthDeckSize );
            int num = readNumber ( 4, (getmaxx(m_Win)/2) - ( menuItems[2].first.size() / 2) + menuItems[m_Highlight].first.size(), defaultInputLengthDeckSize );
            if ( isValidDeckSize ( num ) )
                m_Settings.setMaxDeckSize ( num );
            drawMenu ( "Settings" );
            wrefresh ( m_Win );
        }
    }
    return true;
}
// -----------------------------------------------------------------------------------------------------
void CMenu::printCharacters ( vector<shared_ptr<CCharacter>> & characters ) {
    for ( size_t i = 0; i < characters.size(); i++ ) {
        if ( i == m_Highlight )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, 2+i, 2, "%s",characters[i]->getHeader().c_str() );
        wattroff ( m_Win, A_REVERSE );
    }
}
bool CMenu::chooseCharacterMovement ( vector<shared_ptr<CCharacter>> & characters ) {
    while ( 1 ) {
        printCharacters ( characters );
        int res = handleNavigation ( characters.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}
shared_ptr<CCharacter> CMenu::chooseCharacter ( vector<shared_ptr<CCharacter>> & characters, const char * menuHeader ) {
    m_Highlight = 0;
    drawMenu ( menuHeader );
    if ( ! chooseCharacterMovement ( characters ) )
        return nullptr;
    return characters[m_Highlight];
}

string CMenu::chooseNameMenu ( const char * menuHeader ) {
    drawMenu ( menuHeader );
    string str;
    while ( 1 ) {
        clearSpaces ( 2, 2, maxNameLength );
        str = readString ( 2, 2, maxNameLength );
        if ( str == "" )
            printError ( "Name field cannot be empty.\n" );
        else break;
    }
    return str;
}
// -----------------------------------------------------------------------------------------------------
void CMenu::printDecks ( vector<CDeck> & decks ) {
    for ( size_t i = 0; i < decks.size(); i++ ) {
        if ( i == m_Highlight )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, 2+i, 2, "%s",decks[i].getName().c_str() );
        wattroff ( m_Win, A_REVERSE );
    }
}
bool CMenu::chooseDeckMovement ( vector<CDeck> & decks ) {
    while ( 1 ) {
        printDecks ( decks );
        int res = handleNavigation ( decks.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}
CDeck CMenu::chooseDeckMenu ( vector<CDeck> & decks ) {
    m_Highlight = 0;
    drawMenu ( "Choose a deck:" );
    while ( 1 ) {
        if ( ! chooseDeckMovement ( decks ) )
            return CDeck ("");
        if ( decks[m_Highlight].size() > m_Settings.getMaxDeckSize() )
            printError ( "Selected deck is too large" );
        else
            break;
    }
    return decks[m_Highlight];
}
// -----------------------------------------------------------------------------------------------------
shared_ptr<CPlayer> CMenu::createPlayerMenu ( map<string,shared_ptr<CCharacter>> & loadedCharacters, vector<CDeck> & decks, const char * menuHeader ) {
    string p_name = chooseNameMenu ( menuHeader );
    string header = p_name + "'s character:";
    vector<shared_ptr<CCharacter>> charactersVec;
    for ( const auto & x : loadedCharacters )
        charactersVec.push_back ( x.second );
    shared_ptr<CCharacter> playerCharacter = chooseCharacter ( charactersVec, header.c_str() );
    if ( ! playerCharacter )
        return nullptr;
    CDeck deck = chooseDeckMenu ( decks );
    if ( deck.getName() == "" )
        return nullptr;
    return make_shared<CHuman> ( CHuman ( p_name, *playerCharacter, *playerCharacter, deck ) );
}

bool CMenu::loadNecessities ( map<string,shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks ) {
    CConfigParser parser;
    if (  characters = parser.loadCharacters ( defaultCharacterDirectory ); characters.empty() ) {
            printError ( "No characters loaded, check log for more info.\n" );
            return false;
    }
    if (  cards = parser.loadCards ( defaultCardDirectory ); cards.empty() ) {
            printError ( "No cards loaded, check log for more info.\n" );
            return false;
    }
    if (  decks = parser.loadDecks ( defaultDeckDirectory, cards ); decks.empty() ) {
            printError ( "No decks loaded, check log for more info.\n" ); 
            return false;
    }
    return true;
}

bool CMenu::loadingScreen ( map<string,shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks ) {
    drawMenu ( "Loading screen" );
    if ( loadNecessities ( characters, cards, decks ) ) {
        mvwprintw ( m_Win, m_Height/2-1, (m_Width/2) - 9, "Loading successful." );
        mvwprintw ( m_Win, m_Height/2, (m_Width/2) - 13, "Press any key to continue." );
        wgetch ( m_Win );
        return true;
    }
    mvwprintw ( m_Win, m_Height/2-1, (m_Width/2) - 16, "Error while loading necessities." );
    mvwprintw ( m_Win, m_Height/2, m_Width/2 - 13, "Press any key to continue." );
    wgetch ( m_Win );
    return false;
}
int CMenu::handleCreateMenu ( CGameStateManager & gsm ) {
    map<string,shared_ptr<CCharacter>> characters;
    map<string,shared_ptr<CCard>> cards;
    vector<CDeck> decks;
    if ( ! loadingScreen ( characters, cards, decks ) )
        return 0;
    shared_ptr<CPlayer> p1;
    shared_ptr<CPlayer> p2;
    p1 = createPlayerMenu ( characters, decks, "Player 1 name:" );
    if ( ! p1 )
        return -1;
    if ( m_Settings.isTwoPlayerGame() ) {
        p2 = createPlayerMenu ( characters, decks, "Player 2 name:" );
        if ( p2->getName() == p1->getName() ) {
            printError ( "Name already taken!" );
            return 0;
        }
        if ( ! p2 )
            return -1;
    }
    // construct an AI Player with random deck
    else if ( ! m_Settings.isTwoPlayerGame() ) {
        shared_ptr<CCharacter> randomCharacter = characters.begin()->second;
        p2 = make_shared<CBot> ( CBot ( defaultBotNickname, *randomCharacter, *randomCharacter, decks[ random() % decks.size () ] ) );
    }

    gsm = CGameStateManager ( p1, p2, m_Settings );
    return 1;
}
void CMenu::printSaves ( vector<fs::directory_entry> & saves ) {
    for ( size_t i = 0; i < saves.size(); i++ ) {
        if ( i == m_Highlight )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, 1+i, 1, "%s", saves[i].path().filename().c_str() );
        wattroff ( m_Win, A_REVERSE );
    }
}
vector<fs::directory_entry> CMenu::loadSaves ( fs::path savePath ) {
    vector<fs::directory_entry> saves;
    for ( const auto & entry : fs::directory_iterator ( savePath ) ) {
        string fileName = entry.path().filename().generic_string();
        if ( entry.is_directory() && fileName.find("save_game") != fileName.npos )
            saves.push_back ( entry );
    }
    return saves;
}
bool CMenu::loadMenuMovement ( vector<fs::directory_entry> & saves ) {
    while ( 1 ) {
        printSaves ( saves );
        int res = handleNavigation ( saves.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}

int CMenu::handleLoadGameMenu ( CGameStateManager & gsm ) {
    vector<fs::directory_entry> saves = loadSaves ( defaultSaveLocation );
    m_Highlight = 0;
    if ( saves.empty() ) {
        printError ( "No potential saves found in the configured savefile directory " + defaultSaveLocation.generic_string() );
        return 0;
    }
    drawMenu ( "Choose a save:" );
    if ( ! loadMenuMovement ( saves ) )
        return -1;
    CConfigParser parser;
    fs::path saveGamePath = defaultSaveLocation / saves[m_Highlight] ;
    if ( ! parser.loadSave ( gsm, saveGamePath ) ) {
        printError ( "Unable to load game from save." );
        return 0;
    }
    return 1;
}
// -----------------------------------------------------------------------------------------------------
void CMenu::printMainMenu ( vector<string> & menuItems ) {
    for ( size_t i = 0; i < menuItems.size(); i++ ) {
        if ( i == m_Highlight )
            wattron ( m_Win, A_REVERSE );
        mvwprintw ( m_Win, i+2, (getmaxx(m_Win) / 2) - (menuItems[i].size()/2), "%s", menuItems[i].c_str() );
        wattroff ( m_Win, A_REVERSE );
    }
}
bool CMenu::handleMenuMovement ( vector<string> & menuItems ) {
    while ( 1 ) {
        printMainMenu ( menuItems );
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}

int CMenu::handleMainMenu ( CGameStateManager & gsm ) {
    vector<string> menuItems = { "New game", "Load saved game", "Settings", "-Quit-" };
    int res;
    while ( 1 ) {
        m_Highlight = 0;
        drawMenu ( "Main menu" );
        if ( ! handleMenuMovement ( menuItems ) )
            return -1;
        if ( m_Highlight == 0 ) {
            if ( res = handleCreateMenu ( gsm ); res == -1 )
                return -1;
            else if ( res == 1 )
                return 1;
        }    
        else if ( m_Highlight == 1 ) {
            if ( res = handleLoadGameMenu ( gsm ); res == -1 )
                return -1;
            else if ( res == 1 )
                return 2;
        }
        else if ( m_Highlight == 2 ) {
            if ( ! handleSettings () )
                return -1;
        }
        else if ( m_Highlight == 3 )
            return -1;

    }
    return -1;
}
