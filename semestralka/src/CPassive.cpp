#include "CPassive.h"

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
void CPassive::dumpInfo ( ostream & os ) {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "heal = " << m_Heal << endl;
  os << "damage = " << m_Damage << endl;
  os << "duration = " << m_Duration << endl;
}