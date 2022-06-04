#include "CMenu.h"

CMenu::CMenu ( void ) {
    initCurses();
    getmaxyx ( stdscr, m_YMax, m_XMax );
    m_Cols = m_YMax / 2 + 15;
    m_Lines = m_YMax / 3;
    /* lines, cols, int begin_y, int begin_x */
    m_Win = newwin ( m_Lines, m_Cols, ( m_YMax/2 - m_Lines/2 ), (m_XMax/2 - m_Cols/2) );
    // m_Win = newwin ( m_Lines, m_Cols, m_YMax/4, (m_XMax/2 - m_Cols/2) );
    /* for centering
    move ( m_YMax/2 , 0);
    hline ( '-', m_XMax );
    move ( 0, m_XMax/2 );
    vline ( '|', m_YMax );
    */
    keypad ( m_Win, TRUE ); // enable keypad inputs
    m_Settings = CGameSettings(); // might not be necessary
}
CMenu::~CMenu ( void ) {
    endwin();
}

void CMenu::printError ( const char * errorMessage ) {
    move (0,0);
    clrtoeol ();
    printw ( "ERROR: %s", errorMessage );
    refresh();
}

void CMenu::drawMenu ( const char * menuHeader  ) {
    wclear ( m_Win );
    /* lines, colons, beginY, beginX */
    box ( m_Win, 0, 0 );
    mvwprintw ( m_Win, 0, (m_Cols/2 - strlen(menuHeader)/2), "%s", menuHeader );
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
    refresh();
    return true;
}
void CMenu::toggleColor ( const size_t & itemIndex, vector<pair<string,bool>> & menuItems ) {
    wmove ( m_Win, 1+itemIndex,2 );
    wclrtoeol(m_Win);
    drawMenu ( "Settings" );
    wrefresh(m_Win);
    if ( ! menuItems[itemIndex].second ) {
        wattron ( m_Win, ON_PAIR );
        menuItems[itemIndex].second = true;
    }
    else {
        wattron ( m_Win, OFF_PAIR );
        menuItems[itemIndex].second = false;
    }
    mvwprintw ( m_Win , 1+itemIndex, 2, "%s", menuItems[itemIndex].first.c_str() );
    wattroff ( m_Win, ON_PAIR );
    wattroff ( m_Win, OFF_PAIR );
}
string CMenu::readString ( const size_t & y, const size_t & x, const size_t & n ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ n + 1 ];
    mvwgetnstr ( m_Win, y, x, buff, n );
    noecho();
    curs_set ( 0 );
    string str ( buff );
    delete buff;
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

bool CMenu::handleSettingsMovement (  vector<pair<string,bool>> & menuItems ) {
    // SIMPLIFY
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == m_Highlight && ! menuItems[i].second && i == 0 )
                wattron ( m_Win, OFF_SELECTED_PAIR );
            else if ( i == m_Highlight && menuItems[i].second && i == 0 )
                wattron ( m_Win, ON_SELECTED_PAIR );
            else if ( i == 0 && ! menuItems[i].second )
                wattron ( m_Win, OFF_PAIR );
            else if ( i == 0 && menuItems[i].second )
                wattron ( m_Win, ON_PAIR );
            if ( i == m_Highlight && i != 0 )
                wattron ( m_Win, A_REVERSE );
            
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].first.c_str() );
            if ( i == 1 )
                wprintw ( m_Win, " %d", m_Settings.getMaxDeckSize() );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, OFF_PAIR );
            wattroff ( m_Win, ON_SELECTED_PAIR );
            wattroff ( m_Win, OFF_SELECTED_PAIR );
            wattroff ( m_Win, A_REVERSE );
        }
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
            return false;
    }
    return true;
}
int CMenu::readNumber ( const size_t & yCoord, const size_t & xCoord, const size_t & n ) {
    curs_set ( 1 );
    echo();
    char * buff = new char [ n + 1 ];
    mvwgetnstr ( m_Win, yCoord, xCoord, buff, n );
    noecho();
    curs_set ( 0 );
    for ( size_t i = 0; i < n; i ++ )
        if ( ! isdigit ( buff[i] ) && buff[i] != '\0' ) {
            delete buff;
            return -1;
        }
    int x = atoi ( buff );
    delete buff;
    return x;
}

bool CMenu::isValidDeckSize ( int number ) {
    if ( number <= 0 ) {
        printw ( "ERROR: %d Must be a number\n", number );
        refresh();
        return false;
    }
    return true;
}
bool CMenu::handleSettings ( void ) {
    drawMenu ( "Settings" );
    vector<pair<string, bool>> menuItems = { {"Two-player game", m_Settings.isTwoPlayerGame() },
                                             {"Deck size:", false},
                                             {"-Return-", false} };
    m_Highlight = 0;
    while ( m_Highlight != 2 ) {
        if ( ! handleSettingsMovement ( menuItems ) )
            return false;
        if ( m_Highlight == 0 ) {
            toggleColor ( 0, menuItems );
            m_Settings.toggleSP();
        }
        if ( m_Highlight == 1 ) {
            clearSpaces ( 2, 3 + menuItems[m_Highlight].first.size(), defaultInputLengthDeckSize );
            int num = readNumber ( 2, 3 + menuItems[m_Highlight].first.size(), defaultInputLengthDeckSize );
            if ( isValidDeckSize ( num ) )
                m_Settings.setMaxDeckSize ( num );
            drawMenu ( "Settings" );
            wrefresh ( m_Win );
        }
    }
    refresh();
    return true;
}

