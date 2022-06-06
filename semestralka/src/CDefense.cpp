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

void CDefense::render ( WINDOW * win ) {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  init_pair ( 3, COLOR_WHITE, COLOR_YELLOW ); // cost color
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );

  mvwprintw ( win, yMax - 3, xMax / 2 - strlen("Heals:") / 2, "Heals:" );
  string damage = to_string ( m_Heal ) + " Health";
  mvwprintw ( win, yMax - 2, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
}