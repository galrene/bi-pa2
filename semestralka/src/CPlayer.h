#pragma once
#include "CCharacter.h"
#include "CDeck.h"
#include <string>
using namespace std;
 
// ! spravit abstraktne

class CPlayer {
  protected:
    string m_Name;
    CCharacter m_LoadedCharacter; // character representing default state
    CDeck m_Deck;
    CDeck m_Hand;
  public:
    CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );

    CCharacter m_PlayedCharacter; // character representing current state
    bool operator == ( CPlayer & rhs );
    // void loadDeck ( CDeck & deck );
    void drawCard ( void );
    void playCard ( void );
    void save ( string path );
    void load ( string path );
};
/*
class CComputerPlayerAtt : public CPlayer {
  protected:
  public:
};
*/