#include <memory>
#include "CCard.h"
#include "CDeck.h"
using namespace std;

class CGameSettings {
  protected:
    bool m_TwoPlayerGame;
    int m_MaxDeckSize;
  public:
    CGameSettings ()
    : m_TwoPlayerGame ( false ), m_MaxDeckSize ( 16 ) {}
    // switch from single player to 1v1 and vice versa
    void toggleSP ( void ) { m_TwoPlayerGame == true
                             ? m_TwoPlayerGame = false
                             : m_TwoPlayerGame = true;}
    void setMaxDeckSize ( int newSize ) { m_MaxDeckSize = newSize; }
};