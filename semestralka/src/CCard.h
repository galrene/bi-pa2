#pragma once
#include <string>
#include <map>
#include "CPlayer.h"

using namespace std;

class CCard {
  protected:
    string m_Name;
    string m_Type;
    map<string,string> m_Data;
    int m_Cost;
  public:
    virtual ~CCard ( void ) noexcept = default;

    CCard ( string name, string type, int cost );
    CCard ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) = 0;
    virtual bool containsDeps ( map <string,string> & data ) = 0; 
    virtual bool buildCard ( void ) = 0;
    // virtual void renderCard ( win, y, x ) = 0; na vykreslenie karty
    // virtual shared_ptr<CCard> create ( void ) = 0; na vytvorenie shared_ptr
    /* for saving */
    // virtual void dumpInfo ( void ) = 0;
};

class CAttack : public CCard {
  public:
    CAttack ( string name, string type, int cost, int dmg );
    CAttack ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
    virtual bool buildCard ( void ) override;
    // virtual void dumpInfo ( void ) override;
  protected:
    int m_Damage;
};

class CDefense : public CCard {
  public:
    CDefense ( string name, string type, int cost, int heal );
    CDefense ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_Heal;
};

class CPassive : public CCard {
  public:
    CPassive ( string name, string type, int cost, int heal, int dmg, size_t dur );
    CPassive ( map <string,string> & data );
    
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_Heal;
    int m_Damage;
    size_t m_Duration;
};

class CSpecial : public CCard {
  public:
    CSpecial ( string name, string type, int cost, int strDiff, int DefDiff, int manaDiff );
    CSpecial ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) override;
    // virtual void dumpInfo ( void ) override;
    virtual bool buildCard ( void ) override;
    virtual bool containsDeps ( map <string,string> & data ) override;
  protected:
    int m_StrengthDiff;
    int m_DefenseDiff;
    int m_ManaDiff;
};
//-----------------------------------------------------------------------
CCard::CCard ( string name, string type, int cost )
: m_Name ( name ), m_Type ( type ), m_Cost ( cost ) {}

CCard::CCard ( map <string,string> & data )
: m_Data ( map<string,string> ( data ) ) {}
//-----------------------------------------------------------------------
CAttack::CAttack ( string name, string type, int cost, int dmg )
: CCard ( name, type, cost ), m_Damage ( dmg ) {}
CAttack::CAttack ( map <string,string> & data )
: CCard ( data ) {}

bool CAttack::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["damage"] == "" )
    return false;
  return true;
}
/**
 * @brief construct card from provided map<string,string> data source
 * 
 * @return true success
 * @return false data source doesnt contain all necessary values
 */
bool CAttack::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_Damage = stoi(m_Data["damage"]);
  return true;
}
// void CAttack::useCard ( CPlayer & user, CPlayer & opponent ) {
//  opponent.m_PlayedCharacter
// }
//-----------------------------------------------------------------------
CDefense::CDefense ( string name, string type, int cost, int heal )
: CCard ( name, type, cost ), m_Heal ( heal ) {}

CDefense::CDefense ( map <string,string> & data )
: CCard ( data ) {}

bool CDefense::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["heal"] == "" )
    return false;
  return true;
}
bool CDefense::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_Heal = stoi(m_Data["heal"]);
  return true;
}
//-----------------------------------------------------------------------
CPassive::CPassive ( string name, string type, int cost, int heal, int dmg, size_t dur )
: CCard ( name, type, cost ), m_Heal ( heal ), m_Damage ( dmg ), m_Duration ( dur ) {}

CPassive::CPassive ( map <string,string> & data )
: CCard ( data ) {}

bool CPassive::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["heal"] == ""
      || data["damage"] == "" || data["duration"] == "" )
    return false;
  return true;
}
bool CPassive::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_Heal = stoi(m_Data["heal"]);
  m_Damage = stoi(m_Data["damage"]);
  m_Duration = stoi(m_Data["duration"]);
  return true;
}
//-----------------------------------------------------------------------
CSpecial::CSpecial ( string name, string type, int cost, int strDiff, int defDiff, int manaDiff )
: CCard ( name, type, cost ), m_StrengthDiff ( strDiff ), m_DefenseDiff ( defDiff ), m_ManaDiff ( manaDiff ) {}

CSpecial::CSpecial ( map <string,string> & data )
: CCard ( data ) {}

bool CSpecial::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["strengthDiff"] == ""
      || data["defenseDiff"] == "" || data["manaDiff"] == "" )
    return false;
  return true;
}

bool CSpecial::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_ManaDiff = stoi(m_Data["manaDiff"]);
  m_StrengthDiff = stoi(m_Data["strengthDiff"]);
  m_DefenseDiff = stoi(m_Data["defenseDiff"]);
  return true;
}