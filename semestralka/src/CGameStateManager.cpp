#include "CGameStateManager.h"

CGameStateManager::CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett )
: m_Player1 ( p1 ), m_Player2 ( p2 ), m_Settings ( sett ), m_OnTurn ( nullptr ), m_Info ( nullptr ) {}

bool CGameStateManager::dealCards ( void ) {
  m_Player1->shuffleDeck();
  m_Player2->shuffleDeck();
  m_Player1->drawCard(handSize);
  m_Player2->drawCard(handSize);
  m_OnTurn = m_Player1;
  return true;
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
  wclear ( m_Info );
  mvwprintw ( m_Info, getmaxy(m_Info)/2-1, (getmaxx(m_Info)/2) - 5 ,"Turn ended." );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, (getmaxx(m_Info)/2) - 13 ,"Press any key to continue." );
  wrefresh ( m_Info );
  wgetch ( m_Info );
}

int CGameStateManager::pickCard ( void ) {
  wclear ( m_Info );
  mvwprintw ( m_Info, getmaxy(m_Info)/2, (getmaxx(m_Info)/2) - 13 ,"Pick a card to throw away." );
  wrefresh ( m_Info );
  return getch() - '0';
}

void CGameStateManager::discardCard ( void ) {
  m_OnTurn->discardCard ( pickCard () );
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
  int a = getch();
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
// should display some message if not enough mana
void CGameStateManager::playCard ( size_t i ) {
  if ( ! m_OnTurn->tmp_hasEnoughMana ( i ) )
    return;
  shared_ptr<CPlayer> receiver = pickPlayer();
  if ( ! receiver )
    return;
  m_OnTurn->playCard ( i, m_OnTurn, receiver );
}
