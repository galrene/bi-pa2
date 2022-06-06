#pragma once
#include "CCharacter.h"
#include "CDeck.h"
#include <ncurses.h>
#include <string>
using namespace std;
 
// ! spravit abstraktne

class CPlayer {
  public:
    CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );

    CCharacter m_PlayedCharacter; // character representing current state
    bool operator == ( CPlayer & rhs );
    // void loadDeck ( CDeck & deck );
    void drawCard ( void );
    void playCard ( void );
    void save ( string path );
    void load ( string path );

    void shuffleDeck ( void );
    void renderStats ( WINDOW * win );
    void renderCard ( WINDOW * win, size_t & i );

  protected:
    string m_Name;
    CCharacter m_LoadedCharacter; // character representing default state
    CDeck m_Deck;
    CDeck m_Hand;
    bool m_Turn;
};
/*
class CComputerPlayerAtt : public CPlayer {
  protected:
  public:
};
*/