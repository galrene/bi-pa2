#pragma once
#include <memory>
#include "CGameSettings.h"
#include "CPlayer.h"
#include "ncurses.h"
using namespace std;

class CGameStateManager {
  public:
    CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett );

    bool beginGame  ( void );
    bool endGame    ( void );
    bool playCard ( CCard & card, CPlayer & user );
    bool loadCards  ( void );
    bool saveGame   ( void );
    bool loadGame   ( void );
    bool endTurn    ( void );
    bool loadPlayer ( CPlayer & player );
  protected:
    size_t m_TurnNumber;
    shared_ptr<CPlayer> m_Player1;
    shared_ptr<CPlayer> m_Player2;
    CGameSettings m_Settings;
    // ?  CPlayer m_OnTurn;     // might be useful, make as a bool inside CPlayer though
};

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