bool CMenu::handleMenuMovement ( vector<string> & menuItems ) {
    while ( 1 ) {
        // draw menu and highlight currently selected
        for ( size_t i = 0; i < menuItems.size(); i++ ) {
            if ( i == m_Highlight )
                wattron ( m_Win, A_REVERSE );
            mvwprintw ( m_Win, i+1, 2, "%s", menuItems[i].c_str() );
            wattroff ( m_Win, A_REVERSE );
        }
        int res = handleNavigation ( menuItems.size() );
        if ( res == 1 )
            break;
        else if ( res == -1 )
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

string CMenu::chooseName ( const char * menuHeader ) {
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

shared_ptr<CCharacter> CMenu::chooseCharacter ( vector<shared_ptr<CCharacter>> & characters, const char * menuHeader ) {
    m_Highlight = 0;
    drawMenu ( menuHeader );
    if ( ! chooseCharacterMovement ( characters ) )
        return nullptr;
    return characters[m_Highlight];
}

shared_ptr<CPlayer> CMenu::createPlayer ( vector<shared_ptr<CCharacter>> & loadedCharacters, const char * menuHeader ) {
    string p_name = chooseName ( menuHeader );
    string header = p_name + "'s character:";
    shared_ptr<CCharacter> playerCharacter = chooseCharacter ( loadedCharacters, header.c_str() );
    if ( ! playerCharacter )
        return nullptr;
    return make_shared<CPlayer> ( CPlayer ( p_name, *playerCharacter ) );
}

bool CMenu::loadNecessities ( vector<shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks ) {
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

bool CMenu::loadingScreen ( vector<shared_ptr<CCharacter>> & characters, map<string,shared_ptr<CCard>> & cards, vector<CDeck> & decks ) {
    drawMenu ( "Loading screen" );
    if ( loadNecessities ( characters, cards, decks ) ) {
        mvwprintw ( m_Win, m_Lines/2-1, m_Cols/2 - strlen("Loading successful.")/2, "Loading successful." );
        mvwprintw ( m_Win, m_Lines/2, m_Cols/2 - strlen("Press any key to continue.")/2, "Press any key to continue." );
        wgetch ( m_Win );
        return true;
    }
    mvwprintw ( m_Win, m_Lines/2-1, m_Cols/2 - strlen("Error while loading necessities.")/2, "Error while loading necessities." );
    mvwprintw ( m_Win, m_Lines/2, m_Cols/2 - strlen("Press any key to continue.")/2, "Press any key to continue." );
    wgetch ( m_Win );
    return false;
}

bool CMenu::handleCreateMenu ( void ) {
    vector<shared_ptr<CCharacter>> characters;
    map<string,shared_ptr<CCard>> cards;
    vector<CDeck> decks;
    if ( ! loadingScreen ( characters, cards, decks ) )
        return true;
    shared_ptr<CPlayer> p1;
    shared_ptr<CPlayer> p2;
    p1 = createPlayer ( characters, "Player 1 name:" );
    if ( ! p1 )
        return false;
    if ( m_Settings.isTwoPlayerGame() ) {
        p2 = createPlayer ( characters, "Player 2 name:" );
        if ( ! p2 )
            return false;
    }
    // construct an AI Player with random character
    else if ( ! m_Settings.isTwoPlayerGame() )
        p2 = make_shared<CPlayer> ( CPlayer ( defaultBotNickname, *characters[random() % (characters.size () + 1)] ) );
        // create random deck for the bot
    CGameStateManager gsm ( p1, p2, m_Settings );
    if ( ! gsm.beginGame() )
        return false;
    return true;
}

bool CMenu::handleMainMenu ( void ) {
    drawMenu ( "Main menu" );
    vector<string> menuItems = { "New game", "Load saved game", "Settings", "-Quit-" };
    m_Highlight = 0;
    if ( ! handleMenuMovement ( menuItems ) )
        return false;
    switch ( m_Highlight ) {
    case 0:
        if ( ! handleCreateMenu () )
            return false;
        break;
    case 1:
        printError ( "Selected 1\n");
        break;
    case 2:
        if ( ! handleSettings () )
            return false;
        break;
    case 3:
        return false;
        break;
    default:
        break;
    }

    // printw ("Your choice was : %s\n",menuItems[highlight].c_str() );
    refresh();
    return true;
}
