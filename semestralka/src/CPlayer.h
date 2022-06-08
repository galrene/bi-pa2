#pragma once
#ifndef CPlayer_H
#define CPlayer_H
#include <ncurses.h>
#include <string>
#include "CCharacter.h"
#include "CDeck.h"
#include "Constants.h"
using namespace std;
 
// ! spravit abstraktne

class CPlayer {
  public:
    CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );
    ~CPlayer ( void );
    CCharacter m_PlayedCharacter; // character representing current state
    bool operator == ( CPlayer & rhs );
    // void loadDeck ( CDeck & deck );
    /**
     * @brief draw cards from deck to hand
     * 
     * @param cnt amount of cards to draw
     */
    void drawCard ( size_t cnt );
    /**
     * @brief play a card from hand at index i
     * 
     * @param i card index
     * @param user the one who played the card
     * @param receiver receiver of effects
     */
    void playCard ( size_t i, shared_ptr<CPlayer> & user, shared_ptr<CPlayer> & receiver );
    void save ( string path );
    void load ( string path );
    /**
     * @brief Shuffle the loaded deck.
     * 
     */
    void shuffleDeck ( void );
    /**
     * @brief render player name, character stats
     * 
     * @param win 
     */
    void renderPlayer ( void );
    /**
     * @brief render box, card index, card
     * 
     * @param win where to render
     * @param i card index
     */
    void renderCard ( WINDOW * win, size_t & i );
    /**
     * @brief render player name
     * 
     * @param win where to render
     * @param yCoord 
     * @param xCoord 
     */
    void renderName ( WINDOW * win, int yCoord, int xCoord );
    /**
     * @brief Render the whole hand.
     * 
     * @param cardWindows where to render
     */
    void renderHand ( void );
    /**
     * @brief Hide the whole hand.
     * 
     * @param cardWindows what to hide
     */
    void hideHand ( void );
    /**
     * @brief Top up cards on hand to default hand size.
     * 
     */
    void fillHand ( void );
    /**
     * @brief Top up mana.
     * 
     */
    void fillMana ( void );
    /**
     * @brief Discard a card and draw a new one.
     * 
     * @param i index of the discarded card
     */
    void discardCard ( size_t i );
    /**
     * @brief Check if player has enough mana to play card
     * 
     * @param i card index
     */
    bool tmp_hasEnoughMana ( size_t i );
    string getName ( void ) { return m_Name; }
    void setStatsWin ( WINDOW * win ) { m_StatsWin = win; }
    void setCardWins ( vector<WINDOW*> & cardWins ) { m_HandWins = cardWins; }
    bool isAlive ( void ) { return m_PlayedCharacter.getHP() > 0; }
  protected:
    string m_Name;
    CCharacter m_LoadedCharacter; // character representing default state
    CDeck m_Deck;
    CDeck m_Hand;

    vector<WINDOW*> m_HandWins;
    WINDOW* m_StatsWin;
};
/*
class CComputerPlayerAtt : public CPlayer {
  protected:
  public:
};
*/
#endif