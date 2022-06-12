#pragma once
#include "CCard.h"
/**
 * @brief A card for altering character attributes.
 * 
 */
class CSpecial : public CCard {
  public:
    CSpecial ( const string & name, const string & type, int cost, int strDiff, int DefDiff, int manaDiff );
    CSpecial ( map <string,string> & data );

    virtual pair<CEffect,CEffect> getEffects ( void ) const override;
    virtual void dumpInfo ( ostream & os ) const override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) const override;
    virtual void render ( WINDOW * win ) const override;
  protected:
    int m_StrengthDiff;
    int m_DefenseDiff;
    int m_ManaDiff;
};