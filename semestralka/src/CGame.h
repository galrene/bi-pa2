#pragma once
#include "CGameStateManager.h"

class CGame {
  public:
    CGame ( CGameStateManager & gsm );
    ~CGame ( void );
    /**
     * @brief Start a new game from scratch.
     * 
     * @return true = game ended, false = CTRL-D called
     */
    bool beginGame ( void );
    /**
     * @brief Start a loaded game from a savefile.
     * 
     * @return true = game ended, false = CTRL-D called
     */
    bool continueGame ( void );
  protected:
  /**
   * @brief Initialize ncurses.
   * 
   * @return true = success, false = terminal doesn't support colors
   */
    bool initCurses ( void );
    /**
     * @brief Draws the game controls, players' stats, hands.
     */
    void drawLayout ( void );
    /**
     * @brief Control the game using user input.
     * 
     * @return true = game finished or requested exit to main menu, false = CTRL-D called
     */
    bool handleGame ( void );
    int m_xMax, m_yMax;
    CGameStateManager m_Gsm;
    int m_CardHeight, m_CardWidth;
    int m_StatsHeight, m_StatsWidth;
    int m_InfoWidth;
};