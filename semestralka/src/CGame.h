#pragma once
#ifndef CGame_H
#define CGame_H
#include "CGameStateManager.h"

class CGame {
  public:
    CGame ( CGameStateManager & gsm );
    ~CGame ( void );
    bool beginGame ( void );
    bool continueGame ( void );

  protected:
    bool initCurses ( void );
    void drawLayout ( void );
    bool handleGame ( void );
  
    int m_xMax, m_yMax;
    CGameStateManager m_Gsm;
    int m_CardHeight, m_CardWidth;
    int m_StatsHeight, m_StatsWidth;
    int m_InfoWidth;
};
#endif