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
    /**
     * @brief pick who to use card on, use it if user has enough mana
     * 
     * @param i index of card to in hand
     * @param win window for prompting the receiver pick
     */
    void playCard ( size_t i );
    /**
     * @brief Discard a card from hand and return it back into the deck
     * 
     * @param i index of the card in hand
     */
    void discardCard ( void );
    int pickCard ( void );
    bool loadCards  ( void );
    bool saveGame   ( void );
    bool loadGame   ( void );
    /**
     * @brief Fill up current player's hand, switch turns, hide hand if cheeky mode is off, prompt for continuing.
     */
    void endTurn    ( void );
    bool loadPlayer ( CPlayer & player );
    /**
     * @brief clear, box, refresh window, render both player's stats
     */
    void renderPlayerStats ( void );
    /**
     * @brief render player's cards on hand, clear and refresh each card window
     * 
     * @param cardWindows where to render cards
     * @param player, whose hand to render -> 1 or 2
     */
    void renderHands ( void );
    /**
     * @brief prints who's turn it is, centered in the middle
     * 
     * @param win where to print 
     */
    void whoIsOnTurn ( void );
    /**
     * @brief pick one of two players ( KEY_UP = player1, KED_DOWN = player2 )
     *
     * @param win window where to prompt the user to pick
     * @return shared_ptr<CPlayer> picked player
     */
    shared_ptr<CPlayer> pickPlayer ( void );
    /**
     * @brief Set both player's windows where the will render their cards and stats
     */
    void setWindows ( vector<WINDOW*> & p1_cards, WINDOW * p1_stats, vector<WINDOW*> & p2_cards, WINDOW * p2_stats, WINDOW * info );

  protected:
    shared_ptr<CPlayer> m_Player1;
    shared_ptr<CPlayer> m_Player2;
    CGameSettings m_Settings;
    shared_ptr<CPlayer> m_OnTurn;
    
    WINDOW * m_Info;
};
#endif