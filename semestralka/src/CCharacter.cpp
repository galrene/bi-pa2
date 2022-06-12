#include "CCharacter.h"

CCharacter::CCharacter ( const string & name, const string & charClass, int hp, int mana, int strength, int defense )
: m_Name ( name ), m_Class ( charClass ), m_HP ( hp ), m_Mana ( mana ), m_Strength ( strength ), m_Defense ( defense ) {}
CCharacter::CCharacter ( map <string,string> & data )
: m_DataSource ( data ) {}

bool CCharacter::containsDeps ( map <string,string> & data ) const {
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
  if ( m_HP < 1 || m_Mana < 1 || m_Strength < 1 || m_Defense < 1 )
    return false;
  return true;
}
void CCharacter::dumpInfo ( ostream & os ) const {
  os << "[character]" << endl;
  os << "name = " << m_Name << endl;
  os << "class = " << m_Class << endl;
  os << "hp = " << m_HP << endl;
  os << "mana = " << m_Mana << endl;
  os << "strength = " << m_Strength << endl;
  os << "defense = " << m_Defense << endl;
}
void CCharacter::renderStats ( WINDOW * win ) const {
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
void CCharacter::applyEffect ( const CEffect & eff ) {
  // here should be a calculation of actual effects based on stats
  m_HP += eff.m_HPDiff;
  m_Mana += eff.m_ManaDiff;
  m_Strength += eff.m_StrengthDiff;
  m_Defense += eff.m_DefenseDiff;
}
