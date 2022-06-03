#pragma once

#include <memory>
#include "CCard.h"
#include "CDeck.h"
#include "Constants.h"

using namespace std;

class CGameSettings {
  protected:    
    bool m_TwoPlayerGame;
    int m_MaxDeckSize;
    friend class CApplication_v2;
  public:
    CGameSettings ( void )
    : m_TwoPlayerGame ( false ), m_MaxDeckSize ( defaultDeckSize ) {}
    // switch from single player to 1v1 and vice versa
    void toggleSP ( void ) { m_TwoPlayerGame == true
                             ? m_TwoPlayerGame = false
                             : m_TwoPlayerGame = true;}
    bool isTwoPlayerGame ( void ) { return m_TwoPlayerGame; }
    void setMaxDeckSize ( int newSize ) { m_MaxDeckSize = newSize; }
    int getMaxDeckSize ( void ) { return m_MaxDeckSize; }
};