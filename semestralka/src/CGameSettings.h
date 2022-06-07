#pragma once

#include <memory>
#include "CCard.h"
#include "CDeck.h"
#include "Constants.h"

using namespace std;

class CGameSettings {
  protected:    
    bool m_TwoPlayerGame;
    /**
     * @brief true = players see each other's hands, false = only on turn player's hand visible
     * 
     */
    size_t m_MaxDeckSize;
    bool m_CheekyMode;
  public:
    CGameSettings ( void );
    // switch from single player to 1v1 and vice versa
    void toggleSP ( void ) { m_TwoPlayerGame == true ? m_TwoPlayerGame = false : m_TwoPlayerGame = true;}
    void toggleCheeky ( void ) { m_CheekyMode == true ? m_CheekyMode = false : m_CheekyMode = true; }
    bool isTwoPlayerGame ( void ) { return m_TwoPlayerGame; }
    bool isCheeky ( void ) { return m_CheekyMode; }
    void setMaxDeckSize ( size_t newSize ) { m_MaxDeckSize = newSize; }
    size_t getMaxDeckSize ( void ) { return m_MaxDeckSize; }
};