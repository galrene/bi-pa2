#pragma once
#ifndef CGameStateManager_H
#define CGameStateManager_H
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
     * @return 
     */
    bool dealCards  ( void );
    bool endGame    ( void );
    void playCard ( size_t i );
    /**
     * @brief Discard a card from hand and return it back into the deck
     * 
     * @param i index of the card in hand
     */
    void discardCard ( size_t i );
    bool loadCards  ( void );
    bool saveGame   ( void );
    bool loadGame   ( void );
    /**
     * @brief Increment turn count, fill up current player's hand, switch turns.
     * 
     */
    void endTurn    ( void );
    bool loadPlayer ( CPlayer & player );
    /**
     * @brief clear, box, refresh window, also render given player's stats
     * 
     * @param win where to render
     * @param player, whose stats to render -> 1 or 2
     */
    void renderPlayerStats ( WINDOW * win, short player );
    /**
     * @brief render player's cards on hand, clear and refresh each card window
     * 
     * @param cardWindows where to render cards
     * @param player, whose hand to render -> 1 or 2
     */
    void renderPlayerHand ( vector<WINDOW*> cardWindows, short player );
    /**
     * @brief prints who's turn it is at given coordinates
     * 
     * @param y 
     * @param x 
     */
    void whoIsOnTurn ( int yCoord, int xCoord );
    /**
     * @brief pick one of two players ( KEY_UP = player1, KED_DOWN = player2 )
     * 
     * @return shared_ptr<CPlayer> picked player
     */
    shared_ptr<CPlayer> pickPlayer ( void );
  protected:
    size_t m_TurnNumber;
    shared_ptr<CPlayer> m_Player1;
    shared_ptr<CPlayer> m_Player2;
    CGameSettings m_Settings;
    shared_ptr<CPlayer> m_OnTurn;
};
#endif