#pragma once
#include "CCard.h"
/**
 * @brief Attack card for dealing damage.
 * 
 */
class CAttack : public CCard {
  public:
    CAttack ( const string & name, const string & type, int cost, int dmg );
    CAttack ( map <string,string> & data );
    virtual ~CAttack ( void ) = default;
    virtual pair<CEffect,CEffect> getEffects ( void ) const override;
    virtual bool containsDeps ( map <string,string> & data ) const override;
    virtual bool buildCard ( void ) override;
    virtual void dumpInfo ( ostream & os ) const override;
    virtual void render ( WINDOW * win ) const override;
  protected:
    int m_Damage;
};