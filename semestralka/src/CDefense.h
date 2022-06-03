#pragma once
#include "CCard.h"

class CDefense : public CCard {
  public:
    CDefense ( string name, string type, int cost, int heal );
    CDefense ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    virtual void dumpInfo ( ostream & os ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_Heal;
};