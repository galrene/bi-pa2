#include "CPlayer.h"

CPlayer::CPlayer ( string name, CCharacter character )
: m_Name ( name ), m_LoadedCharacter ( character ), m_PlayedCharacter ( character ) {}


bool CPlayer::operator == ( CPlayer & rhs ) {
  return rhs.m_Name == m_Name;
}