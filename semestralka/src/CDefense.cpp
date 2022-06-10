#include "CDefense.h"

CDefense::CDefense ( const string & name, const string & type, int cost, int heal )
: CCard ( name, type, cost ), m_Heal ( heal ) {}

CDefense::CDefense ( map <string,string> & data )
: CCard ( data ) {}

bool CDefense::containsDeps ( map <string,string> & data ) const {
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
void CDefense::dumpInfo ( ostream & os ) const {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "heal = " << m_Heal << endl;
}

void CDefense::render ( WINDOW * win ) const {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );

  mvwprintw ( win, yMax - 3, xMax / 2 - 3, "Heals:" );
  string damage = to_string ( m_Heal ) + " Health";
  mvwprintw ( win, yMax - 2, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
}
pair<CEffect, CEffect> CDefense::getEffects ( void ) const {
  return make_pair <CEffect, CEffect> ( CEffect ( m_Heal ), CEffect ( 0, -m_Cost ) );
}
