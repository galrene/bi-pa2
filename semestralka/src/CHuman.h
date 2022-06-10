#pragma once
#include "CPlayer.h"

class CHuman : public CPlayer {
  public:
    CHuman ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );
    CHuman ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand );

    virtual int readAction ( void ) override;
    virtual int readReceiver ( void ) override;

  protected:
};