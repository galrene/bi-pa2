#pragma once

#include <memory>
#include "CCard.h"
#include "CDeck.h"
#include "Constants.h"

using namespace std;

class CGameSettings {
  protected:    
    bool m_TwoPlayerGame;
    size_t m_MaxDeckSize;
  public:
    CGameSettings ( void );
    // switch from single player to 1v1 and vice versa
    void toggleSP ( void ) { m_TwoPlayerGame == true ? m_TwoPlayerGame = false : m_TwoPlayerGame = true;}
    bool isTwoPlayerGame ( void ) { return m_TwoPlayerGame; }
    void setMaxDeckSize ( size_t newSize ) { m_MaxDeckSize = newSize; }
    size_t getMaxDeckSize ( void ) { return m_MaxDeckSize; }
};