#pragma once

class CEffect {
  public:
    CEffect ( int hpDiff = 0, int manaDiff = 0, int strDiff = 0, int defDiff = 0 );
    int m_HPDiff;
    int m_ManaDiff;
    int m_StrengthDiff;
    int m_DefenseDiff;
};

