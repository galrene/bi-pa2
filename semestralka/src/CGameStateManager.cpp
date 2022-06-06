#include "CGameStateManager.h"

CGameStateManager::CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett )
: m_Player1 ( p1 ), m_Player2 ( p2 ), m_Settings ( sett ) {}

bool CGameStateManager::beginGame ( void ) {
  m_Player1->shuffleDeck();
  m_Player2->shuffleDeck();
  for ( size_t i = 0; i < 6; i++ ) {
    m_Player1->drawCard();
    m_Player2->drawCard();
  }
  /* nejak riesit ze kto je na tahu */
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

/*
void CGameStateManager::playCard ( CCard & card, CPlayer & user ) {
  
  user == m_Player1
  ? card.useCard ( m_Player1, m_Player2 )
  : card.useCard ( m_Player2, m_Player1 );
}
*/