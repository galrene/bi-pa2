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

void CAttack::render ( WINDOW * win ) {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  init_pair ( 3, COLOR_WHITE, COLOR_YELLOW ); // cost color
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );

  mvwprintw ( win, yMax - 3, xMax / 2 - strlen("Deals:") / 2, "Deals:" );
  string damage = to_string ( m_Damage ) + " Damage";
  mvwprintw ( win, yMax - 2, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
}