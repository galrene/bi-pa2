#pragma once
#include "CPlayer.h"

class CBot : public CPlayer {
  public:
    CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );
    CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand );

    virtual int readAction ( void ) override;
    virtual int readReceiver ( void ) override;
    int getMostDmgCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay );
    int getMostHealCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay );
    vector<pair<shared_ptr<CCard>,size_t>> canPlay ( void );
  protected:
    bool m_UseOnEnemy;
};
