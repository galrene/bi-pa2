#include "CGameStateManager.h"

CGameStateManager::CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett )
: m_Player1 ( p1 ), m_Player2 ( p2 ), m_Settings ( sett ) {}

bool CGameStateManager::beginGame ( void ) {
  return true;
}

/*
void CGameStateManager::playCard ( CCard & card, CPlayer & user ) {
  
  user == m_Player1
  ? card.useCard ( m_Player1, m_Player2 )
  : card.useCard ( m_Player2, m_Player1 );
}
*/