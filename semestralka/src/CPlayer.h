#include "CCharacter.h"
#include "CDeck.h"
#include <string>
using namespace std;

class CPlayer {
  protected:
    CCharacter m_Character;
    CDeck m_Deck;
    CDeck m_Hand;
    string m_Name;
  public:
    void loadDeck ( CDeck & deck );
    void drawCard ( void );
    void playCard ( void );
    void save ( string path );
    void load ( string path );
};

class CComputerPlayerAtt : public CPlayer {
  protected:
  public:
};

class CComputerPlayerDef : public CPlayer {
  protected:
  public:
};