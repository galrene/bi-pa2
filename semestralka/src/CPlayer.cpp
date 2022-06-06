#include "CPlayer.h"

CPlayer::CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: m_Name ( name ), m_LoadedCharacter ( currentCharacter ), m_PlayedCharacter ( defaultCharacter ), m_Deck ( deck ) {}


bool CPlayer::operator == ( CPlayer & rhs ) {
  return rhs.m_Name == m_Name;
}

void CPlayer::renderStats ( WINDOW * win ) {
  int yMax, xMax;
  getmaxyx ( win, yMax, xMax );
  string name = m_Name.append("'s ").append(m_PlayedCharacter.getName()) ;
  mvwprintw ( win, yMax/2, xMax/2 - name.size()/2, "%s", name.c_str() );
  m_PlayedCharacter.renderStats ( win );
}

void CPlayer::shuffleDeck ( void ) {
  m_Deck.shuffleCards();
}

void CPlayer::drawCard ( void ) {
  m_Hand.addCard ( m_Deck.drawCard() );
}

void CPlayer::renderCard ( WINDOW * win, size_t & i ) {
  box ( win, 0, 0 );
  m_Hand.renderCard ( win, i );
}