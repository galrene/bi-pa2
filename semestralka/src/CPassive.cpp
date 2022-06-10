#include "CPassive.h"

CPassive::CPassive ( const string & name, const string & type, int cost, int heal, int dmg, size_t dur )
: CCard ( name, type, cost ), m_Heal ( heal ), m_Damage ( dmg ), m_Duration ( dur ) {}

CPassive::CPassive ( map <string,string> & data )
: CCard ( data ) {}

bool CPassive::containsDeps ( map <string,string> & data ) const {
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
void CPassive::dumpInfo ( ostream & os ) const {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "heal = " << m_Heal << endl;
  os << "damage = " << m_Damage << endl;
  os << "duration = " << m_Duration << endl;
}

void CPassive::render ( WINDOW * win ) const {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );
  if ( m_Heal ) {
    string heal = "+" + to_string ( m_Heal ) + " health";
    mvwprintw ( win, yMax - 4, xMax / 2 - heal.size() / 2, "%s", heal.c_str() );
  }
  if ( m_Damage ) {
    string damage = "Deal " + to_string ( m_Damage ) + " damage";
    mvwprintw ( win, yMax - 3, xMax / 2 - damage.size() / 2, "%s", damage.c_str() );
  }
  string duration = "Duration: " + to_string ( m_Duration ) + " rounds";
  mvwprintw ( win, yMax - 2 , xMax / 2 - duration.size() / 2, "%s", duration.c_str() );
}

pair<CEffect,CEffect> CPassive::getEffects ( void ) const {
  return make_pair<CEffect,CEffect> ( CEffect ( m_Heal-m_Damage ), CEffect ( 0, -m_Cost ) );
}
