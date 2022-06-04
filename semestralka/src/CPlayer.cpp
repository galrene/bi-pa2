#include "CPlayer.h"

CPlayer::CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: m_Name ( name ), m_LoadedCharacter ( currentCharacter ), m_PlayedCharacter ( defaultCharacter ), m_Deck ( deck ) {}


bool CPlayer::operator == ( CPlayer & rhs ) {
  return rhs.m_Name == m_Name;
}