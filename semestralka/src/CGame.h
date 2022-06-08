#pragma once
#ifndef CGame_H
#define CGame_H
#include "CGameStateManager.h"
#include "CWindow.h"

class CGame {
  public:
    CGame ( CGameStateManager gsm );
    ~CGame ( void );
    bool beginGame ( void );

  protected:
    bool initCurses ( void );
    void drawLayout ( void );

    int m_xMax, m_yMax;
    CGameStateManager m_Gsm;
    int m_CardHeight, m_CardWidth;
    int m_StatsHeight, m_StatsWidth;
    int m_InfoWidth;
};
#endif