#include "CSpecial.h"

CSpecial::CSpecial ( const string & name, const string & type, int cost, int strDiff, int defDiff, int manaDiff )
: CCard ( name, type, cost ), m_StrengthDiff ( strDiff ), m_DefenseDiff ( defDiff ), m_ManaDiff ( manaDiff ) {}

CSpecial::CSpecial ( map <string,string> & data )
: CCard ( data ) {}

bool CSpecial::containsDeps ( map <string,string> & data ) const {
  if ( data["name"] == "" || data["type"] == "" || data["manaCost"] == "" || data["strengthDiff"] == ""
      || data["defenseDiff"] == "" || data["manaDiff"] == "" )
    return false;
  return true;
}
bool CSpecial::buildCard ( void ) {
  if ( ! containsDeps ( m_Data ) )
    return false;
  m_Name = m_Data["name"];
  m_Type = m_Data["type"];
  m_Cost = stoi(m_Data["manaCost"]);
  m_ManaDiff = stoi(m_Data["manaDiff"]);
  m_StrengthDiff = stoi(m_Data["strengthDiff"]);
  m_DefenseDiff = stoi(m_Data["defenseDiff"]);
  return true;
}
void CSpecial::dumpInfo ( ostream & os ) const {
  os << "[card]" << endl;
  os << "type = " << m_Type << endl;
  os << "name = " << m_Name << endl;
  os << "manaCost = " << m_Cost << endl;
  os << "manaDiff = " << m_ManaDiff << endl;
  os << "strengthDiff = " << m_StrengthDiff << endl;
  os << "defenseDiff = " << m_DefenseDiff << endl;
}

void CSpecial::render ( WINDOW * win ) const {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  mvwprintw ( win, 1, xMax / 2 - m_Name.size() / 2, "%s", m_Name.c_str() );
  mvwprintw ( win, 2, xMax / 2 - m_Type.size() / 2, "%s", m_Type.c_str() );
  wattron ( win, COLOR_PAIR(2) );
  mvwprintw ( win, 1, xMax - 2, "%d", m_Cost );
  wattroff ( win, COLOR_PAIR(2) );
  if ( m_StrengthDiff ) {
    string str = "Strength" + to_string ( m_StrengthDiff );
    mvwprintw ( win, yMax - 4, xMax / 2 - str.size() / 2, "%s", str.c_str() );
  }
  if ( m_DefenseDiff ) {
    string def = "Defense " + to_string ( m_DefenseDiff );
    mvwprintw ( win, yMax - 3, xMax / 2 - def.size() / 2, "%s", def.c_str() );
  }
  if ( m_ManaDiff ) {
    string mana = "Mana " + to_string ( m_ManaDiff );
    mvwprintw ( win, yMax - 2, xMax / 2 - mana.size() / 2, "%s", mana.c_str() );
  }
}

pair<CEffect,CEffect> CSpecial::getEffects ( void ) const {
  return make_pair<CEffect,CEffect> ( CEffect ( 0, m_ManaDiff, m_StrengthDiff, m_DefenseDiff ), CEffect ( 0, -m_Cost ) );
}
