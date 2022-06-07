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
    m_Player1->renderStats ( win );
    break;
  case 2:
    m_Player2->renderStats ( win );
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
  default:
    break;
  }
}

void CGameStateManager::whoIsOnTurn ( int yCoord, int xCoord ) {
  // m_OnTurn->renderName ( stdscr, yCoord, xCoord );
  // printw(" ", yCoord, xCoord );
  if ( m_OnTurn == m_Player1 )
    mvaddch ( yCoord, xCoord, ACS_UARROW );
  else
    mvaddch ( yCoord, xCoord, ACS_DARROW );
  refresh();
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

shared_ptr<CPlayer> CGameStateManager::pickPlayer ( void ) {
  keypad ( stdscr, TRUE );
  int a = getch();
  keypad ( stdscr, FALSE );
  // ! printovat daco nech vi typek co robit
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
