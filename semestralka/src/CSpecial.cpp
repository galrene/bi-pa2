#include "CSpecial.h"

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
void CSpecial::dumpInfo ( ostream & os ) {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "manaDiff = " << m_ManaDiff << endl;
  os << "strengthDiff = " << m_StrengthDiff << endl;
  os << "defenseDiff = " << m_DefenseDiff << endl;
}