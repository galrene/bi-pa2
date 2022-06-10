#include "CHuman.h"

CHuman::CHuman ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: CPlayer ( name, defaultCharacter, currentCharacter, deck )  {}

CHuman::CHuman ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand )
: CPlayer ( name, defaultCharacter, currentCharacter, deck, hand ) {}

int CHuman::readAction ( void ) {
  return getch();
}
int CHuman::readReceiver ( void ) const {
  return getch();
}