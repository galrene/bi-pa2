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
/**
 * @brief Print stats to window, don't refresh, don't clear
 * 
 * @param win window for printing
 */
void CCharacter::renderStats ( WINDOW * win ) {
  int strengthPos = 2;
  init_pair ( 1, COLOR_WHITE, COLOR_RED ); // hp color
  init_pair ( 2, COLOR_WHITE, COLOR_BLUE ); // mana color
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  string strength = "Strength: " + to_string(m_Strength);
  mvwprintw ( win, yMax/2, strengthPos, "%s", strength.c_str() );
  string defense = "Defense: " + to_string(m_Defense);
  mvwprintw ( win, yMax/2, strengthPos + strength.size() + 2, "%s", defense.c_str() );
  wattron ( win, COLOR_PAIR ( 1 ) );
  string hp = "HP: " + to_string(m_HP);
  string mana = "Mana: " + to_string(m_Mana); 
  mvwprintw ( win, yMax/2, xMax - 2 - mana.size() - 2 - hp.size(), "%s", hp.c_str() );
  wattroff ( win, COLOR_PAIR ( 1 ) );
  wattron ( win, COLOR_PAIR ( 2 ) );
  mvwprintw ( win, yMax/2, xMax - 2 - mana.size(), "%s", mana.c_str() );
  wattroff ( win, COLOR_PAIR ( 2 ) );
}
/*
int CCharacter::applyEffect ( CEffect eff ) {
  if ( eff.m_HPDiff >= 0 )
    m_HP += eff.m_HPDiff;
  else {
    if ( eff.m_HPDiff + m_Defense <= 0 )
      return 1;
    else
    m_HP += eff.m_HPDiff + m_Defense;
  }
  m_Mana += eff.m_ManaDiff;
  m_Strength += eff.m_StrengthDiff;
  m_Defense += eff.m_DefenseDiff;
  return 1;
}
*/
int CCharacter::applyEffect ( CEffect eff ) {
  m_HP += eff.m_HPDiff;
  m_Mana += eff.m_ManaDiff;
  m_Strength += eff.m_StrengthDiff;
  m_Defense += eff.m_DefenseDiff;
  return 1;
}
