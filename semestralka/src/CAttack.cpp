#include "CAttack.h"

CAttack::CAttack ( const string & name, const string & type, int cost, int dmg )
: CCard ( name, type, cost ), m_Damage ( dmg ) {}

CAttack::CAttack ( map <string,string> & data )
: CCard ( data ) {}

bool CAttack::containsDeps ( map <string,string> & data ) const {
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

void CAttack::dumpInfo ( ostream & os ) const {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "damage = " << m_Damage << endl;
}

void CAttack::render ( WINDOW * win ) const {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );
  mvwprintw ( win, yMax - 3, xMax / 2 - 3, "Deals:" );
  string damage = to_string ( m_Damage ) + " Damage";
  mvwprintw ( win, yMax - 2, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
}

pair<CEffect,CEffect> CAttack::getEffects ( void ) const {
  return make_pair<CEffect,CEffect> ( CEffect ( -m_Damage ), CEffect ( 0, -m_Cost ) );
}