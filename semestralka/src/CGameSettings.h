#pragma once
#include <memory>
#include "CCard.h"
#include "CDeck.h"
#include "Constants.h"

using namespace std;

class CGameSettings {
  protected:
    /**
     * @brief true = Two player game, false = Single player game.
     */
    bool m_TwoPlayerGame;
    size_t m_MaxDeckSize;
    /**
     * @brief true = players see each other's hands, false = only on turn player's hand visible
     */
    bool m_CheekyMode;
    /**
     * @brief true = Player1, false = Player2
     */
    bool m_FirstOnTurn;
  public:
    CGameSettings ( void );
    CGameSettings ( bool twoPlayerGame, size_t maxDeckSize, bool cheeky, bool firstOnTurn );
    /**
     * @brief Dump settings inside filestream.
     * 
     * @param os where to dump info
     */
    void dumpInfo ( ostream & os );
    /**
     * @brief Switch from 1vs1 vs 1vsBOT and vice versa.
     */
    void toggleSP ( void ) { m_TwoPlayerGame == true ? m_TwoPlayerGame = false : m_TwoPlayerGame = true;}
    /**
     * @brief Toggle being able to see each other's hands.
     */
    void toggleCheeky ( void ) { m_CheekyMode == true ? m_CheekyMode = false : m_CheekyMode = true; }
    /**
     * @return true = 1vs1 false = 1vsBOT
     */
    bool isTwoPlayerGame ( void ) const { return m_TwoPlayerGame; }
    /**
     * @brief Can players see into each other's hands? 
     */
    bool isCheeky ( void ) const { return m_CheekyMode; }
    /**
      * @brief Sets who should be next on turn after starting the game.
      * 
      * @return true = player1, false = player2
      */
    void setFirstOnTurn ( bool firstOnTurn ) { m_FirstOnTurn = firstOnTurn; }
    /**
      * @brief Who is should be next on turn after starting the game.
      * 
      * @return true = player1, false = player2
      */
    bool firstOnTurn ( void ) const { return m_FirstOnTurn; }
    void setMaxDeckSize ( size_t newSize ) { m_MaxDeckSize = newSize; }
    size_t getMaxDeckSize ( void ) const { return m_MaxDeckSize; }
    /**
     * @brief Load settings from key value pairs
     * 
     * @param data 
     * @return true = sucess, false = not enough data to set all required attributes
     */
    bool load ( map<string,string> & data );
};