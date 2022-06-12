#pragma once
#include <ncurses.h>
#include <string>
#include "CCharacter.h"
#include "CDeck.h"
#include "Constants.h"
using namespace std;
 
class CPlayer {
  public:
    CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );
    CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand );
    virtual ~CPlayer ( void );
    /**
     * @brief Compare two players by their names.
     * 
     * @param rhs 
     */
    bool operator == ( CPlayer & rhs ) const { return rhs.m_Name == m_Name; }
    /**
     * @brief Draw cards from deck to hand.
     * 
     * @param cnt amount of cards to draw
     */
    void drawCard ( size_t cnt );
    /**
     * @brief Play a card from hand at index i.
     * 
     * @param i card index
     * @param receiver receiver of effects
     */
    void playCard ( size_t i, shared_ptr<CPlayer> & receiver );
    /**
     * @brief Shuffle the loaded deck.
     */
    void shuffleDeck ( void );
    /**
     * @brief Render player name, character stats.
     */
    void renderPlayer ( void ) const ;
    /**
     * @brief Render box, card index, card.
     * 
     * @param win where to render
     * @param i card index
     */
    void renderCard ( WINDOW * win, size_t & i ) const ;
    /**
     * @brief Render player name.
     * 
     * @param win where to render
     * @param yCoord 
     * @param xCoord 
     */
    void renderName ( WINDOW * win, int yCoord, int xCoord ) const ;
    /**
     * @brief Render the whole hand.
     * 
     * @param cardWindows where to render
     */
    void renderHand ( void ) const ;
    /**
     * @brief Hide the whole hand.
     */
    void hideHand ( void ) const ;
    /**
     * @brief Top up cards on hand to default hand size.
     */
    void fillHand ( void );
    /**
     * @brief Top up mana.
     */
    void fillMana ( void );
    /**
     * @brief Discard a card and draw a new one.
     * 
     * @param i index of the discarded card
     */
    void discardCard ( size_t i );
    /**
     * @brief Check if player has enough mana to play card.
     * 
     * @param i card index
     */
    bool hasEnoughMana ( size_t i ) const ;
    /**
     * @brief Save player info.
     * 
     * @param dirName Where to save
     * @return true saved succesfully
     * @return false couldn't create some of the required files
     */
    bool save ( fs::path & playerDir );
    /**
     * @brief Read player input.
     * 
     * @return int read input
     */
    virtual int readAction ( void ) = 0;
    /**
     * @brief Read which player should receive a picked card's effects.
     * 
     * @return int KEY_UP == Player1, KEY_DOWN == Player2
     */
    virtual int readReceiver ( void ) const = 0;
    string getName ( void ) { return m_Name; }
    void setStatsWin ( WINDOW * win ) { m_StatsWin = win; }
    void setCardWins ( vector<WINDOW*> & cardWins ) { m_HandWins = cardWins; }
    bool isAlive ( void ) { return m_PlayedCharacter.getHP() > 0; }
  protected:
    /**
     * @brief Character representing current state
     */
    CCharacter m_PlayedCharacter;
    string m_Name;
    /**
     * @brief Character representing default state with attributes loaded by default
     */
    CCharacter m_LoadedCharacter;
    CDeck m_Deck;
    CDeck m_Hand;
    vector<WINDOW*> m_HandWins;
    WINDOW * m_StatsWin;
};