#pragma once
#include <memory>
#include "CGameSettings.h"
#include "CPlayer.h"
#include "CSaver.h"
#include "ncurses.h"

using namespace std;

class CGameStateManager {
  public:
    CGameStateManager ( void ) = default;
    CGameStateManager ( shared_ptr<CPlayer> p1, shared_ptr<CPlayer> p2, CGameSettings sett );
    ~CGameStateManager ( void );
    /**
     * @brief Shuffle decks, deal hands.
     */
    void dealCards  ( void );
    /**
     * @brief Pick who to use card on, use it if user has enough mana.
     * 
     * @param i index of card to use in hand
     */
    void playCard ( size_t i );
    /**
     * @brief Discard a card from hand and return it back into the deck.
     * 
     * @param i index of the card in hand
     */
    void discardCard ( void );
    /**
     * @brief Prompt the user to pick a card for throwing away.
     * 
     * @return int picked card index
     */
    int pickCard ( void );
    /**
     * @brief Save the game.
     */
    void saveGame ( void );
    /**
     * @brief Save players inside a directory.
     * 
     * @param saveDir path to directory where to save
     */
    bool savePlayers ( fs::path & saveDir );
    /**
     * @brief Save settings inside a directory.
     * 
     * @param saveDir path to directory where to save
     */
    bool saveSettings ( fs::path & saveDir );
    /**
     * @brief Top up current player's hand, switch turns, hide hand if cheeky mode is off, prompt for continuing.
     */
    void endTurn ( void );
    /**
     * @brief Clear, box, refresh window, render both players' stats.
     */
    void renderPlayerStats ( void );
    /**
     * @brief Render player's cards on hand, clear and refresh each card window
     */
    void renderHands ( void );
    /**
     * @brief Print who's turn it is, centered in the middle
     */
    void whoIsOnTurn ( void );
    /**
     * @brief Pick one of two players ( KEY_UP = player1, KED_DOWN = player2 )
     *
     * @return shared_ptr<CPlayer> picked player
     */
    shared_ptr<CPlayer> pickPlayer ( void );
    /**
     * @brief Set both players' windows for rendering their cards and stats.
     */
    void setWindows ( vector<WINDOW*> & p1_cards, WINDOW * p1_stats, vector<WINDOW*> & p2_cards, WINDOW * p2_stats, WINDOW * info );
    /**
     * @brief Check if one of the players is dead.
     * 
     * @return false = winner undecided yet, true = winner decided
     */
    bool winnerDecided ( void );
    /**
     * @brief Print who won inside the info window.
     * 
     * @param winner Player who won.
     */
    void printWinner ( shared_ptr<CPlayer> winner );
    /**
     * @brief Draw in-game menu.
     * 
     * @return 0 = exit to menu, 1 = save game, 2 or any other unused button = return to game, -1 = CTRL-D
     */
    int handleMenu ( void );
    /**
     * @brief Print a blocking message to the info screen.
     * 
     * @param mess message to print
     */
    void printMess ( const string & mess  );
    /**
     * @brief Decide who should be next on turn.
     */
    void decideTurnFromSettings ( void );
    /**
     * @brief Read input from the currently on turn player.
     * 
     * @return int received input
     */
    int readUserInput ( void );
  protected:
    shared_ptr<CPlayer> m_Player1;
    shared_ptr<CPlayer> m_Player2;
    CGameSettings m_Settings;
    shared_ptr<CPlayer> m_OnTurn;
    
    WINDOW * m_Info;
};