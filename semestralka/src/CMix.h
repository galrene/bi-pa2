#pragma once
#include "CCard.h"
/**
 * @brief Card combining dealing damage and healing.
 */
class CMix : public CCard {
  public:
    CMix ( const string & name, const string & type, int cost, int heal, int dmg );
    CMix ( map <string,string> & data );
    virtual ~CMix ( void ) = default;
    virtual pair<CEffect,CEffect> getEffects ( void ) const override;
    virtual void dumpInfo ( ostream & os ) const override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) const override;
    virtual void render ( WINDOW * win ) const override;
  protected:
    int m_Heal;
    int m_Damage;
};