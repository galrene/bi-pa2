#pragma once

#include "CGameSettings.h"
#include "CPlayer.h"
using namespace std;

class CGameStateManager {
  protected:
    size_t m_TurnNumber;
    CPlayer m_Player1;
    CPlayer m_Player2;
    // ?  CPlayer m_OnTurn;     // might be useful, make as a bool inside CPlayer though
  public:
    CGameStateManager ( CPlayer p1, CPlayer p2 );

    void beginGame  ( void );
    void endGame    ( void );
    void playCard ( CCard & card, CPlayer & user );

    void loadCards  ( void );
    void saveGame   ( void );
    void loadGame   ( void );
    void endTurn    ( void );
    void loadPlayer ( CPlayer & player );
};
CGameStateManager::CGameStateManager ( CPlayer p1, CPlayer p2 )
: m_Player1 ( p1 ), m_Player2 ( p2 ) {}


/*
void CGameStateManager::playCard ( CCard & card, CPlayer & user ) {
  
  user == m_Player1
  ? card.useCard ( m_Player1, m_Player2 )
  : card.useCard ( m_Player2, m_Player1 );
}
*/