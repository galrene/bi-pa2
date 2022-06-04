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
    bool drawGame  ( void );
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
