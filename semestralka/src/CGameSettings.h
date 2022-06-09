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
    /**
     * @brief Who is first on turn -> true = Player1, false = Player2
     * 
     */
    bool m_FirstOnTurn;
  public:
    CGameSettings ( void );
    /**
     * @brief Dumps settings inside filestream
     * 
     * @param os 
     */
    void dumpInfo ( ostream & os );
    /**
     * @brief Switch from 1v1 vs 1vBOT and vice versa
     * 
     */
    void toggleSP ( void ) { m_TwoPlayerGame == true ? m_TwoPlayerGame = false : m_TwoPlayerGame = true;}
    void toggleCheeky ( void ) { m_CheekyMode == true ? m_CheekyMode = false : m_CheekyMode = true; }
    bool isTwoPlayerGame ( void ) { return m_TwoPlayerGame; }
    bool isCheeky ( void ) { return m_CheekyMode; }
    void setMaxDeckSize ( size_t newSize ) { m_MaxDeckSize = newSize; }
    void p1OnTurn ( bool p1onTurn ) { m_FirstOnTurn = p1onTurn; }
    bool firstOnTurn ( void ) { return m_FirstOnTurn; }
    size_t getMaxDeckSize ( void ) { return m_MaxDeckSize; }
};