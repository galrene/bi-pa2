#pragma once
#include "CCard.h"
#ifndef CDefense_H
#define CDefense_H
class CDefense : public CCard {
  public:
    CDefense ( string name, string type, int cost, int heal );
    CDefense ( map <string,string> & data );

    virtual pair<CEffect,CEffect> getEffects ( void ) override;
    virtual void dumpInfo ( ostream & os ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    virtual void render ( WINDOW * win ) override;
  protected:
    int m_Heal;
};
#endif