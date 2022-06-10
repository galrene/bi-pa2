#include "CGameStateManager.h"

CGameStateManager::CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett )
: m_Player1 ( p1 ), m_Player2 ( p2 ), m_Settings ( sett ), m_OnTurn ( nullptr ), m_Info ( nullptr ) {}

CGameStateManager::~CGameStateManager ( void ) {
  if ( m_Info )
    delwin ( m_Info );
}
int CGameStateManager::handleMenu ( void ) {
  vector<string> menuItems = { "Exit to main menu", "Save game", "Return to game" };
  wclear ( m_Info );
  box ( m_Info, 0, 0 );
  wattron ( m_Info, A_BOLD );
  for ( size_t i = 0; i < menuItems.size(); i++ )
    mvwprintw ( m_Info, i+1, getmaxx(m_Info)/2 - ( menuItems[i].size() + 4 )/2, "%s (%ld)", menuItems[i].c_str(), i );
  wattroff ( m_Info, A_BOLD );
  int a;
  wrefresh ( m_Info );
  switch ( (a = getch()) ) {
  case '0':
    return 0;
    break;
  case '1':
    return 1;
    break;
  case '2':
    return 2;
    break;
  case ('d' & 0x1F):
    return -1;
    break;
  default:
    break;
  }
  return 2;
}
void CGameStateManager::dealCards ( void ) {
  m_Player1->shuffleDeck();
  m_Player2->shuffleDeck();
  m_Player1->drawCard(handSize);
  m_Player2->drawCard(handSize);
}
void CGameStateManager::decideTurnFromSettings ( void ) {
   m_Settings.firstOnTurn() ? m_OnTurn = m_Player1 : m_OnTurn = m_Player2;
}

void CGameStateManager::renderPlayerStats ( void ) {
    m_Player1->renderPlayer();
    m_Player2->renderPlayer();
}
void CGameStateManager::renderHands ( void ) {
    shared_ptr<CPlayer> offTurn = m_OnTurn == m_Player1 ? m_Player2 : m_Player1;
    if ( m_Settings.isCheeky() )
      offTurn->renderHand();
    m_OnTurn->renderHand();
}

void CGameStateManager::setWindows ( vector<WINDOW*> & p1_cards, WINDOW * p1_stats,
                                     vector<WINDOW*> & p2_cards, WINDOW * p2_stats, WINDOW * info ) {
  m_Player1->setCardWins ( p1_cards );
  m_Player1->setStatsWin ( p1_stats );
  m_Player2->setCardWins ( p2_cards );
  m_Player2->setStatsWin ( p2_stats );
  m_Info = info;
}

void CGameStateManager::whoIsOnTurn () {
  int yMax, xMax;
  getmaxyx ( m_Info, yMax, xMax );
  wclear ( m_Info );
  string str = m_OnTurn->getName() + "'s turn";;
  if ( m_OnTurn == m_Player1 ) {
    mvwaddch ( m_Info, yMax/2, xMax/2, ACS_UARROW );
    mvwprintw  ( m_Info, yMax/2 - 1, xMax/2 - str.size() / 2, "%s", str.c_str() );
  }
  else {
    mvwaddch ( m_Info, yMax/2, xMax/2, ACS_DARROW );
    mvwprintw  ( m_Info, yMax/2 + 1, xMax/2 - str.size() / 2, "%s", str.c_str() );
  }
  wrefresh ( m_Info );
}

void CGameStateManager::endTurn ( void ) {
  m_OnTurn->fillHand();
  m_OnTurn->fillMana();
  if ( ! m_Settings.isCheeky () )
    m_OnTurn->hideHand();
  m_OnTurn == m_Player1
  ? m_OnTurn = m_Player2
  : m_OnTurn = m_Player1;
  printMess ( "Turn ended." );
}

int CGameStateManager::pickCard ( void ) {
  wclear ( m_Info );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, (getmaxx(m_Info)/2) - 13 ,"Pick a card to throw away." );
  wrefresh ( m_Info );
  return getch() - '0';
}

void CGameStateManager::discardCard ( void ) {
  int a = pickCard ();
  if ( a < 0 || a >= (int) handSize )
    return;
  m_OnTurn->discardCard ( a );
  endTurn();
}

