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

    void shuffleDeck ( void );
    /**
     * @brief render player name, character stats
     * 
     * @param win 
     */
    void renderPlayer ( WINDOW * win );
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
     * @brief Top up cards on hand to default hand size
     * 
     */
    void fillHand ( void );
    /**
     * @brief Top up mana
     * 
     */
    void fillMana ( void );
    /**
     * @brief discard a card and draw a new one
     * 
     * @param i index of the discarded card
     */
    void discardCard ( size_t i );
    /**
     * @brief check if player has enough mana to play card
     * 
     * @param i card index
     */
    bool tmp_hasEnoughMana ( size_t i );
    string getName ( void ) { return m_Name; }

  protected:
    string m_Name;
    CCharacter m_LoadedCharacter; // character representing default state
    CDeck m_Deck;
    CDeck m_Hand;
};
/*
class CComputerPlayerAtt : public CPlayer {
  protected:
  public:
};
*/
#endif