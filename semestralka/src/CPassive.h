#pragma once
#ifndef CPassive_H
#define CPassive_H
#include "CCard.h"

class CPassive : public CCard {
  public:
    CPassive ( string name, string type, int cost, int heal, int dmg, size_t dur );
    CPassive ( map <string,string> & data );
    
    virtual pair<CEffect,CEffect> getEffects ( void ) override;
    virtual void dumpInfo ( ostream & os ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    virtual void render ( WINDOW * win ) override;
  protected:
    int m_Heal;
    int m_Damage;
    size_t m_Duration;
};
#endif