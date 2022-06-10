#pragma once
#include "CCard.h"
class CDefense : public CCard {
  public:
    CDefense ( const string & name, const string & type, int cost, int heal );
    CDefense ( map <string,string> & data );

    virtual pair<CEffect,CEffect> getEffects ( void ) const override;
    virtual void dumpInfo ( ostream & os ) const override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) const override;
    virtual void render ( WINDOW * win ) const override;
  protected:
    int m_Heal;
};