#include "CGameSettings.h"
CGameSettings::CGameSettings ( void )
: m_TwoPlayerGame ( defaultTwoPlayerGame ), m_MaxDeckSize ( defaultDeckSize ), m_CheekyMode ( defaultHandVisibility ),
m_FirstOnTurn ( defaultFirstOnTurn ) {}

void CGameSettings::dumpInfo ( ostream & os ) {
    os << "TwoPlayerGame = " << m_TwoPlayerGame << endl;
    os << "MaxDeckSize = " << m_MaxDeckSize << endl;
    os << "CheekyMode = " << m_CheekyMode << endl;
    os << "FirstOnTurn = " << m_FirstOnTurn << endl;
}
