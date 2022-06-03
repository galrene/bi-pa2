#pragma once
#include "CCharacter.h"
// #include "CDeck.h"
#include <string>
using namespace std;
 
// ! spravit abstraktne

class CPlayer {
  protected:
    // CDeck m_Deck;
    // CDeck m_Hand;
    string m_Name;
    CCharacter m_LoadedCharacter; // character representing default state
  public:
    CPlayer ( void );
    CPlayer ( string name, CCharacter character );

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