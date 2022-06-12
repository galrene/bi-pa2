#include "CMix.h"

CMix::CMix ( const string & name, const string & type, int cost, int heal, int dmg )
: CCard ( name, type, cost ), m_Heal ( heal ), m_Damage ( dmg ) {}

CMix::CMix ( map <string,string> & data )
: CCard ( data ) {}

bool CMix::containsDeps ( map <string,string> & data ) const {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["heal"] == ""
      || data["damage"] == "" )
    return false;
  return true;
}
bool CMix::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_Heal = stoi(m_Data["heal"]);
  m_Damage = stoi(m_Data["damage"]);
  return true;
}
void CMix::dumpInfo ( ostream & os ) const {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "heal = " << m_Heal << endl;
  os << "damage = " << m_Damage << endl;
}
void CMix::render ( WINDOW * win ) const {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );
  if ( m_Heal ) {
    string heal = "+" + to_string ( m_Heal ) + " health";
    mvwprintw ( win, yMax - 3, xMax / 2 - heal.size() / 2, "%s", heal.c_str() );
  }
  if ( m_Damage ) {
    string damage = "Deal " + to_string ( m_Damage ) + " damage";
    mvwprintw ( win, yMax - 2, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
  }
}
pair<CEffect,CEffect> CMix::getEffects ( void ) const {
  return make_pair<CEffect,CEffect> ( CEffect ( -m_Damage ), CEffect ( m_Heal, -m_Cost ) );
}