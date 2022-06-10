#include "CCard.h"

CCard::CCard ( const string & name, const string & type, int cost )
: m_Name ( name ), m_Type ( type ), m_Cost ( cost ) {}

CCard::CCard ( map <string,string> & data )
: m_Data ( map<string,string> ( data ) ) {}