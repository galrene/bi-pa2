#include "CCard.h"

CCard::CCard ( string name, string type, int cost )
: m_Name ( name ), m_Type ( type ), m_Cost ( cost ) {}

CCard::CCard ( map <string,string> & data )
: m_Data ( map<string,string> ( data ) ) {}

bool CCard::operator == ( CCard & rhs ) { return rhs.m_Name == m_Name; }

string CCard::getHeader ( void ) {
  return m_Name + "_" + m_Type;
}
string CCard::getName ( void ) {
  return m_Name;
}