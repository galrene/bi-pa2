#pragma once
#include "CCard.h"

class CSpecial : public CCard {
  public:
    CSpecial ( string name, string type, int cost, int strDiff, int DefDiff, int manaDiff );
    CSpecial ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    virtual void dumpInfo ( ostream & os ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    virtual void render ( WINDOW * win ) override;
  protected:
    int m_StrengthDiff;
    int m_DefenseDiff;
    int m_ManaDiff;
};