shared_ptr<CPlayer> CGameStateManager::pickPlayer ( void ) {
  wclear ( m_Info );
  string str = "Pick a player";
  string str2 = "(UP/DOWN ARROW)";
  mvwprintw ( m_Info, getmaxy(m_Info)/2 - 1, getmaxx(m_Info)/2 - str.size()/2, "%s", str.c_str() );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, getmaxx(m_Info)/2 - str2.size()/2, "%s", str2.c_str() );
  wrefresh ( m_Info );
  keypad ( stdscr, TRUE );
  int a = m_OnTurn->readReceiver();
  keypad ( stdscr, FALSE );
  switch ( a ) {
  case KEY_UP:
    return m_Player1;
  case KEY_DOWN:
    return m_Player2;
  case 'q':
    break;
  }
  return nullptr;
}
void CGameStateManager::playCard ( size_t i ) {
  if ( ! m_OnTurn->hasEnoughMana ( i ) ) {
    wclear ( m_Info );
    mvwprintw ( m_Info, getmaxy(m_Info)/2-1, (getmaxx(m_Info)/2) - 8 ,"Not enough mana." );
    mvwprintw ( m_Info, getmaxy(m_Info)/2, (getmaxx(m_Info)/2) - 13 ,"Press any key to continue." );
    wrefresh ( m_Info );
    getch();
    return;
  }
  shared_ptr<CPlayer> receiver = pickPlayer();
  if ( ! receiver )
    return;
  m_OnTurn->playCard ( i, receiver );
}

void CGameStateManager::printWinner ( shared_ptr<CPlayer> winner ) {
  wclear ( m_Info );
  wattron (m_Info, A_BLINK );
  mvwprintw ( m_Info, getmaxy(m_Info)/2-1, (getmaxx(m_Info)/2) - ( winner->getName().size() + 5 ) / 2 ,"%s WON!", winner->getName().c_str() );
  wattroff (m_Info, A_BLINK );
  wattron ( m_Info, A_BOLD );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, (getmaxx(m_Info)/2) - 11 ,"Press any key to return" );
  mvwprintw ( m_Info, getmaxy(m_Info)/2+1, (getmaxx(m_Info)/2) - 8 ,"to the main menu." );
  wattroff ( m_Info, A_BOLD );
  wrefresh ( m_Info );
  getch();
}
bool CGameStateManager::winnerDecided ( void ) {
  if ( ! m_Player1->isAlive() ) {
    printWinner ( m_Player2 );    
    return true;
  }
  else if ( ! m_Player2->isAlive() ) {
    printWinner ( m_Player1 );
    return true;
  }
  return false;
}
void CGameStateManager::printMess ( const string & mess  ) {
  wclear ( m_Info );
  mvwprintw ( m_Info, getmaxy(m_Info)/2-1, getmaxx(m_Info)/2 - mess.size()/2, "%s", mess.c_str() );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, getmaxx(m_Info)/2 - 13, "Press any key to continue." );
  wrefresh ( m_Info );
  getch();
}
bool CGameStateManager::savePlayers ( fs::path & saveDir ) {
  CSaver saver;
  try {
    saver.createDirectory ( saveDir );
    fs::path p1SaveLocation = saveDir.generic_string() + "/P1_" + m_Player1->getName();
    fs::path p2SaveLocation = saveDir.generic_string() + "/P2_" + m_Player2->getName();
    saver.createDirectory ( p1SaveLocation );
    saver.createDirectory ( p2SaveLocation );
    if ( ! m_Player1->save ( p1SaveLocation ) || ! m_Player2->save ( p2SaveLocation ) )
      return false;
  }
  catch ( const exception & e ) {
    printMess ( e.what() );
    return false;
  }
  return true;
}
bool CGameStateManager::saveSettings ( fs::path & saveDir ) {
  ofstream ofs ( saveDir.generic_string() + "/" + defaultSettingsFileName );
  if ( ! ofs.good() )
    return false;
  m_Settings.setFirstOnTurn ( m_OnTurn == m_Player1 ? true : false );
  m_Settings.dumpInfo ( ofs );
  return true;
}
void CGameStateManager::saveGame ( void ) {
  fs::path saveDir = defaultSaveLocation.generic_string() +  "/save_game_" + m_Player1->getName() + "-" + m_Player2->getName();
  if ( ! savePlayers ( saveDir ) || ! saveSettings ( saveDir ) ) {
    printMess ( "Failed to save." );
    fs::remove (saveDir);
  }
  printMess ( "Game saved." );
}
int CGameStateManager::readUserInput ( void ) {
  return m_OnTurn->readAction();
}