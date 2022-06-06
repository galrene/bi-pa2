#pragma once
#include <memory>
#include "CGameSettings.h"
#include "CPlayer.h"
#include "ncurses.h"
using namespace std;

class CGameStateManager {
  public:
    CGameStateManager ( void ) = default;
    CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett );
    /**
     * @brief Shuffle decks, deal hands, set P1's turn 
     * ! change name to something better ! maybe initGame()
     * @return 
     */
    bool beginGame  ( void );
    bool endGame    ( void );
    bool playCard ( CCard & card, CPlayer & user );
    bool loadCards  ( void );
    bool saveGame   ( void );
    bool loadGame   ( void );
    bool endTurn    ( void );
    bool loadPlayer ( CPlayer & player );

    void renderPlayerStats ( WINDOW * win, short player );
    void renderPlayerHand ( vector<WINDOW*> p1Cards, short player );
  protected:
    size_t m_TurnNumber;
    shared_ptr<CPlayer> m_Player1;
    shared_ptr<CPlayer> m_Player2;
    CGameSettings m_Settings;
    // ?  CPlayer m_OnTurn;     // might be useful, make as a bool inside CPlayer though
};
