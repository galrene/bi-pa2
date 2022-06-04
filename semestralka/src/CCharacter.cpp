#include "CCharacter.h"

CCharacter::CCharacter ( string name, string charClass, int hp, int mana, int strength, int defense )
: m_Name ( name ), m_Class ( charClass ), m_HP ( hp ), m_Mana ( mana ), m_Strength ( strength ), m_Defense ( defense ) {}
CCharacter::CCharacter ( map <string,string> data )
: m_DataSource ( data ) {}

string CCharacter::getHeader ( void ) {
  return m_Name + "_" + m_Class;
}
string CCharacter::getName ( void ) {
  return m_Name;
}
bool CCharacter::containsDeps ( map <string,string> & data ) {
  if ( data["name"] == "" || data["class"] == "" || data["mana"] == "" || data["hp"] == ""
      || data["strength"] == "" || data["defense"] == "" )
    return false;
  return true;
}

bool CCharacter::buildCharacter ( void ) {
  if ( ! containsDeps ( m_DataSource ) )
    return false;
  m_Name = m_DataSource["name"];
  m_Class = m_DataSource["class"];
  m_HP = stoi(m_DataSource["hp"]);
  m_Mana = stoi(m_DataSource["mana"]);
  m_Strength = stoi(m_DataSource["strength"]);
  m_Defense = stoi(m_DataSource["defense"]);
  return true;
}

void CCharacter::dumpInfo ( ostream & os ) {
  os << "[character]" << endl;
  os << "name = " << m_Name << endl;
  os << "class = " << m_Class << endl;
  os << "hp = " << m_HP << endl;
  os << "mana = " << m_Mana << endl;
  os << "strength = " << m_Strength << endl;
  os << "defense = " << m_Defense << endl;
}