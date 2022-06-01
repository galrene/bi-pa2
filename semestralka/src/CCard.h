#pragma once
#include <string>
#include <map>
#include "CPlayer.h"

using namespace std;

class CCard {
  protected:
    string m_Name;
    string m_Type;
    int m_Cost;

  public:
    virtual ~CCard ( void ) noexcept = default;

    CCard ( string name, string type, int cost )
    : m_Name ( name ), m_Type ( type ), m_Cost ( cost ) {}
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) = 0;
      
    virtual bool containsDeps ( map <string,string> & data ) = 0; 
    // virtual void renderCard ( win, y, x ) = 0; na vykreslenie karty
    // virtual shared_ptr<CCard> create ( void ) = 0; na vytvorenie shared_ptr
    /* for saving */
    // virtual void dumpInfo ( void ) = 0;
};

class CAttack : public CCard {
  public:
    CAttack ( string name, string type, int cost, int dmg );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    // virtual void dumpInfo ( void ) override;
  protected:
    int m_Damage;
};

class CDefense : public CCard {
  public:
    CDefense ( string name, string type, int cost, int heal );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_Heal;
};

class CPassive : public CCard {
  public:
    CPassive ( string name, string type, int cost, int heal, int dmg, size_t dur );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_Heal;
    int m_Damage;
    size_t m_Duration;
};

class CSpecial : public CCard {
  public:
    CSpecial ( string name, string type, int cost, int strDiff, int DefDiff, int manaDiff );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_StrengthDiff;
    int m_DefenseDiff;
    int m_ManaDiff;
};
//-----------------------------------------------------------------------
CAttack::CAttack ( string name, string type, int cost, int dmg )
: CCard ( name, type, cost ), m_Damage ( dmg ) {}

bool CAttack::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["damage"] == "" )
    return false;
  return true;
}
// void CAttack::useCard ( CPlayer & user, CPlayer & opponent ) {
//  opponent.m_PlayedCharacter
// }
//-----------------------------------------------------------------------
CDefense::CDefense ( string name, string type, int cost, int heal )
: CCard ( name, type, cost ), m_Heal ( heal ) {}

bool CDefense::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["heal"] == "" )
    return false;
  return true;
}
//-----------------------------------------------------------------------
CPassive::CPassive ( string name, string type, int cost, int heal, int dmg, size_t dur )
: CCard ( name, type, cost ), m_Heal ( heal ), m_Damage ( dmg ), m_Duration ( dur ) {}

bool CPassive::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["heal"] == ""
      || data["damage"] == "" || data["duration"] == "" )
    return false;
  return true;
}
//-----------------------------------------------------------------------
CSpecial::CSpecial ( string name, string type, int cost, int strDiff, int defDiff, int manaDiff )
: CCard ( name, type, cost ), m_StrengthDiff ( strDiff ), m_DefenseDiff ( defDiff ), m_ManaDiff ( manaDiff ) {}

bool CSpecial::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["strengthDiff"] == ""
      || data["defenseDiff"] == "" || data["manaDiff"] == "" )
    return false;
  return true;
}