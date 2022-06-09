#include "CGameSettings.h"

CGameSettings::CGameSettings ( void )
: m_TwoPlayerGame ( defaultTwoPlayerGame ), m_MaxDeckSize ( defaultDeckSize ), m_CheekyMode ( defaultHandVisibility ),
m_FirstOnTurn ( defaultFirstOnTurn ) {}

CGameSettings::CGameSettings ( bool twoPlayerGame, size_t maxDeckSize, bool cheeky, bool firstOnTurn )
: m_TwoPlayerGame ( twoPlayerGame ), m_MaxDeckSize ( maxDeckSize ), m_CheekyMode ( cheeky ), m_FirstOnTurn ( firstOnTurn ) {}

bool CGameSettings::load ( map<string,string> & data ) {
    if ( data.count("TwoPlayerGame") == 0 || data.count("MaxDeckSize") == 0
        || data.count("CheekyMode") == 0 || data.count("FirstOnTurn") == 0 )
        return false;
    m_TwoPlayerGame = stoi(data["TwoPlayerGame"]);
    m_MaxDeckSize = stoi(data["MaxDeckSize"]);
    m_CheekyMode = stoi(data["CheekyMode"]);
    m_FirstOnTurn = stoi(data["FirstOnTurn"]);
    return true;
}

void CGameSettings::dumpInfo ( ostream & os ) {
    os << "[settings]" << endl;
    os << "TwoPlayerGame = " << m_TwoPlayerGame << endl;
    os << "MaxDeckSize = " << m_MaxDeckSize << endl;
    os << "CheekyMode = " << m_CheekyMode << endl;
    os << "FirstOnTurn = " << m_FirstOnTurn << endl;
}
