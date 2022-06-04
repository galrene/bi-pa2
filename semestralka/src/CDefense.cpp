#include "CDefense.h"

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
void CDefense::dumpInfo ( ostream & os ) {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "heal = " << m_Heal << endl;
}