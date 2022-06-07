#include "CGameStateManager.h"

CGameStateManager::CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett )
: m_Player1 ( p1 ), m_Player2 ( p2 ), m_Settings ( sett ) {}

bool CGameStateManager::dealCards ( void ) {
  m_Player1->shuffleDeck();
  m_Player2->shuffleDeck();
  m_Player1->drawCard(handSize);
  m_Player2->drawCard(handSize);
  m_OnTurn = m_Player1;
  return true;
}
void CGameStateManager::renderPlayerStats ( WINDOW * win, short player ) {
  wclear ( win );
  box ( win, 0 ,0 );
  switch ( player ) {
  case 1:
    m_Player1->renderPlayer ( win );
    break;
  case 2:
    m_Player2->renderPlayer ( win );
    break;
  default:
    break;
  }
  wrefresh ( win );
}

void CGameStateManager::renderPlayerHand ( vector<WINDOW*> cardWindows, short player ) {
  switch ( player ) {
  case 1:
    for ( size_t i = 0; i < cardWindows.size(); i++ ) {
      wclear ( cardWindows[i] );
      m_Player1->renderCard ( cardWindows[i], i );
      wrefresh ( cardWindows[i] );
    }
    break;
  case 2:
    for ( size_t i = 0; i < cardWindows.size(); i++ ) {
      wclear ( cardWindows[i] );
      m_Player2->renderCard ( cardWindows[i], i );
      wrefresh ( cardWindows[i] );
    }
    break;
  default:
    break;
  }
}

void CGameStateManager::whoIsOnTurn ( WINDOW * win ) {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  wclear ( win );
  string str = m_OnTurn->getName() + "'s turn";;
  if ( m_OnTurn == m_Player1 ) {
    mvwaddch ( win, yMax/2, xMax/2, ACS_UARROW );
    mvwprintw  ( win, yMax/2 - 1, xMax/2 - str.size() / 2, "%s", str.c_str() );
  }
  else {
    mvwaddch ( win, yMax/2, xMax/2, ACS_DARROW );
    mvwprintw  ( win, yMax/2 + 1, xMax/2 - str.size() / 2, "%s", str.c_str() );
  }
  wrefresh(win);
}

void CGameStateManager::endTurn ( void ) {
  m_OnTurn->fillHand();
  m_OnTurn->fillMana();
  m_OnTurn == m_Player1
  ? m_OnTurn = m_Player2
  : m_OnTurn = m_Player1;
  m_TurnNumber++;
}

void CGameStateManager::discardCard ( size_t i ) {
  m_OnTurn->discardCard ( i );
  endTurn();
}

shared_ptr<CPlayer> CGameStateManager::pickPlayer ( WINDOW * win ) {
  wclear ( win );
  string str = "Pick a player";
  string str2 = "(UP/DOWN ARROW)";
  mvwprintw ( win, getmaxy(win)/2 - 1, getmaxx(win)/2 - str.size()/2, "%s", str.c_str() );
  mvwprintw ( win, getmaxy(win)/2, getmaxx(win)/2 - str2.size()/2, "%s", str2.c_str() );
  wrefresh ( win );
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
void CGameStateManager::playCard ( size_t i, WINDOW * win ) {
  if ( ! m_OnTurn->tmp_hasEnoughMana ( i ) )
    return;
  shared_ptr<CPlayer> receiver = pickPlayer(win);
  if ( ! receiver )
    return;
  m_OnTurn->playCard ( i, m_OnTurn, receiver );
}
