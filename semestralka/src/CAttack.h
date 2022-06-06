#pragma once
#include "CCard.h"

class CAttack : public CCard {
  public:
    CAttack ( string name, string type, int cost, int dmg );
    CAttack ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    virtual bool buildCard ( void ) override;
    virtual void dumpInfo ( ostream & os ) override;
    virtual void render ( WINDOW * win ) override;

  protected:
    int m_Damage;
};
