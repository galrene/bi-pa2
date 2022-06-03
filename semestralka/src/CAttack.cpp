#include "CAttack.h"

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

void CAttack::dumpInfo ( ostream & os ) {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "damage = " << m_Damage << endl;
}
// void CAttack::useCard ( CPlayer & user, CPlayer & opponent ) {
//  opponent.m_PlayedCharacter
// }